#include "g_local.h"

#define MANNEQUIN_MODEL				"models/monsters/berserk/tris.md2"
#define MANNEQUIN_MOVE_SPEED		300.0f
#define MANNEQUIN_MAX_DIST			1024.0f
#define MANNEQUIN_FOV_THRESH		0.3f
#define MANNEQUIN_SANITY_DMG		10
#define MANNEQUIN_ATTACK_DELAY		2.0_sec

#define FRAME_stand_start			0
#define FRAME_stand_end				29
#define FRAME_run_start				43
#define FRAME_run_end				48

static void mannequin_attack(edict_t* self, edict_t* other);

static bool Mannequin_IsBeingWatched(edict_t* self) {
	edict_t* player = NULL;
	for (int i = 1; i <= game.maxclients; i++) {
		edict_t* ent = &g_edicts[i];
		if (ent->inuse && ent->client && ent->health > 0) {
			player = ent;
			break;
		}
	}

	if (!player) return false;

	vec3_t player_eyes = player->s.origin + player->client->ps.viewoffset;
	vec3_t v_to_mon = self->s.origin - player_eyes;
	float dist = v_to_mon.length();

	if (dist > MANNEQUIN_MAX_DIST) return true;

	trace_t tr = gi.trace(player_eyes, vec3_origin, vec3_origin, self->s.origin, player, MASK_OPAQUE);
	if (tr.fraction < 1.0f && tr.ent != self) {
		return false;
	}

	vec3_t forward;
	AngleVectors(player->client->v_angle, forward, NULL, NULL);
	v_to_mon.normalize();

	float dot = forward.dot(v_to_mon);

	if (dot > MANNEQUIN_FOV_THRESH) {
		return true;
	}
	return false;
}

THINK(mannequin_think) (edict_t* self)->void {
	if (!self->enemy) {
		for (int i = 1; i <= game.maxclients; i++) {
			if (g_edicts[i].inuse && g_edicts[i].client) {
				self->enemy = &g_edicts[i];
				break;
			}
		}
	}

	if (!self->groundentity) {
		M_CheckGround(self, MASK_MONSTERSOLID);
	}

	self->s.frame = FRAME_stand_start;

	if (self->enemy && !Mannequin_IsBeingWatched(self) && self->enemy) {
		vec3_t dir = self->enemy->s.origin - self->s.origin;
		dir[2] = 0;

		float current_yaw = vectoyaw(dir);
		self->s.angles[YAW] = current_yaw;
		self->ideal_yaw = current_yaw;

		M_walkmove(self,current_yaw, MANNEQUIN_MOVE_SPEED * 0.1f);

		if (self->s.frame<FRAME_run_start || self->s.frame > FRAME_run_end) {
			self->s.frame = FRAME_run_start;
		}
		else {
			self->s.frame++;
			if (self->s.frame > FRAME_run_end) self->s.frame = FRAME_run_start;
		}

		float dist = dir.length();
		if (dist < 80.0f) {
			mannequin_attack(self, self->enemy);
		}
	}

	self->nextthink = level.time + 0.1_sec;
	gi.linkentity(self);
}

static void mannequin_attack(edict_t* self, edict_t* other) {
	if (!other->client) return;

	if (level.time < self->monsterinfo.attack_finished) {
		return;
	}

	self->monsterinfo.attack_finished = level.time + MANNEQUIN_ATTACK_DELAY;

	gi.sound(other, CHAN_VOICE, gi.soundindex("mutant/mutatck2.wav"), 1, ATTN_NORM, 0);

	if (other->client->pers.sanity > 0) {
		other->client->pers.sanity = max(0, other->client->pers.sanity - MANNEQUIN_SANITY_DMG);
	}

	vec3_t face_org = self->s.origin;
	face_org[2] += 24;
	vec3_t eyes_org = other->s.origin + other->client->ps.viewoffset;

	vec3_t dir = face_org - eyes_org;
	vec3_t new_angles = vectoangles(dir);

	other->client->v_angle = new_angles;
	other->client->ps.viewangles = new_angles;
	other->s.angles = new_angles;

	for (int i = 0; i < 3; i++) {
		float delta = new_angles[i] - other->client->resp.cmd_angles[i];
		if (delta > 180) delta -= 360;
		if (delta < -180) delta += 360;
		other->client->ps.pmove.delta_angles[i] = delta;
	}

	other->client->ps.pmove.delta_angles[ROLL]=0;
	other->client->resp.cmd_angles[ROLL] = 0;
	
	other->client->ps.pmove.pm_flags |= PMF_TIME_TELEPORT;
	other->client->ps.pmove.pm_time = 100;

	vec3_t push;
	AngleVectors(new_angles, push, NULL, NULL);
	push *= -1.0f;
	other->velocity = vec3_origin;

	vec3_t knockback = push * 500.0f;
	knockback[2] += 250;

	other->velocity=knockback;

	if (other->client->pers.sanity <= 0) {
		T_Damage(other, self, self, vec3_origin, other->s.origin, vec3_origin, 100000, 0, DAMAGE_NO_PROTECTION, MOD_UNKNOWN);
	}
}

MONSTERINFO_STAND(mannequin_stand) (edict_t* self)->void {}
MONSTERINFO_WALK(mannequin_walk) (edict_t* self)->void {}
MONSTERINFO_RUN(mannequin_run) (edict_t* self)->void {}

THINK(mannequin_start_go) (edict_t* self)->void {
	if (!self->inuse) return;

	self->s.scale = 1.0f;
	self->s.frame = FRAME_stand_start;

	self->s.origin[2] += 48.0f;

	vec3_t start = self->s.origin;
	vec3_t end = start;
	end[2] -= 500.0f;

	trace_t tr = gi.trace(start, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);

	if (tr.fraction < 1.0f) {
		self->s.origin = tr.endpos;
		self->s.origin[2] -= self->mins[2];
		self->s.origin[2] += 1.0f;
	}

	self->think = mannequin_think;
	self->nextthink = level.time + 0.1_sec;

	gi.linkentity(self);
	gi.Com_PrintFmt_("Mannequin active at {} {} {}\n",self->s.origin[0], self->s.origin[1], self->s.origin[2]);
}

void SP_monster_mannequin(edict_t* self) {
	if (deathmatch->integer) {
		G_FreeEdict(self);
		return;
	}

	self->classname = "monster_mannequin";
	self->s.modelindex = gi.modelindex(MANNEQUIN_MODEL);

	gi.soundindex("mutant/mutatck2.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->clipmask = MASK_MONSTERSOLID;

	self->mins = { -16,-16,-24 };
	self->maxs = { 16,16,32 };
	self->mass = 400;
	self->health = 1000;

	self->s.skinnum = 0;
	self->s.frame = FRAME_stand_start;
	self->s.scale = 1.0f;

	self->yaw_speed = 20;

	self->svflags |= SVF_MONSTER;

	self->monsterinfo.stand = mannequin_stand;
	self->monsterinfo.walk = mannequin_walk;
	self->monsterinfo.run = mannequin_run;
	self->monsterinfo.melee = NULL;
	self->monsterinfo.sight = NULL;
	self->monsterinfo.idle = NULL;

	gi.linkentity(self);

	monster_start(self);

	self->think = mannequin_start_go;
	self->nextthink = level.time + 0.5_sec;
}