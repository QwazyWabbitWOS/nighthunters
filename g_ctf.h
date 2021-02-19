#ifndef G_CTF_H
#define G_CTF_H

#define CTF_VERSION			1.02
#define CTF_VSTRING2(x) #x
#define CTF_VSTRING(x) CTF_VSTRING2(x)
#define CTF_STRING_VERSION  CTF_VSTRING(CTF_VERSION)

/* STAT_* 0 through 16 are defined in q_shared.h */
#define STAT_CTF_TEAM1_PIC			17
#define STAT_CTF_TEAM1_CAPS			18
#define STAT_CTF_TEAM2_PIC			19
#define STAT_CTF_TEAM2_CAPS			20
#define STAT_CTF_FLAG_PIC			21
#define STAT_CTF_JOINED_TEAM1_PIC	22
#define STAT_CTF_JOINED_TEAM2_PIC	23
#define STAT_CTF_TEAM1_HEADER		24
#define STAT_CTF_TEAM2_HEADER		25
#define STAT_CTF_TECH				26

//#define STAT_CTF_ID_VIEW			27

#define STAT_CTF_RED_FLAG_CARRIER   29
#define STAT_CTF_BLUE_FLAG_CARRIER  30

#define MAX_MENU_MAPS	14	//number of lines in mapvote menu

typedef enum ctfteam_n {
	CTF_NOTEAM,
	CTF_TEAM1,
	CTF_TEAM2
} ctfteam_t;

typedef enum ctfstate_n {
	CTF_STATE_START,
	CTF_STATE_PLAYING
} ctfstate_t;

typedef enum ctfgrapplestate_n {
	CTF_GRAPPLE_STATE_FLY,
	CTF_GRAPPLE_STATE_PULL,
	CTF_GRAPPLE_STATE_HANG
} ctfgrapplestate_t;

extern cvar_t *ctf;
extern cvar_t *dropflag_delay;
extern cvar_t *newscore;
extern cvar_t *ctf_deathscores;

#define CTF_TEAM1_SKIN "ctf_r"
#define CTF_TEAM2_SKIN "ctf_b"

#define DF_CTF_FORCEJOIN	131072	
#define DF_ARMOR_PROTECT	262144
#define DF_CTF_NO_TECH      524288

#define CTF_CAPTURE_BONUS		cap_point->value // 15	// what you get for capture
#define CTF_TEAM_BONUS			cap_team->value //10	// what your team gets for capture
#define CTF_RECOVERY_BONUS		recover_flag->value //1	// what you get for recovery
#define CTF_FLAG_BONUS			flag_bonus->value	// what you get for picking up enemy flag
#define CTF_FRAG_CARRIER_BONUS	frag_carrier->value //2	// what you get for fragging enemy flag carrier
#define CTF_FLAG_RETURN_TIME	flag_return_time->value	// seconds until auto return

#define CTF_CARRIER_DANGER_PROTECT_BONUS	carrier_save->value //2	// bonus for fraggin someone who has recently hurt your flag carrier
#define CTF_CARRIER_PROTECT_BONUS			carrier_protect->value //1	// bonus for fraggin someone while either you or your target are near your flag carrier
#define CTF_FLAG_DEFENSE_BONUS				flag_defend->value //1	// bonus for fraggin someone while either you or your target are near your flag
#define CTF_RETURN_FLAG_ASSIST_BONUS		flag_assist->value //1	// awarded for returning a flag that causes a capture to happen almost immediately
#define CTF_FRAG_CARRIER_ASSIST_BONUS		frag_carrier_assist->value //2	// award for fragging a flag carrier if a capture happens almost immediately

#define CTF_TARGET_PROTECT_RADIUS			400	// the radius around an object being defended where a target will be worth extra frags
#define CTF_ATTACKER_PROTECT_RADIUS			400	// the radius around an object being defended where an attacker will get extra frags when making kills

#define CTF_CARRIER_DANGER_PROTECT_TIMEOUT	8
#define CTF_FRAG_CARRIER_ASSIST_TIMEOUT		10
#define CTF_RETURN_FLAG_ASSIST_TIMEOUT		10

//#define CTF_AUTO_FLAG_RETURN_TIMEOUT		auto_flag_return->value//30	// number of seconds before dropped flag auto-returns

#define CTF_TECH_TIMEOUT					60  // seconds before techs spawn again

#define CTF_GRAPPLE_SPEED					grapple_speed->value // speed of grapple in flight
#define CTF_GRAPPLE_PULL_SPEED				grapple_pullspeed->value	// speed player is pulled at


void CTFChaseCam(edict_t *ent);

/**
 Count spectators
 */

void CTFSay_Team_Location(edict_t *who, char *buf);
void CTFSay_Team(edict_t *who, char *msg);


#endif	/* G_CTF_H */
