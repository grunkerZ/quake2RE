#include "g_local.h"
#include "m_soldier.h"

#define WIREFRAME_MODEL					"models/monsters/soldier/tris.md2"
#define WIREFRAME_SANITY_THRESH			0.40f
#define WIREFRAME_DESPAWN_DIST			1500.0f
#define WIREFRAME_BLIND_DIST			128.0f
#define WIREFRAME_HEAR_DIST				1024.0f
#define WIREFRAME_DRAIN_RATE			0.1f
#define WIREFRAME_DRAIN_AMOUNT			2

#define MOVEMENT_NOISE_THRESHOLD		300.0f

extern void FoundTarget(edict_t* self);

constexpr spawnflags_t SPAWNFLAG_WIREFRAME_ALWAYS_ACTIVE = 8_spawnflag;

static void wireframe_active(edict_t* self);
static void wireframe_dormant(edict_t* self);

void wireframe_stand(edict_t* self);
void wireframe_run(edict_t* self);
void wireframe_attack(edict_t* self);
void wireframe_melee(edict_t* self);
void wireframe_pain(edict_t* self, edict_t* other, float kick, int damage, const mod_t& mod);
void wireframe_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point, const mod_t& mod);


static bool Wireframe_CheckRealVis(edict_t* self, edict_t* other) {
	trace_t tr = gi.traceline(self->s.origin + vec3_t{ 0,0,(float)self->viewheight }, other->s.origin + vec3_t{ 0,0,(float)other->viewheight }, self, MASK_OPAQUE);

	return (tr.fraction == 1.0f || tr.ent == other);
}

static bool Wireframe_IsTargetLoud(edict_t* self, edict_t* target) {
	if (!target || !target->client)
		return false;

	float speed = (target->velocity.x * target->velocity.x) + (target->velocity.y * target->velocity.y);
	if (speed > (MOVEMENT_NOISE_THRESHOLD * MOVEMENT_NOISE_THRESHOLD))
		return true;

	edict_t* noise = NULL;
	while ((noise = G_Find(noise, [](edict_t* e) {return e->classname && !strcmp(e->classname, "player_noise"); })))
	{
		if (noise->owner == target && noise->timestamp > level.time-1.0_sec) {
			
			if ((noise->s.origin - self->s.origin).length() < WIREFRAME_HEAR_DIST) return true;
			
		}
	}
	return false;
}

static bool Wireframe_CanSense(edict_t* self, edict_t* targ) {
	float dist = (targ->s.origin - self->s.origin).length();

	if (dist <= WIREFRAME_BLIND_DIST) return visible(self, targ);

	if (Wireframe_IsTargetLoud(self, targ)) return visible(self, targ);

	return false;
}

static bool Wireframe_GetLoudPoint(edict_t* self, edict_t* target, vec3_t& out_origin) {
	if (!target || !target->client) {
		return false;
	}

	float speed = (target->velocity.x * target->velocity.x) + (target->velocity.y * target->velocity.y);
	if (speed > (MOVEMENT_NOISE_THRESHOLD * MOVEMENT_NOISE_THRESHOLD)) {
		out_origin = target->s.origin;
		return true;
	}

	edict_t* noise = NULL;
	while ((noise = G_Find(noise, [](edict_t* e) {return e->classname && !strcmp(e->classname, "player_noise"); }))){
		if (noise->owner == target && noise->timestamp > level.time - 1.0_sec) {
			float dist = (noise->s.origin - self->s.origin).length();
			if (dist < WIREFRAME_HEAR_DIST) {
				out_origin = noise->s.origin;
				return true;
			}
		}
	}
	return false;
}

static bool Wireframe_Look(edict_t* self) {
	int i;
	edict_t* ent;
	vec3_t noise_origin;

	for (i = 1; i <= game.maxclients; i++) {
		ent = &g_edicts[i];
		if (!ent->inuse || ent->health <= 0 || ent->flags & FL_NOTARGET)
			continue;

		float dist = (ent->s.origin - self->s.origin).length();
		bool is_close = (dist <= WIREFRAME_BLIND_DIST) && visible(self, ent);
		bool is_loud = Wireframe_GetLoudPoint(self, ent, noise_origin);

		if (is_close || is_loud) {
			self->enemy = ent;
			self->monsterinfo.trail_time = level.time;

			if (is_loud && !is_close)
				self->monsterinfo.last_sighting = noise_origin;
			else
				self->monsterinfo.last_sighting = ent->s.origin;
			FoundTarget(self);
			return true;
		}

	}
	return false;
}

void wireframe_idle_loop(edict_t* self, float dist) {
	Wireframe_Look(self);
}

void Wireframe_Strike(edict_t* self) {
	if (!self->enemy) return;

	float dist = (self->enemy->s.origin - self->s.origin).length();
	if (dist > 96.0f) return;

	if (self->enemy->client) {
		if(self->enemy->client->pers.sanity>0)
			self->enemy->client->pers.sanity = max(0, self->enemy->client->pers.sanity -5);
	}

	T_Damage(self->enemy, self, self, vec3_origin, self->enemy->s.origin, vec3_origin, 10, 200, DAMAGE_NO_ARMOR, MOD_HIT);

	gi.sound(self, CHAN_WEAPON, gi.soundindex("soldier/solatck2.wav"), 1, ATTN_NORM, 0);
}


mframe_t wireframe_frames_stand[] =
{
	{wireframe_idle_loop, 0, NULL}, {wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},
	{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},
	{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},
	{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},
	{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},
	{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},
	{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL},
	{wireframe_idle_loop, 0, NULL},{wireframe_idle_loop, 0, NULL}
};
MMOVE_T(wireframe_move_stand) = { FRAME_stand101,FRAME_stand130,wireframe_frames_stand,wireframe_stand };

mframe_t wireframe_frames_walk[] =
{
	{ai_walk,3,NULL},{ai_walk,6,NULL},{ai_walk,2,NULL},{ai_walk,2,NULL},
	{ai_walk,2,NULL},{ai_walk,1,NULL},{ai_walk,6,NULL},{ai_walk,10,NULL},
	{ai_walk,3,NULL},{ai_walk,0,NULL},{ai_walk,3,NULL},{ai_walk,2,NULL}
};
MMOVE_T(wireframe_move_walk) = { FRAME_walk101,FRAME_walk112,wireframe_frames_walk,NULL };

mframe_t wireframe_frames_run[] =
{
	{ai_run,10,NULL}, {ai_run,11,NULL}, {ai_run,11,NULL}, {ai_run,16,NULL},
	{ai_run,10,NULL}, {ai_run,15,NULL}
};
MMOVE_T(wireframe_move_run) = { FRAME_run01,FRAME_run06,wireframe_frames_run,NULL };

mframe_t wireframe_frames_attack[] =
{
	{ai_charge,0,NULL}, {ai_charge,0,NULL}, {ai_charge,0,NULL}, {ai_charge,0,Wireframe_Strike},
	{ai_charge,0,NULL}, {ai_charge,0,NULL}, {ai_charge,0,NULL}, {ai_charge,0,NULL}
};
MMOVE_T(wireframe_move_attack) = { FRAME_attak101,FRAME_attak108,wireframe_frames_attack,wireframe_run };

mframe_t wireframe_frames_pain1[] = {
	{ai_move, -3, NULL}, {ai_move, 4, NULL}, {ai_move, 1, NULL}, {ai_move, 1, NULL},
	{ai_move, 0, NULL}
};
MMOVE_T(wireframe_move_pain1) = { FRAME_pain101, FRAME_pain105,wireframe_frames_pain1,wireframe_run };

mframe_t wireframe_frames_death1[] = {
	{ ai_move, 0,  NULL }, { ai_move, -10, NULL }, { ai_move, -6,  NULL }, { ai_move, -8,  NULL },
	{ ai_move, -5, NULL }, { ai_move, -7,  NULL }, { ai_move, -6,  NULL }, { ai_move, -5,  NULL },
	{ ai_move, -2, NULL }, { ai_move, -4,  NULL }, { ai_move, 0,   NULL }, { ai_move, 0,   NULL },
	{ ai_move, 0,  NULL }, { ai_move, -2,  NULL }, { ai_move, 1,   NULL }, { ai_move, 0,   NULL },
	{ ai_move, -1, NULL }, { ai_move, 0,   NULL }, { ai_move, 0,   NULL }, { ai_move, 0,   NULL },
	{ ai_move, 0,  NULL }, { ai_move, 0,   NULL }
};
MMOVE_T(wireframe_move_death1) = { FRAME_death101,FRAME_death122, wireframe_frames_death1,monster_dead };

MONSTERINFO_STAND(wireframe_stand) (edict_t* self) -> void {
	M_SetAnimation(self, &wireframe_move_stand);
}

MONSTERINFO_WALK(wireframe_walk) (edict_t* self) -> void {
	M_SetAnimation(self, &wireframe_move_stand);
}

MONSTERINFO_RUN(wireframe_run) (edict_t* self) -> void {
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		M_SetAnimation(self, &wireframe_move_stand);
	else
		M_SetAnimation(self, &wireframe_move_run);
}

MONSTERINFO_ATTACK(wireframe_attack) (edict_t* self) -> void {
	M_SetAnimation(self, &wireframe_move_attack);
}

MONSTERINFO_MELEE(wireframe_melee) (edict_t* self) -> void {
	M_SetAnimation(self, &wireframe_move_attack);
}

PAIN(wireframe_pain) (edict_t* self, edict_t* other, float kick, int damage, const mod_t& mod)-> void {
	if (self->health < (self->max_health / 2))
		self->s.skinnum = 1;
	if (level.time < self->pain_debounce_time) return;

	self->pain_debounce_time = level.time + 3.0_sec;
	gi.sound(self, CHAN_VOICE, gi.soundindex("soldier/pain1.wav"), 1, ATTN_NORM, 0);

	M_SetAnimation(self, &wireframe_move_pain1);
}

DIE(wireframe_die) (edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point, const mod_t& mod)-> void {
	gi.sound(self, CHAN_VOICE, gi.soundindex("soldier/death1.wav"), 1, ATTN_NORM, 0);

	self->deadflag = true;
	self->takedamage = false;

	self->s.effects = EF_NONE;
	self->s.renderfx = RF_NONE;

	M_SetAnimation(self, &wireframe_move_death1);
}




MONSTERINFO_CHECKATTACK(Wireframe_CheckAttack) (edict_t* self) -> bool {
	if (!self->enemy)
		return false;

	float dist_to_player = (self->enemy->s.origin - self->s.origin).length();
	vec3_t noise_origin;
	bool physcially_visible = Wireframe_CheckRealVis(self, self->enemy);

	if (dist_to_player <= 128.0f && physcially_visible) {
		self->monsterinfo.last_sighting = self->enemy->s.origin;
		self->monsterinfo.trail_time = level.time;

		if (dist_to_player <= 64.0f) {
			self->monsterinfo.attack_state = AS_MELEE;
			return true;
		}

		self->monsterinfo.attack_state = AS_STRAIGHT;
		return false;
	}

	if (Wireframe_GetLoudPoint(self, self->enemy, noise_origin)) {

		float dist_to_new_sound = (noise_origin - self->monsterinfo.last_sighting).length();
		
		if (dist_to_new_sound > 32.0f || (level.time - self->monsterinfo.trail_time) > 0.5_sec) {
			self->monsterinfo.last_sighting = noise_origin;
			self->monsterinfo.trail_time = level.time;
		}
		

		self->monsterinfo.attack_state = AS_STRAIGHT;
		return false;
	}

	

	self->monsterinfo.attack_state = AS_STRAIGHT;
	return false;

}

THINK(wireframe_dormant) (edict_t* self)->void {
	bool should_wake = false;
	for (int i = 1; i <= game.maxclients; i++) {
		edict_t* ent = &g_edicts[i];
		if (!ent->inuse || !ent->client || ent->health <= 0) continue;

		float dist = (ent->s.origin - self->s.origin).length();
		float sanity_pct = (float)ent->client->pers.sanity / (float)g_sanity_max->integer;

		if (sanity_pct <= WIREFRAME_SANITY_THRESH && dist < WIREFRAME_DESPAWN_DIST) {
			should_wake = true;
			break;
		}
	}

	if (should_wake || self->spawnflags.has(SPAWNFLAG_WIREFRAME_ALWAYS_ACTIVE)) {
		self->svflags &= ~SVF_NOCLIENT;
		self->solid = SOLID_BBOX;
		self->takedamage = true;
		self->think = wireframe_active;
		self->nextthink = level.time + 0.1_sec;

		wireframe_stand(self);

		gi.linkentity(self);
	}
	else {
		self->nextthink = level.time + 0.5_sec;
	}
}

THINK(wireframe_active) (edict_t* self)->void {
	bool keep_active = false;

	if (self->spawnflags.has(SPAWNFLAG_WIREFRAME_ALWAYS_ACTIVE)) {
		keep_active = true;
	}
	else {
		for (int i = 1; i <= game.maxclients; i++) {
			edict_t* ent = &g_edicts[i];
			if (!ent->inuse || !ent->client || ent->health <= 0) continue;

			float dist = (ent->s.origin - self->s.origin).length();
			float sanity_pct = (float)ent->client->pers.sanity / (float)g_sanity_max->integer;

			if (sanity_pct <= WIREFRAME_SANITY_THRESH && dist < WIREFRAME_DESPAWN_DIST) {
				keep_active = true;
			}

			if (dist < 128.0f) {
				if (ent->client->pers.sanity > 0)
					ent->client->pers.sanity -= WIREFRAME_DRAIN_AMOUNT;
			}
		}
	}

	if (!keep_active) {
		self->svflags |= SVF_NOCLIENT;
		self->solid = SOLID_NOT;
		self->takedamage = false;
		self->enemy = NULL;
		self->think = wireframe_dormant;
		self->nextthink = level.time + 0.1_sec;
		gi.linkentity(self);
		return;
	}

	if (self->enemy) {
		if (!Wireframe_CanSense(self, self->enemy)) {
			bool timed_out = (level.time > self->monsterinfo.trail_time + 5.0_sec);

			vec3_t diff = self->monsterinfo.last_sighting - self->s.origin;
			diff.z = 0;
			bool goal_reached = (diff.length() < 48.0f);

			if (timed_out || goal_reached) {
				self->enemy = NULL;
				wireframe_stand(self);
			}
		}
	}

	bool applied_flag = false;
	if (self->enemy) {
		bool physical_vis = (Wireframe_CheckRealVis(self, self->enemy) && (self->enemy->s.origin - self->s.origin).length() <= 128.0f);

		if(!physical_vis){
			self->enemy->flags |= FL_NOVISIBLE;
			applied_flag = true;
		}
	}

	auto saved_think = self->think;
	self->think = monster_think;
	monster_think(self);

	if (self->think == monster_think) {
		self->think = saved_think;
	}

	if (applied_flag && self->enemy) {
		self->enemy->flags &= ~FL_NOVISIBLE;
	}
}

void Wireframe_TeleportNear(edict_t* target) {
	edict_t* ent = NULL;
	edict_t* best_ent = NULL;
	float best_dist = 999999.0f;
	vec3_t forward, right, offset, best_spot;
	bool found_spot = false;

	while ((ent = G_Find(ent, [](edict_t* e) {return e->inuse && e->classname && !strcmp(e->classname, "monster_wireframe"); }))) {
		if (ent->health <= 0) continue;

		float dist = (ent->s.origin - target->s.origin).length();

		if (dist < best_dist) {
			best_dist = dist;
			best_ent = ent;
		}
	}

	if(best_ent){
		
		AngleVectors(target->s.angles, forward, right, NULL);

		float distances[] = { 300.0f,200.0f,100.0f };

		for (int i = 0; i < 3; i++) {
			offset = target->s.origin - (forward * distances[i]);
			offset.z += 24;
		
			trace_t tr = gi.trace(offset, best_ent->mins, best_ent->maxs, offset, target, MASK_MONSTERSOLID);

			if (!tr.startsolid && !tr.allsolid && tr.fraction==1.0f) {
				vec3_t down = offset;
				down.z -= 256;

				trace_t floor_tr = gi.trace(offset, vec3_origin, vec3_origin, down, target, MASK_SOLID);

				if (floor_tr.fraction < 1.0f && !floor_tr.allsolid && !(floor_tr.surface && (floor_tr.surface->flags & SURF_SKY)) && floor_tr.plane.normal[2]>0.7f) {
					best_spot = floor_tr.endpos;
					best_spot.z -= best_ent->mins[2];
					best_spot.z += 1;
					found_spot = true;
					break;
				}
			}
		}

		if (found_spot) {
			gi.unlinkentity(best_ent);

			best_ent->s.origin = best_spot;
			best_ent->s.angles = target->s.angles;
			best_ent->velocity = { 0,0,0 };

			best_ent->svflags &= ~SVF_NOCLIENT;
			best_ent->solid = SOLID_BBOX;
			best_ent->takedamage = true;
			best_ent->enemy = target;

			best_ent->think = wireframe_active;
			best_ent->nextthink = level.time + 0.1_sec;

			gi.linkentity(best_ent);

			gi.sound(best_ent, CHAN_VOICE, gi.soundindex("misc/tele1.wav"), 1, ATTN_NORM, 0);
		}
		

		
		
	}
}

THINK(wireframe_start_go) (edict_t* self)->void{
	monster_start_go(self);

	self->monsterinfo.pausetime = 0_sec;

	if (self->spawnflags.has(SPAWNFLAG_WIREFRAME_ALWAYS_ACTIVE)) {
		self->think = wireframe_active;
		self->nextthink = level.time + 0.1_sec;
	}
	else {
		self->svflags |= SVF_NOCLIENT;
		self->solid = SOLID_NOT;
		self->takedamage = false;
		self->think = wireframe_dormant;
		self->nextthink = level.time + 0.1_sec;
	}
		
}

void SP_monster_wireframe(edict_t* self) {
	if (deathmatch->integer) {
		G_FreeEdict(self);
		return;
	}

	self->classname = "monster_wireframe";
	self->s.modelindex = gi.modelindex(WIREFRAME_MODEL);

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->clipmask = MASK_MONSTERSOLID;

	self->mins = { -24,-24,-24 };
	self->maxs = { 24,24,48 };
	self->mass = 100;
	self->health = 200;
	self->s.scale = 1.5f;

	self->s.effects |= EF_COLOR_SHELL;
	self->s.renderfx |= RF_SHELL_GREEN;

	self->svflags |= SVF_MONSTER;
	self->monsterinfo.checkattack = Wireframe_CheckAttack;
	self->monsterinfo.stand = wireframe_stand;
	self->monsterinfo.run = wireframe_run;
	self->monsterinfo.melee = wireframe_melee;
	self->monsterinfo.attack = wireframe_attack;
	self->pain = wireframe_pain;
	self->die = wireframe_die;

	self->monsterinfo.aiflags |= AI_NOSTEP;

	gi.linkentity(self);

	self->monsterinfo.active_move = &wireframe_move_stand;

	self->monsterinfo.scale = MODEL_SCALE;

	self->yaw_speed = 30;

	monster_start(self);

	self->think = wireframe_start_go;
	self->nextthink = level.time + FRAME_TIME_S;

}