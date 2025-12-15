#include "g_local.h"

#define HUMAN_MODEL				"players/male/tris.md2"
#define FRAME_stand01			0
#define FRAME_stand40			39

THINK(fake_human_think) (edict_t* self)->void {
	self->s.frame++;
	if (self->s.frame > FRAME_stand40)
		self->s.frame = FRAME_stand01;

	for (int i = 1; i <= game.maxclients; i++) {
		edict_t* ent = &g_edicts[i];
		if (!ent->inuse || !ent->client) continue;

		float dist = (ent->s.origin - self->s.origin).length();
		if (dist <= self->dmg_radius) {
			if (ent->client->pers.sanity > 0) {
				ent->client->pers.sanity = max(0, ent->client->pers.sanity - self->dmg);
			}
			if (self->count > 0) {
				T_Damage(ent, self, self, vec3_origin, ent->s.origin, vec3_origin, self->count, 0, DAMAGE_NO_PROTECTION, MOD_UNKNOWN);
			}

			gi.sound(self, CHAN_AUTO, gi.soundindex("mutant/mutatck1.wav"), 1, ATTN_NORM, 0);
			gi.sound(self, CHAN_ITEM, gi.soundindex("misc/tele1.wav"), 1, ATTN_NORM, 0);

			gi.WriteByte(svc_temp_entity);
			gi.WriteByte(TE_TELEPORT_EFFECT);
			gi.WritePosition(self->s.origin);
			gi.multicast(self->s.origin, MULTICAST_PVS,false);

			G_FreeEdict(self);
			return;
		}
	}
	self->nextthink = level.time + 0.1_sec;
}

void SP_monster_fakehuman(edict_t* self) {
	if (deathmatch->integer) {
		G_FreeEdict(self);
		return;
	}

	self->classname = "monster_fakehuman";
	self->s.modelindex = gi.modelindex(HUMAN_MODEL);

	gi.soundindex("mutant/mutatck1.wav");
	gi.soundindex("misc/tele1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->clipmask = MASK_PLAYERSOLID;

	self->mins = { -16,-16,-24 };
	self->maxs = { 16,16,32 };

	if (!self->dmg) self->dmg = 10;
	if (!self->count) self->count = 20;
	
	if (st.distance) self->dmg_radius = st.distance;
	else self->dmg_radius = 128.0f;

	self->s.skinnum = 0;
	self->s.frame = FRAME_stand01;

	gi.linkentity(self);

	self->think = fake_human_think;
	self->nextthink = level.time + 0.1_sec;
}