
// p_motd.c
//
// Night Hunters menu
//
// Copyright (c), 1999 The BatCave. All Rights Reserved.
//============================================================================

// CTF-like menu for NH
//QW// Refactored to prevent buffer overflow. March 2021.

#include "g_local.h"

void ShowMOTD(edict_t* ent)
{
	int c;
	char line[80] = { 0 };
	char entry[120] = { 0 };	// a line with the xv yv etc..
	char string[MAX_MSGLEN] = { 0 };	// contains what is sent to WriteString
	char motdPath[MAX_QPATH];

	int		i = 0;	// for going through each motd.txt line - counter
	int		y = 16;	// for incrementing the y coord on screen
	size_t	j = 0;	// temp for creating string
	size_t	stringlength = 0;	// temp for contatenating entry's into string

	if (!ent || !ent->inuse)
		return;

	// Generate the path to the MOTD file.
	Com_sprintf(motdPath, sizeof motdPath, "%s/%s", game_dir->string, motdfile->string);

	FILE* motd_file;
	motd_file = fopen(motdPath, "rt");

	if (!motd_file) {
		gi.dprintf("Unable to open MOTD file at %s.\n", motdPath);
		return;
	}
	else
	{
		size_t motdBytes = 0;
		while ((c = fgetc(motd_file)) != EOF)
			++motdBytes;

		// we are constrained by the game's max message size.
		if (motdBytes >= MAX_MSGLEN - 300) { // 300 byte margin
			gi.error("MoTD file too large! %u bytes\n", motdBytes);
			return;
		}

		fseek(motd_file, 0, SEEK_SET);
		while ((fgets(line, 80, motd_file)) && (i < 20)) // limit is 20 lines
		{
			// add each new line to motd, to create a BIG message entry.
			Com_sprintf(entry, sizeof(entry), "xv 0 yv %i string \"%s\" ", y, line);
			j = strlen(entry);
			Q_strncpyz(string + stringlength, sizeof string - 1, entry);
			stringlength += j;
			i++;
			y += 8;
		}

		fclose(motd_file);
		if (strlen(string) >= MAX_MSGLEN - 20) { //QW// do not exceed 1380
			gi.dprintf("Length of MoTD string too long! %u bytes\n", strlen(string));
		}

		// Added by bruce
		ent->ShowMOTD = (int)(level.time + getMotdTime());

		gi.WriteByte(svc_layout);
		gi.WriteString(string);
		gi.unicast(ent, true);
	}
}

qboolean
ShowingMOTD(edict_t* ent) {

	if (ent->ShowMOTD > (int)level.time)
		return true;

	return false;
}

void
ClearMOTD(edict_t* ent) {
	ent->ShowMOTD = (int)(level.time);
}


void validateMotdTime() {

	// motd_time must be between 0 and 90 secs
	if ((motd_time->value < 0) ||
		(motd_time->value > 90))
		gi.cvar_set("motd_time", MOTD_TIME_DEFAULT);
}

int getMotdTime() {

	// Check if the cvar has changed.
	if (motd_time->modified)
		validateMotdTime();

	return motd_time->value;
}
