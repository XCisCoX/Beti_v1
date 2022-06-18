/*

CODE BY: ALOT OF PEOPLE!!!!!!!!!!!!! but by me: Tkz ;)

to: 2016

www.unknowncheats.me

*/

#include "ESP.h"
#include "client.h"
#include "players.h"
#include "drawing.h"
#include "color.h"
#include "Engine.h"

#include "apihook.h"
#include "cvar.h"
extern bool oglSubtractive;

int GetTeamByModel(char *mdl)
{
	if (strstr(mdl, "leet") ||
		strstr(mdl, "terror") ||
		strstr(mdl, "arctic") ||
		strstr(mdl, "guerilla"))
	{
		return 0;
	}

	if (strstr(mdl, "gign") ||
		strstr(mdl, "gsg9") ||
		strstr(mdl, "sas") ||
		strstr(mdl, "urban") ||
		strstr(mdl, "vip"))
	{
		return 1;
	}

	return -1;
}
int Cstrike_SequenceInfo[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0..9
	0, 1, 2, 0, 1, 2, 0, 1, 2, 0, // 10..19
	1, 2, 0, 1, 1, 2, 0, 1, 1, 2, // 20..29
	0, 1, 2, 0, 1, 2, 0, 1, 2, 0, // 30..39
	1, 2, 0, 1, 2, 0, 1, 2, 0, 1, // 40..49
	2, 0, 1, 2, 0, 0, 0, 4, 0, 4, // 50..59
	0, 5, 0, 5, 0, 0, 1, 1, 2, 0, // 60..69
	1, 1, 2, 0, 1, 0, 1, 0, 1, 2, // 70..79
	0, 1, 2, 3, 3, 3, 3, 3, 3, 3, // 80..89
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, // 90..99
	3
};
/* -=-=-=-=-=-=-=-=-=- *//* -=-=-=-=-=-=-=-=-=- *//* -=-=-=-=-=-=-=-=-=- */
int GetPlayerTeam()								// Credits ap0c -- used for visbox and visglow
{
	cl_entity_t *pLocal = GetLocalPlayer();
	hud_player_info_t pinfo;

	for (int i = 0; i < 33; i++)
	{
		pfnGetPlayerInfo(i, &pinfo);

		if (i == pLocal->index)
		{
			if (strstr(pinfo.model, "arctic") || strstr(pinfo.model, "guerilla") || strstr(pinfo.model, "leet") || strstr(pinfo.model, "terror"))
			{
				return 1;
			}
			else if (strstr(pinfo.model, "gign") || strstr(pinfo.model, "gsg9") || strstr(pinfo.model, "sas") || strstr(pinfo.model, "urban") || strstr(pinfo.model, "vip"))
			{
				return 2;
			}
		}
	}

	return 0;
}
//=-=-=-==-==-=-=-=-=-=--==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--==-
float GetDist(float *from, float *to)
{
	float ang[3];
	ang[0] = to[0] - from[0];
	ang[1] = to[1] - from[1];
	ang[2] = to[2] - from[2];
	return (float)sqrt(ang[0] * ang[0] + ang[1] * ang[1] + ang[2] * ang[2]);
}
/* -=-=-=-=-=-=-=-=-=- *//* -=-=-=-=-=-=-=-=-=- *//* -=-=-=-=-=-=-=-=-=- */
char* gGetWeaponName(int weaponmodel)
{
	static char weapon[50];
	weapon[0] = 0;

	model_s* mdl = GetModelByIndex(weaponmodel); // use engine structure model_s and initialize mdl with a pointer to address GetModelByIndex.
	if (!mdl) { return weapon; }

	char* name = mdl->name;  if (!name) { return weapon; }
	int len = strlen(name);  if (len > 48 || len < 10) { return weapon; }

	strcpy(weapon, name + 9); len -= 9;
	if (len > 4)weapon[len - 4] = (char)0;

	return weapon;
}

//===================================================================================================
void DrawEntityEsp(void)
{
	cl_entity_s * pMe = GetLocalPlayer();
	for (int i = 1; i < 1024; i++)
	{
		cl_entity_s * pEnt = GetEntityByIndex(i);
		ColorEntry * clr;
		if (pEnt && (pEnt->curstate.messagenum + 10 > pMe->curstate.messagenum) && pEnt->model && pEnt->model->name)
		{
			float tmp[2];
			char mdl[64];
			char *wpn = NULL;

			strncpy(mdl, pEnt->model->name, 64);
			wpn = mdl;

			if (wpn && CalcScreen(pEnt->origin, tmp) && strstr(wpn, "w_"))
			{
				wpn += 9;
				wpn[strlen(wpn) - 4] = 0;
				clr = colorList.get(5);
				g_Drawing.DrawStringCenter(tmp[0], tmp[1], clr->r, clr->g, clr->b, "%s", wpn);
			}
			else if (wpn && (strstr(wpn, "hostage") || (strstr(wpn, "scientist"))))
			{
				vec3_t forward, right, up, entOrg;

				wpn += 7;
				wpn[strlen(wpn) - 4] = 0;

				VectorCopy(pEnt->origin, entOrg);
				g_Engine.pfnAngleVectors(pEnt->angles, forward, right, up);
				entOrg = entOrg + up * 40;
				clr = colorList.get(6);
				if (CalcScreen(entOrg, tmp))
					g_Drawing.DrawStringCenter(tmp[0], tmp[1], clr->r, clr->g, clr->b, "%s", wpn);
			}
		}
		else if (!pEnt)
			break;
	}
}
//===================================================================================================
void BoxEsp()
{
	//float drawhere[2];
	vec3_t vecScreen;
	int espr, espg, espb; // For teh Weapon
	espr = 255;
	espg = 0;
	espb = 0;
	char nbuf[1024];
	cl_entity_t /**ent,*/ *pLocal = GetLocalPlayer();
	hud_player_info_t pinfo;
	// Lets begin a loop
	for (int i = 0; i < 33; i++)
	{
		if (i == pLocal->index) continue;
		cl_entity_s *ent = GetEntityByIndex(i);
		pfnGetPlayerInfo(i, &pinfo);
		if (ent != NULL && bIsValidEnt(ent)/*Credits to tabris*/)
		{
			if (CalcScreen(ent->origin, vecScreen))
				sprintf(nbuf, "%s", pinfo.name);
			float drawhere[2];
			CalcScreen(ent->origin, drawhere);
			{
				sprintf(nbuf, "%s", pinfo.name);
				CalcScreen(ent->origin, drawhere);
				int namelen;
				namelen = strlen(nbuf);
				if (strstr(pinfo.model, "arctic") || strstr(pinfo.model, "guerilla") || strstr(pinfo.model, "leet") || strstr(pinfo.model, "militia") || strstr(pinfo.model, "terror")) {
					espr = 255;
					espg = 180;
					espb = 40;
				}
				else if (strstr(pinfo.model, "gign") || strstr(pinfo.model, "gsg9") || strstr(pinfo.model, "sas") || strstr(pinfo.model, "urban") || strstr(pinfo.model, "spetsnaz") || strstr(pinfo.model, "vip")) {
					espr = 40;
					espg = 180;
					espb = 255;
				}
				if (bIsValidEnt(ent) == true)
				{
					g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] - 10, 20, 2, espr, espg, espb, 200);
					g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] - 10, 2, 20, espr, espg, espb, 200);
					g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] + 9, 20, 2, espr, espg, espb, 200);
					g_Drawing.FillRGBA(drawhere[0] + 9, drawhere[1] - 10, 2, 20, espr, espg, espb, 200);
				}
			}
		}
	}
}
//===================================================================================================
void PlayerEsp()
{
	float drawhere[2];
	int espr, espg, espb; // For teh Weapon
	espr = 255;
	espg = 0;
	espb = 0;
	char nbuf[1024];
	cl_entity_t *pLocal = GetLocalPlayer();
	hud_player_info_t pinfo;
	// Lets begin a loop
	for (int i = 0; i < 33; i++)
	{
		if (i == pLocal->index) continue;

		cl_entity_s *getcurent = GetEntityByIndex(i);
		float distance = GetDist(pLocal->origin, getcurent->origin) / 22.0f;

		cl_entity_s *ent = GetEntityByIndex(i);
		pfnGetPlayerInfo(i, &pinfo);
		if (ent != NULL &&  bIsValidEnt(ent)/*Credits to tabris*/)
		{
			if (CalcScreen(ent->origin, drawhere))
			{
				sprintf(nbuf, "%s", pinfo.name);
				float drawhere[2];
				CalcScreen(ent->origin, drawhere);
				{
					int namelen;
					namelen = strlen(nbuf);
					if (strstr(pinfo.model, "arctic") || strstr(pinfo.model, "guerilla") || strstr(pinfo.model, "leet") || strstr(pinfo.model, "terror"))
					{
						espr = 255;
						espg = 180;
						espb = 0;
					}
					else
						if (strstr(pinfo.model, "gign") || strstr(pinfo.model, "gsg9") || strstr(pinfo.model, "sas") || strstr(pinfo.model, "urban") || strstr(pinfo.model, "vip"))
						{
							espr = 0;
							espg = 180;
							espb = 255;
						}
					if (ent != NULL &&  bIsValidEnt(ent)/*Credits to tabris*/)
					{
						if (weapon)
						{
							g_Drawing.DrawHudStringCenter((int)drawhere[0], (int)drawhere[1], espr, espg, espb, "%s", gGetWeaponName(ent->curstate.weaponmodel));
						}

						if (name)
						{
							g_Drawing.DrawHudStringCenter((int)drawhere[0], (int)drawhere[1] - 16, espr, espg, espb, nbuf);
						}
					}
					if (ent != NULL &&  bIsValidEnt(getcurent))
					{
						if (CalcScreen(getcurent->origin, drawhere))
						{
							if (distanesp)
							{
								g_Drawing.DrawHudStringCenter((int)drawhere[0], (int)drawhere[1] + 11, espr, espg, espb, "[%.2f]", distance);
							}
						}
						if (reload)
						{
							int atype = Cstrike_SequenceInfo[ent->curstate.sequence];
							if (atype == 2)
							{
								oglSubtractive = true;
								g_Drawing.FillRGBA((int)drawhere[0] - 26, (int)drawhere[1] + 1, 53, 12, 0, 0, 0, 250);
								oglSubtractive = false;
								g_Drawing.FillRGBA((int)drawhere[0] - 25, (int)drawhere[1] + 2, (ent->curstate.frame / 255) * 50, 10, espr, espg, espb, 255);
							}
							else if (atype == 16)
							{
								oglSubtractive = true;
								g_Drawing.FillRGBA((int)drawhere[0] - 26, (int)drawhere[1] + 1, 53, 12, 0, 0, 0, 250);
								oglSubtractive = false;
								g_Drawing.FillRGBA((int)drawhere[0] - 25, (int)drawhere[1] + 2, (ent->curstate.frame / 255) * 101, 10, 255, 255, 0, 255);
							}
							if (reload)
							{
								if (atype == 4)
								{
									g_Drawing.DrawHudString(drawhere[0] - namelen * 2, drawhere[1] + 1 - 53, 0, 255, 0, "-SEND ITEM-");
								}
								if (atype == 3)
								{
									g_Drawing.DrawHudString(drawhere[0] - namelen * 2, drawhere[1] + 1 - 53, 0, 255, 0, "-DAMAGE-");
								}
								if (atype == 5)
								{
									g_Drawing.DrawHudString(drawhere[0] - namelen * 2, drawhere[1] - 120, 0, 255, 0, "-PLANTING BOMB-");
								}
							}
						}
					}
				}
			}
		}
	}
}
//===================================================================================================
void visEsp()
{
	float drawhere[2];
	int espr, espg, espb;
	espr = 255;
	espg = 0;
	espb = 0;

	cl_entity_t *pLocal = GetLocalPlayer();
	hud_player_info_t pinfo;

	int teamme = 0;
	int teamot = 0;
	bool visible = false;

	teamme = GetPlayerTeam();

	for (int i = 0; i < 33; i++)
	{
		if (i == pLocal->index) continue;
		cl_entity_s *ent = GetEntityByIndex(i);
		pmtrace_t *vis = g_pEngine->PM_TraceLine(pLocal->origin, ent->origin, 0, 2, -1);
		pfnGetPlayerInfo(i, &pinfo);

		if (ent != NULL && bIsValidEnt(ent))
		{
			if (CalcScreen(ent->origin, drawhere))
			{
				if (i != pLocal->index)
				{
					if (strstr(pinfo.model, "arctic") || strstr(pinfo.model, "guerilla") || strstr(pinfo.model, "leet") || strstr(pinfo.model, "terror"))
					{
						espr = 255;
						espg = 180;
						espb = 0;

						teamot = 1;
					}
					else if (strstr(pinfo.model, "gign") || strstr(pinfo.model, "gsg9") || strstr(pinfo.model, "sas") || strstr(pinfo.model, "urban") || strstr(pinfo.model, "vip"))
					{
						espr = 0;
						espg = 180;
						espb = 255;

						teamot = 2;
					}

					if (vis->fraction == 1.0f)
					{
						visible = true;
					}
					else if (vis->fraction < 1.0f)
					{
						visible = false;
					}
				}

				if (teamot != teamme)
				{
					if (visible)
					{
						g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] - 10, 20, 2, 0, 255, 0, 200);
						g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] - 10, 2, 20, 0, 255, 0, 200);
						g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] + 10, 20, 2, 0, 255, 0, 200);
						g_Drawing.FillRGBA(drawhere[0] + 10, drawhere[1] - 10, 2, 20, 0, 255, 0, 200);
					}

					else if (!visible)
					{
						g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] - 10, 20, 2, espr, espg, espb, 200);
						g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] - 10, 2, 20, espr, espg, espb, 200);
						g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] + 10, 20, 2, espr, espg, espb, 200);
						g_Drawing.FillRGBA(drawhere[0] + 10, drawhere[1] - 10, 2, 20, espr, espg, espb, 200);
					}
				}
				else if (teamot == teamme)
				{
					if (visible || !visible)
					{
						g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] - 10, 20, 2, espr, espg, espb, 200);
						g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] - 10, 2, 20, espr, espg, espb, 200);
						g_Drawing.FillRGBA(drawhere[0] - 10, drawhere[1] + 10, 20, 2, espr, espg, espb, 200);
						g_Drawing.FillRGBA(drawhere[0] + 10, drawhere[1] - 10, 2, 20, espr, espg, espb, 200);
					}
				}
			}
		}
	}
}