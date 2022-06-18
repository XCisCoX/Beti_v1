#include "gateways.h"
#include "client.h"
#include <windows.h>
CLIENT gClient = { NULL };
DWORD dwClientPtr = 0x01A179F0;
bool bClientActive = FALSE;
bool bEngineActive = FALSE;

void InitHack();
DWORD retaddress;

__declspec(naked)void Gateway2_HUD_Frame(void)
{
	__asm	call HUD_Frame;
	__asm	jmp retaddress;
}

DWORD Frame = (DWORD)&Gateway2_HUD_Frame;
__declspec(naked)void Gateway1_HUD_Frame(void)
{
	__asm	push esi;
	__asm	mov esi, dword ptr ss : [esp + 0x0C];
	__asm	mov retaddress, esi;
	__asm	push Frame
	__asm	pop esi;
	__asm	mov dword ptr ss : [esp + 0x0C], esi;
	__asm	pop esi;
	__asm	ret;
}
__declspec(naked)void Gateway2_V_CalcRefdef(void)
{
	__asm		call PostV_CalcRefdef;
	__asm	jmp retaddress;
}

DWORD CalcRef = (DWORD)&Gateway2_V_CalcRefdef;
__declspec(naked)void Gateway1_V_CalcRefdef(void)
{
	__asm		push esi;
	__asm	mov esi, dword ptr ss : [esp + 0x10];
	__asm		push esi;
	__asm		call PreV_CalcRefdef;
	__asm	add esp, 4;
	__asm	mov esi, dword ptr ss : [esp + 0x0c];
	__asm	mov retaddress, esi;
	__asm	push CalcRef;
	__asm	pop esi;
	__asm	mov dword ptr ss : [esp + 0x0c], esi;
	__asm	pop esi;
	__asm	ret;
}

__declspec(naked)void Gateway2_CL_CreateMove(void)
{
	__asm	call CL_CreateMove;
	__asm	jmp retaddress;
}

DWORD CreateMove = (DWORD)&Gateway2_CL_CreateMove;
__declspec(naked)void Gateway1_CL_CreateMove(void)
{
	__asm	push esi;
	__asm	mov esi, dword ptr ss : [esp + 0x28];
	__asm	mov retaddress, esi;
	__asm	push CreateMove;
	__asm	pop esi;
	__asm	mov dword ptr ss : [esp + 0x28], esi;
	__asm	pop esi;
	__asm	ret;
}

__declspec(naked)void Gateway2_HUD_Redraw(void)
{
	__asm	call HUD_Redraw;
	__asm jmp retaddress;
}

DWORD Redraw = (DWORD)&Gateway2_HUD_Redraw;
__declspec(naked)void Gateway1_HUD_Redraw(void)
{
	__asm	push esi;
	__asm	mov esi, dword ptr ss : [esp + 0x10];
	__asm	mov retaddress, esi;
	__asm	push Redraw
	__asm	pop esi;
	__asm	mov dword ptr ss : [esp + 0x10], esi;
	__asm		pop esi;
	__asm	ret;
}

__declspec(naked)void Gateway2_HUD_PostRunCmd(void)
{
	__asm	call HUD_PostRunCmd;
	__asm	jmp retaddress;
}

DWORD PostRunCmd = (DWORD)&Gateway2_HUD_PostRunCmd;
__declspec(naked)void Gateway1_HUD_PostRunCmd(void)
{
	__asm		push esi;
	__asm		mov esi, dword ptr ss : [esp + 0x38];
	__asm		mov retaddress, esi;
	__asm		push PostRunCmd
	__asm		pop esi;
	__asm	mov dword ptr ss : [esp + 0x38], esi;
	__asm	pop esi;
	__asm	ret;
}

__declspec(naked)void Gateway2_HUD_PlayerMove(void)
{
	__asm		call HUD_PlayerMove;
	__asm		jmp retaddress;
}

DWORD PlayerMove = (DWORD)&Gateway2_HUD_PlayerMove;
__declspec(naked)void Gateway1_HUD_PlayerMove(void)
{
	__asm		push esi;
	__asm		mov esi, dword ptr ss : [esp + 0x10];
	__asm		mov retaddress, esi;
	__asm		push PlayerMove
	__asm		pop esi;
	__asm		mov dword ptr ss : [esp + 0x10], esi;
	__asm		pop esi;
	__asm	ret;
}
extern int AddEntResult;
__declspec(naked)void Gateway2_HUD_AddEntity(void)
{
	__asm		mov AddEntResult, eax;
	__asm		call HUD_AddEntity;
	__asm	mov eax, AddEntResult;
	__asm	jmp retaddress;
}

DWORD AddEnt = (DWORD)&Gateway2_HUD_AddEntity;
__declspec(naked)void Gateway1_HUD_AddEntity(void)
{
	__asm		push esi;
	__asm		mov esi, dword ptr ss : [esp + 0x14];
	__asm	mov retaddress, esi;
	__asm	push AddEnt
	__asm		pop esi;
	__asm	mov dword ptr ss : [esp + 0x14], esi;
	__asm	pop esi;
	__asm	ret;
}
extern int KeyEventResult;
__declspec(naked)void Gateway2_HUD_Key_Event(void)
{
	__asm	mov KeyEventResult, eax;
	__asm	call HUD_Key_Event;
	__asm	mov eax, KeyEventResult;
	__asm	jmp retaddress;
}

DWORD KeyEvent = (DWORD)&Gateway2_HUD_Key_Event;
__declspec(naked)void Gateway1_HUD_Key_Event(void)
{
	__asm		push esi;
	__asm		mov esi, dword ptr ss : [esp + 0x14];
	__asm	mov retaddress, esi;
	__asm	push KeyEvent
	__asm	pop esi;
	__asm	mov dword ptr ss : [esp + 0x14], esi;
	__asm	pop esi;
	__asm	ret;
}

__declspec(naked) void NullStub()
{
	_asm ret;
}


bool ActivateClient()
{
	if (!IsBadReadPtr((LPCVOID)dwClientPtr, sizeof DWORD))
	{
		if (*((DWORD*)dwClientPtr) != NULL)
		{
			DWORD dwClientRealPtr;

			__asm	push eax;					
			__asm		push edx;				
			__asm	mov edx, dwClientPtr;		
			__asm	mov eax, [edx];				
			__asm	mov dwClientRealPtr, eax;	
			__asm	pop edx;					
			__asm	pop eax;				
		
			memcpy(&gClient, (LPVOID)dwClientRealPtr, sizeof CLIENT);

	
			gClient.HUD_Frame = (HUD_FRAME_FUNCTION)&Gateway1_HUD_Frame;
			gClient.HUD_Redraw = (HUD_REDRAW_FUNCTION)&Gateway1_HUD_Redraw;
			gClient.HUD_PlayerMove = (HUD_CLIENTMOVE_FUNCTION)&Gateway1_HUD_PlayerMove;
			gClient.CL_CreateMove = (HUD_CL_CREATEMOVE_FUNCTION)&Gateway1_CL_CreateMove;
			gClient.V_CalcRefdef = (HUD_V_CALCREFDEF_FUNCTION)&Gateway1_V_CalcRefdef;
			gClient.HUD_AddEntity = (HUD_ADDENTITY_FUNCTION)&Gateway1_HUD_AddEntity;
			gClient.HUD_PostRunCmd = (HUD_POSTRUNCMD_FUNCTION)&Gateway1_HUD_PostRunCmd;
			gClient.HUD_Key_Event = (HUD_KEY_EVENT_FUNCTION)&Gateway1_HUD_Key_Event;

		
			DWORD dwClientNewPtr = (DWORD)&gClient;

			__asm		push eax;					
			__asm		push edx;					
			__asm		mov edx, dwClientPtr;		
			__asm		mov eax, dwClientNewPtr;	
			__asm		mov[edx], eax;				
			__asm	pop edx;					
			__asm	pop eax;				

			bClientActive = TRUE;
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else {
		return FALSE;
	}
	return TRUE;
}

bool ActivateEngine()
{
	if (g_pEngine->pfnHookUserMsg && g_pEngine->pfnHookEvent)
	{
		memcpy(&g_Engine, g_pEngine, sizeof(cl_enginefunc_t));
		if (g_pStudio->GetModelByIndex)
		{
			memcpy(&g_Studio, g_pStudio, sizeof(g_Studio));
		}
		else {
			return FALSE;
		}


		g_pEngine->pfnSPR_Set = &SPR_Set;
		g_pEngine->pfnSPR_Draw = &SPR_Draw;
		g_pEngine->pfnSPR_DrawHoles = &SPR_DrawHoles;
		g_pEngine->pfnSPR_DrawAdditive = &SPR_DrawAdditive;
		g_pEngine->pfnDrawCharacter = &DrawCharacter;
		g_pEngine->pfnDrawConsoleString = &DrawConsoleString;
		g_pEngine->pfnFillRGBA = &FillRGBA;

		g_pEngine->pfnHookUserMsg = &HookUserMsg;

		InitHack();

		bEngineActive = TRUE;
	}
	else {
		return FALSE;
	}
	return TRUE;
}
