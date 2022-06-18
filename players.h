
#ifndef _PLAYERS_
#define _PLAYERS_

#include "client.h"
#include "time.h"
#include "stdafx.h"
#include "Engine.h"
#include "weapon.h"
#include <string.h>
#include <vector>

struct entity_s
{
	vec3_t vOrigin;
	char szName[32];
	int iImportant;
	
	DWORD timestamp;

	int id;
};
extern std::vector<entity_s> mySounds;

struct spread_info
{
	unsigned int random_seed;
	int recoil;
	float gtime;
	float prevtime;
	float brokentime; 

	float spreadvar;
	float recoiltime;
	bool firing;
	int WeaponState;
	int prcflags;
};

struct sMe
{
	int iClip;
	int iHealth;
	int iArmor;
	int iKills;
	int iDeaths;
	int iHs;
	int iMoney;
	int iWeaponID;

	float flGroundAngle;
	int iFOV;
	float pmVelocity[3];
	float punchangle[3];
	float pmEyePos[3];
	int pmFlags;
	int iIndex;
	float viewAngles[3];
	float sin_yaw, minus_cos_yaw;
	bool inZoomMode;
	int pmMoveType;
	int team;
	struct cl_entity_s * ent;
	bool alive;
	spread_info spread;
	float pmMaxSpeed;
	float pmAirAccelerate;
	float pmGroundSpeed;
	float m_flNextPrimaryAttack;
	bool bBadWeapon;
	float fOnLadder;
	float flHeight;
	int iWaterLevel;
	int iUseHull;
	int iMoveType;
	void DoBunnyHop(struct usercmd_s *usercmd);

	vec3_t vViewAngles;
	vec3_t vForward;
	vec3_t vRight;
	vec3_t vUp;
	vec3_t vEye;
	vec3_t vOrigin;
	vec3_t vViewOrg;
	net_status_s Status;
	Weapon_List *weapon;
};

enum 
{ 
     SEQUENCE_IDLE     = 0, 
     SEQUENCE_SHOOT    = 1, 
     SEQUENCE_RELOAD   = 2, 
     SEQUENCE_DIE      = 4, 
     SEQUENCE_THROW    = 8, 
     SEQUENCE_ARM_C4   = 16, 
     SEQUENCE_SHIELD   = 32,
	 SEQUENCE_SHIELD_SIDE = 64
};


class PlayerInfo 
{
protected:
	friend class VecPlayers;
	void init( int _entindex) 
	{ 
		team=2;
		entinfo.name="\\missing-name\\";
		entinfo.model="missing-model";
		alive=false;
		entindex = _entindex;
		distance = 1000.0;
		visible  = 0;
		strcpy(m_weapon,"N-A");
		m_updated = false;
		bGotHead = false;
		fixHbAim = false;
		canAim = false;
		bDrawn = false;
	}
public:
	hud_player_info_t entinfo;
	void updateEntInfo()
	{
		pfnGetPlayerInfo(entindex, &entinfo);
		if(!entinfo.name ) { entinfo.name  = "\\missing-name\\"; }
		if(!entinfo.model) { entinfo.model = "unknown model";    } 
	}
	const char* getName() const { return entinfo.name; }

	int    team;
	int    iInfo;
	float  distance;
	bool    visible;
	float  fovangle;   
	vec3_t vHitbox;
	vec3_t vOrigin;
	bool bGotHead;
	bool fixHbAim;
	int canAim;
	bool bDrawn;
	bool bUpdated;
	float fFov;
	float fDistance;
	bool bVisible;
    bool bDucked;


	bool hasbomb;
	bool vip;

	void setAlive() { alive = true;  }
	void setDead() { alive = false;  }
	bool isAlive () { return alive!=false; }

	struct cl_entity_s * getEnt() { return GetEntityByIndex(entindex); }

	enum   { UPDATE_MISSING=0, UPDATE_ADDENT=1, UPDATE_SOUND_RADAR=2};

	void updateSoundRadar(void) { m_lastUpdateType=UPDATE_SOUND_RADAR; }
	const char* getWeapon() { return m_weapon; }
	void setWeapon(const char* newname)
	{
		register int len = strlen(newname);
		if(len && len<30) { strcpy(m_weapon,newname); }
	}

	void   updateClear    ()
	{ 
		bDrawn = false;
		m_lastUpdateType=UPDATE_MISSING; 
	}
	void SetOrigin(const float* neworg)
	{ VectorCopy(neworg,m_origin); }
	void   updateAddEntity (const float* neworg)
	{ 
		if(alive)
		{
			m_lastUpdateType=UPDATE_ADDENT; 
			m_lastUpdateTime=ClientTime::current; 
			VectorCopy(neworg,m_origin); 
		}
	}

	bool   isUpdated       ()    { return m_lastUpdateType!=UPDATE_MISSING; }
	bool   isUpdatedAddEnt ()    { return m_lastUpdateType==UPDATE_ADDENT;  }
	int    updateType()          { return m_lastUpdateType; }
	const float * origin()       { return m_origin; }
	float  timeSinceLastUpdate() { return (float)(ClientTime::current-m_lastUpdateTime); }

private:
	int     m_lastUpdateType;
	double  m_lastUpdateTime;
	float   m_origin[3];
	char    m_weapon[32];
	bool    m_updated;

	bool alive;
	int entindex;

public:
	PlayerInfo() { init(0); }
};

enum{  MAX_VPLAYERS =36 };
class VecPlayers
{
private:
	PlayerInfo* players;
public:
	~VecPlayers() { delete[] players; }
	VecPlayers() 
	{ 
		players = new PlayerInfo[MAX_VPLAYERS];
		for(int i=0;i<MAX_VPLAYERS;i++) { players[i].init(i); }
	}

	inline PlayerInfo& operator [] (unsigned int i)
	{
		if(i>=MAX_VPLAYERS) {return players[0];}
		else                {return players[i];}
	}
	
	inline unsigned int size() { return MAX_VPLAYERS; }
};
extern VecPlayers g_Player;
extern sMe g_Local;

#define M_PI 3.14159265358979323846
#define POW(x) ((x)*(x))
#define VectorTransform(a,b,c){(c)[0]=DotProduct((a),(b)[0])+(b)[0][3];(c)[1]=DotProduct((a),(b)[1])+(b)[1][3];(c)[2]=DotProduct((a),(b)[2])+(b)[2][3];}

#define VectorLengthSquared(v) ((v)[0]*(v)[0]+(v)[1]*(v)[1]+(v)[2]*(v)[2])
#define VectorDistance(a,b) sqrt(POW((a)[0]-(b)[0])+POW((a)[1]-(b)[1])+POW((a)[2]-(b)[2]))
#define VectorLength(a) sqrt(POW((a)[0])+POW((a)[1])+POW((a)[2]))
#define VectorAdd(a,b,c) {(c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];(c)[2]=(a)[2]+(b)[2];}
#define VectorMul(vec,num,res){(res)[0]=(vec)[0]*(num);(res)[1]=(vec)[1]*(num);(res)[2]=(vec)[2]*(num);}
#define VectorSubtract(a,b,c) {(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];}
#define Square(a) ((a)*(a))
#endif