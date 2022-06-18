#include <windows.h>
#include <mmsystem.h>
#include <memory.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
#include "gateways.h"
#include "parsemsg.h"
#include "client.h"
#include "weapon.h"
#include "players.h"
#include "xorstr.h"
#include "cvar.h"
#include "apihook.h"
#include "Engine.h"
#include "stdafx.h"
#define IS_DEAD (1<<0)
#define HAS_BOMB (1<<1)
#define IS_VIP (1<<2)
void AtRoundStart(void);
int modId;
int RoundsPlayed = 0;
bool bMsgHook = false;
#pragma warning(disable:4786)
#pragma warning(disable:4800)
#pragma warning(disable:4244)
#pragma warning(disable:4101)
#pragma warning(disable:4715)
#pragma warning(disable:4305)

void ConPrint(LPSTR form, ...)
{
	static char tmp[2048];
	va_list p;
	va_start(p, form);
	vsprintf(tmp, form, p);
	OutputDebugStringA(tmp);
	va_end(p);
}

pfnUserMsgHook TeamInfoOrg = NULL;
pfnUserMsgHook SetFOVOrg = NULL;
pfnUserMsgHook CurWeaponOrg = NULL;
pfnUserMsgHook ScoreAttribOrg = NULL;
pfnUserMsgHook HealthOrg = NULL;
pfnUserMsgHook BatteryOrg = NULL;
pfnUserMsgHook ScoreInfoOrg = NULL;
pfnUserMsgHook DeathMsgOrg = NULL;
pfnUserMsgHook SayTextOrg = NULL;
pfnUserMsgHook ResetHUDOrg = NULL;
pfnUserMsgHook TextMsgOrg = NULL;
pfnUserMsgHook DamageOrg = NULL;
pfnUserMsgHook AmmoXOrg = NULL;
pfnUserMsgHook WeaponListOrg = NULL;
pfnUserMsgHook MoneyOrg = NULL;
pfnUserMsgHook ServerNameOrg = NULL;
pfnUserMsgHook IAC_QueryOrg = NULL;
pfnUserMsgHook IAC_HIQueryOrg = NULL;
pfnUserMsgHook RadarOrg = NULL;
pfnUserMsgHook StatusValueOrg = NULL;
pfnUserMsgHook BrassOrg = NULL;

pfnUserMsgHook PTeamOrg = NULL;
float fCurrentFOV;
//==================================================================================
static int TeamInfo(const char *pszName, int iSize, void *pbuf)
{
	ConPrint("TeamInfo=%s", pbuf);
	BEGIN_READ(pbuf, iSize);
	int px = READ_BYTE();
	char * teamtext = READ_STRING();
	char STR_TERROR[9];
	char STR_CT[2];
	char STR_UNASSIGNED[10];
	char STR_SPECTATOR[9];
	strcpy(STR_TERROR,/*TERRORIST*/XorStr<0xC4, 10, 0x38B5214A>("\x90\x80\x94\x95\x87\x9B\x83\x98\x98" + 0x38B5214A).s);
	strcpy(STR_CT,/*CT*/XorStr<0xA9, 3, 0xF0057E58>("\xEA\xFE" + 0xF0057E58).s);
	strcpy(STR_UNASSIGNED,/*UNASSIGNED*/XorStr<0x96, 11, 0xEAA6E22A>("\xC3\xD9\xD9\xCA\xC9\xD2\xDB\xD3\xDB\xDB" + 0xEAA6E22A).s);
	strcpy(STR_SPECTATOR,/*SPECTATOR*/XorStr<0x8E, 10, 0x924A9907>("\xDD\xDF\xD5\xD2\xC6\xD2\xC0\xDA\xC4" + 0x924A9907).s);

	if (!strcmp(teamtext, STR_TERROR)) g_Player[px].team = 1;
	else if (!strcmp(teamtext, STR_CT)) g_Player[px].team = 2;
	else if (!strcmp(teamtext, STR_UNASSIGNED)) g_Player[px].team = 0;
	else if (!strcmp(teamtext, STR_SPECTATOR)) g_Player[px].team = 0;
	else {
		g_Player[px].team = -1;
	}

	if (px == GetLocalPlayer()->index)
	{
		if (!strcmp(teamtext, STR_TERROR)) g_Local.team = 1;
		else if (!strcmp(teamtext, STR_CT)) g_Local.team = 2;
		else if (!strcmp(teamtext, STR_UNASSIGNED)) g_Local.team = 0;
		else if (!strcmp(teamtext, STR_SPECTATOR)) g_Local.team = 0;
		else {
			g_Local.team = -1;
		}
	}
	return (*TeamInfoOrg)(pszName, iSize, pbuf);
}

int CurWeapon(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int iState = READ_BYTE();
	int iID = READ_CHAR();
	int iClip = READ_CHAR();
	if (iState) g_Local.iClip = iClip;
	WeaponListCurWeapon(iState, iID, iClip);
	return (*CurWeaponOrg)(pszName, iSize, pbuf);
}

int ScoreAttrib(const char *pszName, int iSize, void *pbuf)
{
	UpdateMe();
	BEGIN_READ(pbuf, iSize);
	int idx = READ_BYTE();
	int info = READ_BYTE();
	if (idx == g_Local.ent->index)
		g_Local.alive = info != IS_DEAD;

	g_Player[idx].hasbomb = info == HAS_BOMB;
	g_Player[idx].vip = info == IS_VIP;
	return (*ScoreAttribOrg)(pszName, iSize, pbuf);
}

int SetFOV(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	g_Local.iFOV = READ_BYTE();
	if (!g_Local.iFOV) { g_Local.iFOV = 90; }

	if (g_Local.iFOV == 90) { g_Local.inZoomMode = false; }
	else { g_Local.inZoomMode = true; }
	fCurrentFOV = g_Local.iFOV;
	return (*SetFOVOrg)(pszName, iSize, pbuf);
}

int Health(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	g_Local.iHealth = READ_BYTE();
	return (*HealthOrg)(pszName, iSize, pbuf);
}
int Battery(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	g_Local.iArmor = READ_BYTE();
	return (*BatteryOrg)(pszName, iSize, pbuf);
}


int ScoreInfo(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int id = READ_BYTE();
	int frags = READ_SHORT();
	int deaths = READ_SHORT();
	int classid = READ_SHORT();
	int teamid = READ_SHORT();
	if (id == g_Local.ent->index)
	{
		if (g_Local.iKills > frags)
		{
			g_Local.iKills = 0;
			g_Local.iHs = 0;
		}
		g_Local.iDeaths = deaths;
	}
	return (*ScoreInfoOrg)(pszName, iSize, pbuf);
}

int DeathMsg(const char *pszName, int iSize, void *pbuf)
{
	UpdateMe();
	BEGIN_READ(pbuf, iSize);
	int killer = READ_BYTE();
	int victim = READ_BYTE();
	int headshot = READ_BYTE();
	char* weaponName = READ_STRING();
	if (killer == g_Local.ent->index && headshot)
		g_Local.iHs++;
	if (killer == g_Local.ent->index && victim != g_Local.ent->index)
		g_Local.iKills++;
	g_Player[victim].setDead();
	g_Player[victim].updateClear();
	return (*DeathMsgOrg)(pszName, iSize, pbuf);
}

int SayText(const char *pszName, int iSize, void *pbuf)
{
	return (*SayTextOrg)(pszName, iSize, pbuf);
}

int TextMsg(const char *pszName, int iSize, void *pbuf)
{
	return (*TextMsgOrg)(pszName, iSize, pbuf);
}

int ResetHUD(const char *pszName, int iSize, void *pbuf)
{
	AtRoundStart();
	RoundsPlayed++;
	return (*ResetHUDOrg)(pszName, iSize, pbuf);
}

int Damage(const char *pszName, int iSize, void *pbuf)
{
	return (*DamageOrg)(pszName, iSize, pbuf);
}

int AmmoX(const char *pszName, int iSize, void *pbuf)
{
	return 0;
}

int WeaponListMsg(const char *pszName, int iSize, void *pbuf)
{
	return (*WeaponListOrg)(pszName, iSize, pbuf);
}

int Money(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	g_Local.iMoney = READ_SHORT();
	return (*MoneyOrg)(pszName, iSize, pbuf);
}

int ServerName(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	return (*ServerNameOrg)(pszName, iSize, pbuf);
}

int Radar(const char *pszName, int iSize, void *pbuf)
{
	return (*RadarOrg)(pszName, iSize, pbuf);
}

int	HookUserMsg(char *szMsgName, pfnUserMsgHook pfn)
{
	int retval;
	if (!modId)
	{
		OutputDebugString(szMsgName);
#define REDIRECT_MESSAGE(name)\
		else if (!strcmp(szMsgName,#name))\
		{\
			name##Org = pfn;\
			retval = g_Engine.pfnHookUserMsg(szMsgName,##name);\
		}
		if (0) {}
		else if (!strcmp(szMsgName,/*TeamInfo*/XorStr<0xA7, 9, 0x93B42356>("\xF3\xCD\xC8\xC7\xE2\xC2\xCB\xC1" + 0x93B42356).s))
		{
			TeamInfoOrg = pfn;
			retval = g_Engine.pfnHookUserMsg(szMsgName, TeamInfo);
		}
		else if (!strcmp(szMsgName,/*CurWeapon*/XorStr<0xF7, 10, 0x4D94E1F3>("\xB4\x8D\x8B\xAD\x9E\x9D\x8D\x91\x91" + 0x4D94E1F3).s))
		{
			CurWeaponOrg = pfn;
			retval = g_Engine.pfnHookUserMsg(szMsgName, CurWeapon);
		}
		else if (!strcmp(szMsgName,/*SetFOV*/XorStr<0xDD, 7, 0xFAC49F20>("\x8E\xBB\xAB\xA6\xAE\xB4" + 0xFAC49F20).s))
		{
			SetFOVOrg = pfn;
			retval = g_Engine.pfnHookUserMsg(szMsgName, SetFOV);
		}
		else if (!strcmp(szMsgName,/*Health*/XorStr<0x32, 7, 0xF4BA79A2>("\x7A\x56\x55\x59\x42\x5F" + 0xF4BA79A2).s))
		{
			HealthOrg = pfn;
			retval = g_Engine.pfnHookUserMsg(szMsgName, Health);
		}
		else if (!strcmp(szMsgName,/*ScoreInfo*/XorStr<0xE4, 10, 0xDC8EC937>("\xB7\x86\x89\x95\x8D\xA0\x84\x8D\x83" + 0xDC8EC937).s))
		{
			ScoreInfoOrg = pfn;
			retval = g_Engine.pfnHookUserMsg(szMsgName, ScoreInfo);
		}
		else if (!strcmp(szMsgName,/*WeaponList*/XorStr<0xA4, 11, 0xD15F2FC8>("\xF3\xC0\xC7\xD7\xC7\xC7\xE6\xC2\xDF\xD9" + 0xD15F2FC8).s)) // Because the Class is called like the Msg
		{
			WeaponListOrg = pfn;
			retval = g_Engine.pfnHookUserMsg(szMsgName, WeaponListMsg);
		}

		else
			retval = g_Engine.pfnHookUserMsg(szMsgName, pfn);
	}
	else
		retval = g_Engine.pfnHookUserMsg(szMsgName, pfn);
	return retval;
}