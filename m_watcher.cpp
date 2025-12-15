#include "g_local.h"

#define WATCHER_MODEL				"models/monsters/insane/tris.md2"
#define WATCHER_SANITY_THRESH		0.80f
#define WATCHER_DESPAWN_DIST		1024.0f
#define WATCHER_VIEW_CONE			0.866f
#define WATCHER_DRAIN_RATE			0.1f
#define WATCHER_DRAIN_AMOUNT		1

constexpr spawnflags_t SPAWNFLAG_WATCHER_ALWAYS_ACTIVE = 8_spawnflag;

void watcher_active(edict_t* self);

THINK(watcher_dormant) (edict_t* self)->void {
	
	bool should_wake = false;

	for (int i = 1; i <= game.maxclients; i++) {

		edict_t* ent = &g_edicts[i];

		if (!ent->inuse || !ent->client || ent->health <= 0)
			continue;

		float dist = (ent->s.origin - self->s.origin).length();
		float sanity_pct = (float)ent->client->pers.sanity / (float)g_sanity_max->integer;

		if (sanity_pct < +WATCHER_SANITY_THRESH && dist < WATCHER_DESPAWN_DIST) {
			should_wake = true;
			break;
		}
	}

	if (should_wake) {
		self->svflags &= ~SVF_NOCLIENT;
		self->solid = SOLID_BBOX;
		self->think = watcher_active;
		self->nextthink = level.time + 0.1_sec;
		gi.linkentity(self);
	}
	else {
		self->nextthink = level.time + 0.5_sec;
	}
}

THINK(watcher_active) (edict_t* self) ->void {
	bool keep_active = false;

	if (self->spawnflags.has(SPAWNFLAG_WATCHER_ALWAYS_ACTIVE)) {
		keep_active = true;
	}

	for (int i = 1; i <= game.maxclients; i++) {
		edict_t* ent = &g_edicts[i];

		if (!ent->inuse || !ent->client || ent->health <= 0)
			continue;

		float dist = (ent->s.origin - self->s.origin).length();
		float sanity_pct = (float)ent->client->pers.sanity / (float)g_sanity_max->integer;

		if (!keep_active) {
			if (sanity_pct <= WATCHER_SANITY_THRESH && dist < WATCHER_DESPAWN_DIST) {
				keep_active = true;
			}
		}

		if (dist < WATCHER_DESPAWN_DIST && visible(self, ent)) {
			vec3_t forward;
			vec3_t to_watcher = self->s.origin - ent->s.origin;
			to_watcher.normalize();

			AngleVectors(ent->client->v_angle, forward, nullptr, nullptr);

			if (forward.dot(to_watcher) > WATCHER_VIEW_CONE) {
				trace_t tr = gi.trace(ent->s.origin + ent->client->ps.viewoffset, vec3_origin, vec3_origin, self->s.origin, ent, MASK_OPAQUE);
				
				if (tr.fraction == 1.0f || tr.ent == self) {
					if (ent->client->pers.sanity > 0) {
						ent->client->pers.sanity -= WATCHER_DRAIN_AMOUNT;
					}
				}
			}
		}
	}

	if (!keep_active) {
		self->svflags |= SVF_NOCLIENT;
		self->solid = SOLID_NOT;
		self->think = watcher_dormant;
		self->nextthink = level.time + 0.1_sec;
		gi.linkentity(self);
		return;
	}

	self->nextthink = level.time + 0.1_sec;
}

void SP_monster_watcher(edict_t* self) {
	if (deathmatch->integer) {
		G_FreeEdict(self);
		return;
	}

	self->classname = "monster_watcher";
	self->s.modelindex = gi.modelindex(WATCHER_MODEL);

	self->movetype = MOVETYPE_NONE;
	self->solid = SOLID_BBOX;
	self->clipmask = MASK_MONSTERSOLID;

	self->mins = { -16,-16,-24 };
	self->maxs = { 16,16,32 };

	self->svflags |= SVF_MONSTER;
	self->takedamage = false;
	self->health = 100;

	self->s.frame = 0;

	if (self->spawnflags.has(SPAWNFLAG_WATCHER_ALWAYS_ACTIVE)) {
		self->think = watcher_active;
		self->nextthink = level.time + 1.0_sec;
	}
	else {
		self->svflags |= SVF_NOCLIENT;
		self->solid = SOLID_NOT;
		self->think = watcher_dormant;
		self->nextthink = level.time + 1.0_sec;
	}

	gi.linkentity(self);
}