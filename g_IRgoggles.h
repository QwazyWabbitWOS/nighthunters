#pragma once
//===========================================================================
// g_IRgoggles.h
//
// Various methods. 
// Some oriented around flashlight code.
// Originally coded by DingBat
// Based on a tutorial developed by Decker.
//
// Copyright (c), 1999 The BatCave. All Rights Reserved.
//===========================================================================

#define IR_R 0
#define IR_G 0.5f
#define IR_B 0
#define IR_A 0.08f
#define IR_MARINE_FOV_DEFAULT "75"
#define IR_EFFECT_TIME_DEFAULT "30"

void Use_IRgoggles(edict_t* ent, gitem_t* item);

void enable_IRgoggles(void);
void enable_Quad(void);

void G_SetIREffects(edict_t* ent);

void deadDropIRgoggles(edict_t* self);

void validateIRMarineFOV(void);
int getIRMarineFOV(void);
void validateIREffectTime(void);
int getIREffectTime(void);

// Temporary hack for IR goggle drop.
// We need to know whether the player was carring the quad or the invuln
// Once we get a proper model for IR goggles, it won't matter.
extern gitem_t *IR_type_dropped ;

int getIRMarineFOV(void);
int getIREffectTime(void);
