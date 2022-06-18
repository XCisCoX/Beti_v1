#ifndef _XC3CLIENT_H_
#define _XC3CLIENT_H_
#pragma warning( disable :4311 )
#pragma warning( disable :4312 )
#pragma warning( disable :4313 )

#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "engine/cdll_int.h"
#include "engine/const.h"
#include "engine/progdefs.h"
#include "engine/eiface.h"
#include "engine/edict.h"
#include "engine/studio_event.h"
#include "engine/entity_types.h"
#include "engine/r_efx.h"
#include "engine/pmtrace.h"
#include "common/ref_params.h"
#include "common/screenfade.h"
#include "common/event_api.h"
#include "common/com_model.h"
#include "misc/parsemsg.h"
#include "misc/sprites.h"
#include "misc/r_studioint.h"
#include "engine/triangleapi.h"
#include "engine/pm_defs.h"
#include "engine/studio.h"
#include "engine/keydefs.h"
#include "common/net_api.h"

extern struct cl_enginefuncs_s g_Engine;
extern cl_enginefuncs_s* g_pEngine;
extern engine_studio_api_s* g_pStudio;
extern engine_studio_api_t g_Studio;
extern cl_enginefuncs_s * pEngfuncs;
extern engine_studio_api_s * pstudio;

void HUD_Frame(double time);
void DoBunnyHop(struct usercmd_s *usercmd);
void HUD_Redraw(float x, int y);
void HUD_PlayerMove(struct playermove_s *ppmove, qboolean server);
void CL_CreateMove(float frametime, struct usercmd_s *cmd, int active);
void PreV_CalcRefdef(struct ref_params_s *pparams);
void PostV_CalcRefdef(struct ref_params_s *pparams);
void HUD_AddEntity(int type, struct cl_entity_s *ent, const char *modelname);
void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed, int i);
int HUD_Key_Event(int eventcode, int keynum, const char *pszCurrentBinding);

void SPR_Set(int hPic, int r, int g, int b);
void SPR_Draw(int frame, int x, int y, const struct rect_s *prc);
void SPR_DrawHoles(int frame, int x, int y, const struct rect_s *prc);
void SPR_DrawAdditive(int frame, int x, int y, const struct rect_s *prc);
int DrawCharacter(int x, int y, int number, int r, int g, int b);

int DrawConsoleString(int x, int y, char *string);
void FillRGBA(int x, int y, int width, int height, int r, int g, int b, int a);

#pragma comment (lib , "winmm.lib")
#pragma comment (lib , "Glu32.lib")
#define WM_MOUSEWHEEL 0x020A
//==================================================================================

#endif