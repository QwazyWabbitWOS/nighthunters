#pragma once
//============================================================================
// g_nh_light.h
//
// Simple light control commands.
//
// Originally coded by DingBat
//
//============================================================================


qboolean toggleStuffLight(void);
qboolean stuffLight(void);
void getUserVar(edict_t* ent, char* var);
void checkCheating(edict_t* ent, char* userinfo);
