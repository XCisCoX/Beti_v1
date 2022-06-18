

#include <Windows.h>
#include "client.h"
#include "xEngine.h"

DWORD dwClientCmd = 0x01D0D3C5;
DWORD dwCenterPrint = 0x01D0D9F5;
DWORD dwCreateVisibleEntity = 0x01D0ED65;
DWORD dwGetEntityByIndex = 0x01D0EC65;
DWORD dwConsolePrint = 0x01D0D9D5;
DWORD dwSetScreenFade = 0x01D0F065;
DWORD dwGetScreenFade = 0x01D0F035;
DWORD dwSetViewAngles = 0x01D0EB05;
DWORD dwDrawConsoleString = 0x01D3C637;
DWORD dwDrawSetTextColor = 0x01D3C588;
DWORD dwGetViewModel = 0x01D0EC56;
DWORD dwDrawCharacter = 0x01DBDEE8;
DWORD dwGetScreenInfo = 0x01D0D1D5;
DWORD dwDrawConsoleStringLen = 0x01D0D998;

__declspec(naked) INT xpfnClientCmd(char *szCmdString)
{
	_asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 4]
		PUSH EAX
		JMP[dwClientCmd]
	}
}

__declspec(naked) INT xpfnCenterPrint(char *String)
{
	_asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 4]
		PUSH EAX
		JMP[dwCenterPrint]
	}
}

__declspec(naked) INT xCreateVisibleEntity(int type, struct cl_entity_s *ent)
{
	_asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 8]
		PUSH ESI
		JMP[dwCreateVisibleEntity]
	}
}

__declspec(naked) struct cl_entity_s xGetEntityByIndex(int idx)
{
	_asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 4]
		PUSH EAX
		JMP[dwGetEntityByIndex]
	}
}

__declspec(naked) VOID xpfnConsolePrint(char *String)
{
	_asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 4]
		PUSH EAX
		JMP[dwConsolePrint]
	}
}

__declspec(naked) VOID xpfnSetScreenFade(struct screenfade_s *fade)
{
	_asm
	{
		PUSH ESI
		LEA EAX, DWORD PTR SS : [ESP + 8]
		JMP[dwSetScreenFade]
	}
}

__declspec(naked) VOID xpfnGetScreenFade(struct screenfade_s *fade)
{
	_asm
	{
		PUSH ESI
		LEA EAX, DWORD PTR SS : [ESP + 8]
		JMP[dwGetScreenFade]
	}
}

__declspec(naked) VOID xSetViewAngles(float* fAngels)
{
	_asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 4]
		PUSH EAX
		JMP[dwSetViewAngles]
	}
}

__declspec(naked) INT xpfnGetScreenInfo(SCREENINFO *pscrinfo)
{
	_asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 4]
		PUSH EAX
		JMP[dwGetScreenInfo]
	}
}

__declspec(naked) INT xpfnDrawCharacter(int x, int y, int number, int r, int g, int b)
{
	_asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 18]
		LEA ECX, DWORD PTR SS : [ESP + 14]
		JMP[dwDrawCharacter]
	}
}

__declspec(naked) INT xpfnDrawConsoleString(int x, int y, char *string)
{
	_asm
	{
		PUSH ESI
		PUSH EDI
		CALL DWORD PTR DS : [01D09C50h]
		JMP[dwDrawConsoleString]
	}
}

__declspec(naked) VOID xpfnDrawSetTextColor(float r, float g, float b)
{
	_asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 0Ch]
		LEA ECX, DWORD PTR SS : [ESP + 8]
		JMP[dwDrawSetTextColor]
	}
}

__declspec(naked) VOID xpfnDrawConsoleStringLen(const char *string, int *length, int *height)
{
	_asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 0Ch]
		LEA ECX, DWORD PTR SS : [ESP + 8]
		JMP[dwDrawConsoleStringLen]
	}
}

__declspec(naked) struct cl_entity_s xGetViewModel(void)
{
	_asm
	{
		CALL DWORD PTR DS : [1EC91E0h]
		JMP[dwGetViewModel]
	}
}
