#include "g_local.h"
#include "m_mutant.h"

extern void Wireframe_TeleportNear(edict_t* target);

#define SNITCH_MODEL					"models/monsters/mutant/tris.md2"
#define SNITCH_HEAR_DIST				384.0f
#define SNITCH_SCREAM_COOLDOWN			5.0_sec
#define SNITCH_SANITY_DRAIN				5
#define SNITCH_SPAWN_SANITY_PCT			0.40f
#define SNITCH_WALK_SPEED_THRESH		100.0f
#define SNITCH_WANDER_RADIUS			1000.0f
#define SNITCH_STEP_SIZE				300.0f

constexpr spawnflags_t SPAWNFLAG_SNITCH_ALWAYS_ACTIVE = 8_spawnflag;

static void snitch_stand(edict_t* self);
static void snitch_walk(edict_t* self);
static void snitch_run(edict_t* self);
static void snitch_scream(edict_t* self);
static void snitch_pain(edict_t* self, edict_t* other, float kick, int damage, const mod_t& mod);
static void snitch_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point, const mod_t& mod);
static void snitch_active_think(edict_t* self);
static void snitch_dormant(edict_t* self);
void snitch_start_go(edict_t* self);
void Snitch_DoScream(edict_t* self);
void snitch_wander(edict_t* self, float dist);
bool Snitch_CheckAttack(edict_t* self);
void snitch_static_idle(edict_t* self, float dist);
void SP_monster_snitch(edict_t* self);


static bool Snitch_CanHear(edict_t* self, edict_t* target) {
	if (!target || !target->client)
		return false;

	if (target->client->is_holding_breath)
		return false;

	float dist = (target->s.origin - self->s.origin).length();

	if (dist <= SNITCH_HEAR_DIST) {
		float speed = target->velocity.length();
		bool is_crouching = (target->client->ps.pmove.pm_flags & PMF_DUCKED);

		if (speed > SNITCH_WALK_SPEED_THRESH && !is_crouching) {
			return true;
		}
	}

	edict_t* noise = NULL;
	while ((noise = G_Find(noise, [](edict_t* e) {return e->inuse && e->classname && !strcmp(e->classname, "player_noise"); }))) {

		if (noise->owner == target && noise->timestamp > level.time - 1.0_sec) {
			float noise_dist = (noise->s.origin - self->s.origin).length();
			if (noise_dist < SNITCH_HEAR_DIST * 3.0f) {
				return true;
			}
		}

	}

	return false;
}

static bool Snitch_Look(edict_t* self) {
	int i;
	edict_t* ent;
	bool found = false;

	for (i = 1; i <= game.maxclients; i++) {
		ent = &g_edicts[i];
		if (!ent->inuse || ent->health <= 0 || ent->flags & FL_NOTARGET)
			continue;

		if (Snitch_CanHear(self, ent)) {
			self->enemy = ent;
			self->monsterinfo.last_sighting = ent->s.origin;
			self->monsterinfo.trail_time = level.time;
			found = true;
			break;
		}
	}
	if (found) {
		return true;
	}
	else {
		if (self->enemy && (level.time - self->monsterinfo.trail_time > 0.5_sec)) {
			self->enemy = NULL;
		}
		return(self->enemy != NULL);
	}
}

void Snitch_PickWanderPoint(edict_t* self) {
	if (!self->target_ent) return;

	vec3_t forward, right, up;
	vec3_t start, end;
	trace_t tr;

	for (int attempts = 0; attempts < 10; attempts++) {
		vec3_t angles;
		angles[0] = 0;
		angles[1] = frandom() * 360.0f;
		angles[2] = 0;
		AngleVectors(angles, forward, right, up);

		float dist = 100.0f + (frandom() * (SNITCH_STEP_SIZE-100.0f));
		start[0] = self->s.origin[0];
		start[1] = self->s.origin[1];
		start[2] = self->s.origin[2]+24;
		
		end[0] = start[0] + (forward[0] * dist);
		end[1] = start[1] + (forward[1] * dist);
		end[2] = start[2] + (forward[2] * dist);

		float dist_from_home = (end - self->pos1).length();
		if (dist_from_home > SNITCH_WANDER_RADIUS) {
			vec3_t dir_home = self->pos1 - self->s.origin;
			dir_home.normalize();
			end[0] = self->s.origin[0] + (dir_home[0] * dist);
			end[1] = self->s.origin[1] + (dir_home[1] * dist);
			end[2] = self->s.origin[2] + (dir_home[2] * dist);
		}

		tr = gi.trace(start, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
		if (tr.fraction < 1.0f) continue;

		vec3_t down;
		down[0] = end[0];
		down[1] = end[1];
		down[2] = end[2]-300;

		tr = gi.trace(end, self->mins, self->maxs, down, self, MASK_MONSTERSOLID);

		if (tr.fraction < 1.0f && !tr.startsolid && !tr.allsolid) {
			self->target_ent->s.origin[0] = tr.endpos[0];
			self->target_ent->s.origin[1] = tr.endpos[1];
			self->target_ent->s.origin[2] = tr.endpos[2]+16;
		
			self->goalentity = self->target_ent;

			self->monsterinfo.pausetime = level.time + 5.0_sec;
			return;
		}
	}
	self->goalentity = NULL;
}

void snitch_wander(edict_t* self, float dist) {
	if (Snitch_Look(self)) {
		if (Snitch_CheckAttack(self)) {
			if (self->monsterinfo.attack_state == AS_MISSILE) {
				self->monsterinfo.attack(self);
				return;
			}
		}
	}
	
	if (!self->target_ent) {
		self->target_ent = G_Spawn();
		self->target_ent->classname = "info_notnull";
		self->target_ent->s.origin = self->s.origin;
		Snitch_PickWanderPoint(self);
	}

	if (!self->goalentity) {
		Snitch_PickWanderPoint(self);
	}
	else {
		float d = (self->goalentity->s.origin - self->s.origin).length();
		if (d < 48.0f || level.time > self->monsterinfo.pausetime) {
			Snitch_PickWanderPoint(self);
		}
	}
	if (self->goalentity) {
		M_MoveToGoal(self, dist);
	}
	else {
		//M_SetAnimation(self, &snitch_move_stand);
	}

}

void snitch_static_idle(edict_t* self, float dist) {
	Snitch_Look(self);

	if (Snitch_CheckAttack(self)) {
		if (self->monsterinfo.attack_state == AS_MISSILE) {
			self->monsterinfo.attack(self);
			return;
		}
	}

	ai_stand(self, dist);
}

void Snitch_DoScream(edict_t* self) {
	if (self->enemy) {
		Wireframe_TeleportNear(self->enemy);
		if (self->enemy->client && self->enemy->client->pers.sanity > 0) {
			self->enemy->client->pers.sanity = max(0, self->enemy->client->pers.sanity - SNITCH_SANITY_DRAIN);
		}
	}

	gi.sound(self, CHAN_VOICE, gi.soundindex("mutant/mutsght1.wav"), 1, ATTN_NORM, 0);

	self->monsterinfo.attack_finished = level.time + SNITCH_SCREAM_COOLDOWN;

	self->enemy = NULL;
	self->monsterinfo.aiflags &= ~AI_SOUND_TARGET;
}

mframe_t snitch_frames_stand[] =
{
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }
};

MMOVE_T(snitch_move_stand) = { FRAME_stand101,FRAME_stand151,snitch_frames_stand,snitch_stand };

mframe_t snitch_frames_walk[] = {
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }
};
MMOVE_T(snitch_move_walk) = { FRAME_walk05,FRAME_walk16,snitch_frames_walk,NULL };

mframe_t snitch_frames_run[] = {
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL },
	{ snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }, { snitch_static_idle, 0, NULL }
};
MMOVE_T(snitch_move_run) = { FRAME_run03, FRAME_run08, snitch_frames_run,NULL };

mframe_t snitch_frames_scream[] = {
	{ ai_charge, 0, NULL },
	{ ai_charge, 0, NULL },
	{ ai_charge, 0, Snitch_DoScream },
	{ ai_charge, 0, NULL },
	{ ai_charge, 0, NULL },
	{ ai_charge, 0, NULL },
	{ ai_charge, 0, NULL }
};
MMOVE_T(snitch_move_scream) = { FRAME_attack09,FRAME_attack15,snitch_frames_scream,snitch_run };

mframe_t snitch_frames_pain[] = {
	{ ai_move, 0, NULL }, { ai_move, 0, NULL }, { ai_move, 0, NULL },
	{ ai_move, 0, NULL }, { ai_move, 0, NULL }
};
MMOVE_T(snitch_move_pain) = { FRAME_pain101,FRAME_pain105,snitch_frames_pain,snitch_run };

mframe_t snitch_frames_death[] = {
	{ ai_move, 0, NULL }, { ai_move, 0, NULL }, { ai_move, 0, NULL },
	{ ai_move, 0, NULL }, { ai_move, 0, NULL }, { ai_move, 0, NULL },
	{ ai_move, 0, NULL }, { ai_move, 0, NULL }, { ai_move, 0, NULL }
};

MMOVE_T(snitch_move_death) = { FRAME_death101,FRAME_death109,snitch_frames_death,monster_dead };

MONSTERINFO_STAND(snitch_stand) (edict_t* self)->void {
	Snitch_Look(self);
	M_SetAnimation(self, &snitch_move_stand);
}

MONSTERINFO_WALK(snitch_walk) (edict_t* self)->void {
	M_SetAnimation(self, &snitch_move_stand);

}

MONSTERINFO_RUN(snitch_run) (edict_t* self)->void {
	M_SetAnimation(self, &snitch_move_stand);
}

MONSTERINFO_ATTACK(snitch_scream) (edict_t* self)->void {
	M_SetAnimation(self, &snitch_move_scream);
}

PAIN(snitch_pain) (edict_t* self, edict_t* other, float kick, int damage, const mod_t& mod)->void {
	if (level.time < self->pain_debounce_time) return;
	self->pain_debounce_time = level.time + 3.0_sec;
	gi.sound(self, CHAN_VOICE, gi.soundindex("mutant/mutpain1.wav"), 1, ATTN_NORM, 0);
	M_SetAnimation(self, &snitch_move_pain);
}

DIE(snitch_die) (edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point, const mod_t& mod)->void {
	gi.sound(self, CHAN_VOICE, gi.soundindex("mutant/mutdeth1.wav"), 1, ATTN_NORM, 0);
	self->deadflag = true;
	self->takedamage = false;
	self->s.effects = EF_NONE;
	M_SetAnimation(self, &snitch_move_death);
}

MONSTERINFO_CHECKATTACK(Snitch_CheckAttack) (edict_t* self)->bool {
	if (!self->enemy)
		return false;
	if (level.time < self->monsterinfo.attack_finished)
		return false;

	self->monsterinfo.attack_state = AS_MISSILE;
	return true;
}



THINK(snitch_dormant) (edict_t* self)->void {
	bool should_wake = false;

	if (self->spawnflags.has(SPAWNFLAG_SNITCH_ALWAYS_ACTIVE)) {
		should_wake = true;
	}
	else {
		for (int i = 1; i <= game.maxclients; i++) {
			edict_t* ent = &g_edicts[i];
			if (!ent->inuse || !ent->client || ent->health <= 0) continue;

			float dist = (ent->s.origin - self->s.origin).length();
			float sanity_pct = (float)ent->client->pers.sanity / (float)g_sanity_max->integer;

			if (sanity_pct <= SNITCH_SPAWN_SANITY_PCT && dist<1500.0f) {
				should_wake = true;
				break;
			}
		}
	}
	

	if (should_wake) {
		self->svflags &= ~SVF_NOCLIENT;
		self->movetype = MOVETYPE_STEP;
		self->solid = SOLID_BBOX;
		self->takedamage = true;

		self->think = snitch_active_think;
		self->nextthink = level.time + 0.1_sec;

		snitch_stand(self);
		gi.linkentity(self);

		gi.sound(self, CHAN_BODY, gi.soundindex("mutant/mutsght1.wav"), 1, ATTN_NORM, 0);
	}
	else {
		self->nextthink = level.time + 0.5_sec;
	}
}

THINK(snitch_active_think) (edict_t* self)->void {
	bool keep_active = false;

	if (self->spawnflags.has(SPAWNFLAG_SNITCH_ALWAYS_ACTIVE)) {
		keep_active = true;
	}
	else{
		for (int i = 1; i <= game.maxclients; i++) {
			edict_t* ent = &g_edicts[i];
			if (!ent->inuse || !ent->client || ent->health <= 0) continue;

			float dist = (ent->s.origin - self->s.origin).length();
			float sanity_pct = (float)ent->client->pers.sanity / (float)g_sanity_max->integer;

			if (sanity_pct <= SNITCH_SPAWN_SANITY_PCT && dist < 1500.0f) {
				keep_active = true;
				break;
			}
		}
	}

	if (!keep_active) {
		self->svflags |= SVF_NOCLIENT;
		self->solid = SOLID_NOT;
		self->takedamage = false;
		self->enemy = NULL;
		self->think = snitch_dormant;
		self->nextthink = level.time + 0.1_sec;
		gi.linkentity(self);
		return;
	}

	auto saved_think = self->think;
	self->think = monster_think;
	monster_think(self);

	if (self->think == monster_think) {
		self->think = saved_think;
	}
}

THINK(snitch_start_go) (edict_t* self)->void {
	monster_start_go(self);

	if (!self->inuse)
		return;

	self->monsterinfo.pausetime = 0_sec;

	if (self->spawnflags.has(SPAWNFLAG_SNITCH_ALWAYS_ACTIVE)) {
		self->think = snitch_active_think;
		self->nextthink = level.time + 0.1_sec;
	}
	else {
		self->movetype = MOVETYPE_NONE;
		self->svflags |= SVF_NOCLIENT;
		self->solid = SOLID_NOT;
		self->takedamage = false;
		self->think = snitch_dormant;
		self->nextthink = level.time + 0.1_sec;

		gi.linkentity(self);
	}
}

void SP_monster_snitch(edict_t* self) {
	if (deathmatch->integer) {
		G_FreeEdict(self);
		return;
	}

	self->classname = "monster_snitch";
	self->s.modelindex =gi.modelindex(SNITCH_MODEL);

	gi.modelindex("models/monsters/mutant/gibs/head.md2");
	gi.soundindex("mutant/mutpain1.wav");
	gi.soundindex("mutant/death1.wav");
	gi.soundindex("mutant/mutatck1.wav");
	gi.soundindex("mutant/idle1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->clipmask = MASK_MONSTERSOLID;

	self->mins = { -32,-32,-24 };
	self->maxs = { 32,32,32 };
	self->mass = 300;
	self->health = 100;
	self->s.scale = 1.0f;

	self->s.skinnum = 0;

	self->svflags |= SVF_MONSTER;

	self->monsterinfo.checkattack = Snitch_CheckAttack;
	self->monsterinfo.stand = snitch_stand;
	self->monsterinfo.walk = snitch_walk;
	self->monsterinfo.run = snitch_run;
	self->monsterinfo.attack = snitch_scream;
	self->monsterinfo.melee = NULL;
	self->monsterinfo.sight = NULL;
	self->pain = snitch_pain;
	self->die = snitch_die;

	self->monsterinfo.aiflags |= AI_NOSTEP;

	gi.linkentity(self);

	self->monsterinfo.active_move = &snitch_move_stand;

	self->monsterinfo.scale = MODEL_SCALE;
	self->yaw_speed = 30;

	self->s.frame = FRAME_stand101;
	monster_start(self);

	self->think = snitch_start_go;
	self->nextthink = level.time + FRAME_TIME_S;

}