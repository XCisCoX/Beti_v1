
void EV_PlayerTrace(float*, float*, int, int, pmtrace_s*);
void EV_SetTraceHull(int);
cl_entity_s* GetEntityByIndex(int);
cl_entity_s* GetLocalPlayer();
model_s* GetModelByIndex(int);
int pfnDrawConsoleString(int, int, char*);
void pfnDrawConsoleStringLen(const char*, int*, int*);
void pfnDrawSetTextColor(float, float, float);
void pfnGetPlayerInfo(int, hud_player_info_t*);
int pfnGetScreenInfo(SCREENINFO*);
int WorldToScreen(float*, float*);
