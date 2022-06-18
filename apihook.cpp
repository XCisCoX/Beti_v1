#include <windows.h>
#include "players.h"
#include "color.h"
#include "client.h"
#include "apihook.h"
#include "stdafx.h"
#include "Engine.h"
void UpdateMe(void)
{
	g_Local.ent = GetLocalPlayer();
	static cl_entity_s dummy;
	memset((char*)&dummy, 0, sizeof(dummy));
	if (!g_Local.ent) { g_Local.ent = &dummy; }
}
bool bIsValidEnt(struct cl_entity_s *ent)
{
	if (ent->player && g_Local.iIndex != ent->index
		&& ent->curstate.movetype != 6
		&& ent->curstate.movetype != 0
		&& !(ent->curstate.messagenum < GetLocalPlayer()->curstate.messagenum)
		&& !(GetLocalPlayer()->curstate.iuser1 == 4
			&& GetLocalPlayer()->curstate.iuser2 == ent->index)
		&& g_Player[ent->index].team > 0 && g_Player[ent->index].team <= 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool bIsEntValid(cl_entity_s * ent, int index)
{
	if (g_Player[index].updateType() == 2 || g_Player[index].timeSinceLastUpdate() < 0.3)
		return true;
	UpdateMe(); 
	cl_entity_s* localEnt = GetEntityByIndex(g_Local.ent->index);

	if (ent && !(ent->curstate.effects & EF_NODRAW) && ent->player && !ent->curstate.spectator
		&& ent->curstate.solid && !(ent->curstate.messagenum < localEnt->curstate.messagenum))
		return true;

	return false;
}
ColorEntry * PlayerColor(int ax)
{
	static ColorEntry asd;
	asd.r = 0;
	asd.g = 0;
	asd.b = 0;
	asd.a = 255;
	return &asd;
}
extern SCREENINFO g_Screen;
bool CalcScreen(float *pflOrigin, float *pflVecScreen)
{
	int iResult = WorldToScreen(pflOrigin, pflVecScreen);
	if (pflVecScreen[0] < 1 && pflVecScreen[1] < 1 && pflVecScreen[0] > -1 && pflVecScreen[1] > -1 && !iResult)
	{
		pflVecScreen[0] = pflVecScreen[0] * (g_Screen.iWidth / 2) + (g_Screen.iWidth / 2);
		pflVecScreen[1] = -pflVecScreen[1] * (g_Screen.iHeight / 2) + (g_Screen.iHeight / 2);
		return true;
	}
	return false;
}
bool bPathFree(float *pflFrom, float *pflTo)
{
	if (!pflFrom || !pflTo) { return false; }
	pmtrace_t pTrace;
	EV_SetTraceHull(2);
	EV_PlayerTrace(pflFrom, pflTo, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_Local.iIndex, &pTrace);
	return (pTrace.fraction == 1.0f);
}
ofstream ofile;
void __cdecl add_log(const char *fmt, ...)
{
	if (!fmt)
	{
		return;
	}
	va_list va_alist;
	char logbuf[256] = { 0 };
	va_start(va_alist, fmt);
	_vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end(va_alist);
	ofile << logbuf << endl;
}