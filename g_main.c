
#include "g_local.h"
#include "g_nh_light.h"

game_locals_t	game;
level_locals_t	level;
game_import_t	gi;
game_export_t	globals;
spawn_temp_t	st;

int	sm_meat_index;
int	snd_fry;
int meansOfDeath;

edict_t* g_edicts;

cvar_t* deathmatch;
cvar_t* coop;
cvar_t* dmflags;
cvar_t* skill;
cvar_t* fraglimit;
cvar_t* timelimit;
cvar_t* password;
cvar_t* spectator_password;
cvar_t* needpass;
cvar_t* maxclients;
cvar_t* maxspectators;
cvar_t* maxentities;
cvar_t* g_select_empty;
cvar_t* dedicated;
cvar_t* gamedebug;

cvar_t* filterban;

cvar_t* sv_maxvelocity;
cvar_t* sv_gravity;

cvar_t* sv_rollspeed;
cvar_t* sv_rollangle;
cvar_t* gun_x;
cvar_t* gun_y;
cvar_t* gun_z;

cvar_t* run_pitch;
cvar_t* run_roll;
cvar_t* bob_up;
cvar_t* bob_pitch;
cvar_t* bob_roll;

cvar_t* sv_cheats;

cvar_t* flood_msgs;
cvar_t* flood_persecond;
cvar_t* flood_waitdelay;

cvar_t* sv_maplist;
cvar_t* sv_maplist2; // ***** NH Change *****
cvar_t* sv_maplist3; // ***** NH Change *****
cvar_t* sv_maplist_small_max; // ***** NH Change *****
cvar_t* sv_maplist_medium_max; // ***** NH Change *****

// ***** Start of NH changes *****

cvar_t* maxrate;
cvar_t* maxmarinekill;
cvar_t* penalty_threshold;
cvar_t* maxtime;
cvar_t* minscore;

// flares
cvar_t* max_flares;
cvar_t* flare_bright_time;
cvar_t* flare_dim_time;
cvar_t* flare_die_time;
cvar_t* flare_health;
cvar_t* flare_damage;
cvar_t* flare_damage_radius;
// End of flare cvars.

// teleport
cvar_t* max_teleport_shots;
cvar_t* teleport_health;
cvar_t* teleport_panic_time; // Teleporter panic timer.
// End of teleport cvars.

// IR goggles
cvar_t* IR_marine_fov;
cvar_t* IR_effect_time;
// End of IR goggles.

// NH scoreboard.
cvar_t* use_NH_scoreboard;
// End of NH scoreboard.

// Marine initial weapon
cvar_t* init_marine_weapon;
// End of Marine initial weapon

// Allow Predator weapon overload
cvar_t* enable_predator_overload;
cvar_t* predator_overload_cost;
// End of Predator weapon overload

// Safety mode
cvar_t* marine_safety_time;
cvar_t* predator_safety_time;
cvar_t* enable_marine_safety;
cvar_t* enable_predator_safety;

// End of safety mode

// Predator model/skin
cvar_t* predator_model;
cvar_t* predator_skin;
cvar_t* marine_allow_custom;
cvar_t* marine_model;
cvar_t* marine_skin;
// End of predator model/skin vars

// MOTD hang time
cvar_t* motd_time;

// Predator inventory stuff
cvar_t* predator_max_rockets;
cvar_t* predator_max_slugs;
cvar_t* predator_start_rockets;
cvar_t* predator_start_slugs;
cvar_t* predator_start_health;

// Sound and light show.
cvar_t* enable_light_show;
cvar_t* light_show_interval;

// ***** End of NH changes *****

void SpawnEntities(char* mapname, char* entities, char* spawnpoint);
void ClientThink(edict_t* ent, usercmd_t* cmd);
qboolean ClientConnect(edict_t* ent, char* userinfo);
void ClientUserinfoChanged(edict_t* ent, char* userinfo);
void ClientDisconnect(edict_t* ent);
void ClientBegin(edict_t* ent);
void ClientCommand(edict_t* ent);
void WriteGame(char* filename, qboolean autosave);
void ReadGame(char* filename);
void WriteLevel(char* filename);
void ReadLevel(char* filename);
void InitGame(void);
void G_RunFrame(void);


//===================================================================


void ShutdownGame(void)
{
	gi.dprintf("==== ShutdownGame ====\n");

	gi.FreeTags(TAG_LEVEL);
	gi.FreeTags(TAG_GAME);

#ifdef _WIN32
	OutputDebugString("ShutdownGame() was called.\n");
	_CrtMemDumpAllObjectsSince(&startup1);
	_CrtMemDumpStatistics(&startup1);
	OutputDebugString("Leak dump begin.\n");
	_CrtDumpMemoryLeaks();
	OutputDebugString("Leak dump end.\n");
#endif

}


/*
=================
GetGameAPI

Returns a pointer to the structure with all entry points
and global variables
=================
*/
game_export_t* GetGameAPI(game_import_t* import)
{
	gi = *import;

	globals.apiversion = GAME_API_VERSION;
	globals.Init = InitGame;
	globals.Shutdown = ShutdownGame;
	globals.SpawnEntities = SpawnEntities;

	globals.WriteGame = WriteGame;
	globals.ReadGame = ReadGame;
	globals.WriteLevel = WriteLevel;
	globals.ReadLevel = ReadLevel;

	globals.ClientThink = ClientThink;
	globals.ClientConnect = ClientConnect;
	globals.ClientUserinfoChanged = ClientUserinfoChanged;
	globals.ClientDisconnect = ClientDisconnect;
	globals.ClientBegin = ClientBegin;
	globals.ClientCommand = ClientCommand;

	globals.RunFrame = G_RunFrame;

	globals.ServerCommand = ServerCommand;

	globals.edict_size = sizeof(edict_t);

	return &globals;
}

#ifndef GAME_HARD_LINKED
// this is only here so the functions in q_shared.c and q_shwin.c can link
void Sys_Error(char* error, ...)
{
	va_list		argptr;
	char		text[1024];

	va_start(argptr, error);
	vsprintf(text, error, argptr);
	va_end(argptr);

	gi.error(ERR_FATAL, "%s", text);
}

void Com_Printf(char* msg, ...)
{
	va_list		argptr;
	char		text[1024];

	va_start(argptr, msg);
	vsprintf(text, msg, argptr);
	va_end(argptr);

	gi.dprintf("%s", text);
}

#endif

//======================================================================


/*
=================
ClientEndServerFrames
=================
*/
void ClientEndServerFrames(void)
{
	int		i;
	edict_t* ent;

	// calc the player views now that all pushing
	// and damage has been added
	for (i = 0; i < maxclients->value; i++)
	{
		ent = g_edicts + 1 + i;
		if (!ent->inuse || !ent->client)
			continue;
		ClientEndServerFrame(ent);
	}

}

/*
=================
CreateTargetChangeLevel

Returns the created target changelevel
=================
*/
edict_t* CreateTargetChangeLevel(char* map)
{
	edict_t* ent;

	ent = G_Spawn();
	ent->classname = "target_changelevel";
	Com_sprintf(level.nextmap, sizeof(level.nextmap), "%s", map);
	ent->map = level.nextmap;
	return ent;
}

/*
=================
EndDMLevel

The timelimit or fraglimit has been exceeded
=================
*/
void EndDMLevel(void)
{
	edict_t* ent;
	int count = 0;
	int lastsize = 1; // ***** NH Change *****
	char* s = NULL;
	char* t = NULL;
	char* f;
	char* temp = NULL;
	char* tmp = NULL;
	char lastmap[64] = { 0 };
	static const char* seps = " ,\n\r";

	// Toggle to different intermission beat wav file
	if (last_beat)
		last_beat = 0;
	else
		last_beat = 1;

	// stay on same level flag
	if ((int)dmflags->value & DF_SAME_LEVEL) {
		BeginIntermission(CreateTargetChangeLevel(level.mapname));
		return;
	}

	count = CountConnectedClients();

	// see if it's in the map list

	// This check is redundant now.
	if (*sv_maplist->string) {
		if (!*sv_maplist2->string)
			sv_maplist_small_max->value = 999;
		if (!*sv_maplist3->string)
			sv_maplist_medium_max->value = 999;


		// ***** Start of NH Changes *****
		// This really should be done elsewhere.
		if (strlen(maplist_lastmap) == 0 && strlen(sv_maplist->string)) {
			temp = strdup(sv_maplist->string);
			if (temp)
				tmp = temp;
			strcpy(maplist_lastmap, strsep(&temp, seps));
			while (temp != NULL)
				strcpy(maplist_lastmap, strsep(&temp, seps));
			free(tmp);
		}

		if (strlen(maplist2_lastmap) == 0 && strlen(sv_maplist2->string)) {
			temp = strdup(sv_maplist2->string);
			if (temp)
				tmp = temp;
			strcpy(maplist2_lastmap, strsep(&temp, seps));
			while (temp != NULL)
				strcpy(maplist2_lastmap, strsep(&temp, seps));
			free(tmp);
		}

		if (strlen(maplist3_lastmap) == 0 && strlen(sv_maplist2->string)) {
			temp = strdup(sv_maplist3->string);
			if (temp)
				tmp = temp;
			strcpy(maplist3_lastmap, strsep(&temp, seps));
			while (temp != NULL)
				strcpy(maplist3_lastmap, strsep(&temp, seps));
			free(tmp);
		}


		if ((count <= getMaplistSmallMax()) && (*sv_maplist->string)) {
			s = strdup(sv_maplist->string);
			Q_strncpyz(lastmap, sizeof lastmap, maplist_lastmap);
			lastsize = 1;
		}

		if (((count > getMaplistSmallMax()) &&
			(count <= getMaplistMediumMax())) &&
			*sv_maplist2->string) {
			s = strdup(sv_maplist2->string);
			Q_strncpyz(lastmap, sizeof lastmap, maplist2_lastmap);
			lastsize = 2;
		}

		if ((count > getMaplistMediumMax()) &&
			*sv_maplist3->string) {
			s = strdup(sv_maplist3->string);
			Q_strncpyz(lastmap, sizeof lastmap, maplist3_lastmap);
			lastsize = 3;
		}

		// ***** End of NH Changes *****
		f = NULL;
		t = strtok(s, seps);
		while (t != NULL) {

			if (((strlen(lastmap) != 0) &&
				(Q_stricmp(t, lastmap) == 0)) ||
				(Q_stricmp(t, level.mapname) == 0)) {

				// Found last map in list.
				// Go on to next one.
				t = strtok(NULL, seps);

				// Check if end of list.
				if (t == NULL) {

					// End of list.
					// Go to first level on list.
					if (f == NULL) {
						BeginIntermission(CreateTargetChangeLevel(level.mapname));
						Q_strncpyz(lastmap, sizeof lastmap, level.mapname);
					}
					else {
						BeginIntermission(CreateTargetChangeLevel(f));
						Q_strncpyz(lastmap, sizeof lastmap, f);
					}
				}
				else {
					BeginIntermission(CreateTargetChangeLevel(t));
					Q_strncpyz(lastmap, sizeof lastmap, t);
				}

				// ***** Start of NH Changes *****
				if (lastsize == 1)
					Q_strncpyz(maplist_lastmap, sizeof maplist_lastmap, lastmap);
				else if (lastsize == 2)
					Q_strncpyz(maplist2_lastmap, sizeof maplist2_lastmap, lastmap);
				else if (lastsize == 3)
					Q_strncpyz(maplist3_lastmap, sizeof maplist3_lastmap, lastmap);

				free(s);
				return;
			}
			if (!f) {
				f = t;
			}
			t = strtok(NULL, seps);
		}
		free(s);
	}

	if (level.nextmap[0]) // go to a specific map
		BeginIntermission(CreateTargetChangeLevel(level.nextmap));
	else {	// search for a changelevel
		ent = G_Find(NULL, FOFS(classname), "target_changelevel");

		if (!ent) {	// the map designer didn't include a changelevel,
		  // so create a fake ent that goes back to the same level
			BeginIntermission(CreateTargetChangeLevel(level.mapname));
			return;
		}
		BeginIntermission(ent);
	}
}


/*
=================
CheckNeedPass
=================
*/
void CheckNeedPass(void)
{
	int need;

	// if password or spectator_password has changed, update needpass
	// as needed
	if (password->modified || spectator_password->modified)
	{
		password->modified = spectator_password->modified = false;

		need = 0;

		if (*password->string && Q_stricmp(password->string, "none"))
			need |= 1;
		if (*spectator_password->string && Q_stricmp(spectator_password->string, "none"))
			need |= 2;

		gi.cvar_set("needpass", va("%d", need));
	}
}

/*
=================
CheckDMRules
=================
*/
void CheckDMRules(void)
{
	int			i;
	gclient_t* cl;

	if (level.intermissiontime)
		return;

	if (!deathmatch->value)
		return;

	if (timelimit->value)
	{
		if (level.time >= timelimit->value * 60)
		{
			gi.bprintf(PRINT_HIGH, "Timelimit hit.\n");
			EndDMLevel();
			return;
		}
	}

	if (fraglimit->value)
	{
		for (i = 0; i < maxclients->value; i++)
		{
			cl = game.clients + i;
			if (!g_edicts[i + 1].inuse)
				continue;

			if (cl->resp.score >= fraglimit->value)
			{
				gi.bprintf(PRINT_HIGH, "Fraglimit hit.\n");
				EndDMLevel();
				return;
			}
		}
	}
}


/*
=============
ExitLevel
=============
*/
void ExitLevel(void)
{
	int		i;
	edict_t* ent;
	char	command[256];

	Com_sprintf(command, sizeof(command), "gamemap \"%s\"\n", level.changemap);
	gi.AddCommandString(command);
	level.changemap = NULL;
	level.exitintermission = 0;
	level.intermissiontime = 0;
	ClientEndServerFrames();

	// clear some things before going to next level
	for (i = 0; i < maxclients->value; i++)
	{
		ent = g_edicts + 1 + i;
		if (!ent->inuse)
			continue;
		if (ent->health > ent->client->pers.max_health)
			ent->health = ent->client->pers.max_health;
	}

}

/*
================
G_RunFrame

Advances the world by 0.1 seconds
================
*/
void G_RunFrame(void)
{
	int		i;
	edict_t* ent;

	level.framenum++;
	level.time = level.framenum * FRAMETIME;

	// choose a client for monsters to target this frame
	AI_SetSightClient();

	// exit intermissions

	if (level.exitintermission)
	{
		ExitLevel();
		return;
	}

	//
	// treat each object in turn
	// even the world gets a chance to think
	//
	ent = &g_edicts[0];
	for (i = 0; i < globals.num_edicts; i++, ent++)
	{
		if (!ent->inuse)
			continue;

		level.current_entity = ent;

		VectorCopy(ent->s.origin, ent->s.old_origin);

		// if the ground entity moved, make sure we are still on it
		if ((ent->groundentity) && (ent->groundentity->linkcount != ent->groundentity_linkcount))
		{
			ent->groundentity = NULL;
			if (!(ent->flags & (FL_SWIM | FL_FLY)) && (ent->svflags & SVF_MONSTER))
			{
				M_CheckGround(ent);
			}
		}

		if (i > 0 && i <= maxclients->value)
		{
			ClientBeginServerFrame(ent);
			continue;
		}

		G_RunEntity(ent);
	}

	// see if it is time to end a deathmatch
	CheckDMRules();

	// see if needpass needs updated
	CheckNeedPass();

	// build the playerstate_t structures for all players
	ClientEndServerFrames();

	// ***** Start of NH changes *****

	if (level.time >= level.pred_check_restart)
	{

		// New predator switching.
		lookForPredator(NULL);

		if (stuffLight()) 
		{
			for (i = 0; i <= maxclients->value; i++)
			{
				ent = &g_edicts[i];
				if (ent->inuse && ent->client) {
					// only if using software mode check for drawflat
					stuffcmd(ent, "set gl_dynamic 1;set sw_drawflat 0\n");
					// gi.cvar_forceset ("gl_dynamic", "1");
				}
			}
		}

		level.pred_check_restart = level.time + 5;
	}

	// Start thunder and lightning.
	// I am the great and powerful Oz!
	if ((enable_light_show->value) &&
		(!level.lightningOn) &&
		(level.nextLightning < (int)level.time))

		// Calling this sets timer for next lightning event.
		Lightning_Off();

	// ***** End of NH changes *****	


}
