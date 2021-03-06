
#include"client.h"
#define NAKED __declspec(naked)
extern int cdeath;
NAKED void EV_PlayerTrace(float* start, float* end, int traceFlags, int ignore_pe, pmtrace_s* tr)
{
	__asm
	{
		MOV EAX, DWORD PTR SS : [ESP + 10h]
		MOV ECX, DWORD PTR SS : [ESP + 0Ch]
		MOV EDX, DWORD PTR SS : [ESP + 08h]
		SUB ESP, 44h
		PUSH ESI
		PUSH EDI
		PUSH EAX
		MOV EAX, DWORD PTR SS : [ESP + 54h]
		PUSH ECX
		PUSH EDX
		LEA ECX, DWORD PTR SS : [ESP + 14h]
		PUSH EAX
		PUSH ECX
		MOV EAX, 01D75950h
		CALL EAX
		MOV EDI, DWORD PTR SS : [ESP + 74h]
		MOV ECX, 11h
		MOV ESI, EAX
		ADD ESP, 14h
		REP MOVS DWORD PTR ES : [EDI], DWORD PTR DS : [ESI]
		POP EDI
		POP ESI
		ADD ESP, 44h
		RETN
	}
}
//===================================================================================================
NAKED void EV_SetTraceHull(int hull)
{
	__asm
	{
		MOV ECX, DWORD PTR DS : [024329CCh]; hl.02D5FE20
		MOV EAX, DWORD PTR SS : [ESP + 04h]
		MOV DWORD PTR DS : [ECX + 0BCh], EAX
		RETN
	}
}
//===================================================================================================
NAKED cl_entity_s* GetEntityByIndex(int idx)
{
	__asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 04h]
		PUSH EAX
		MOV EAX, DWORD PTR SS : [ESP + 08h]
		ADD ESP, 04h
		TEST EAX, EAX
		JL label_01
		CMP EAX, DWORD PTR DS : [02DB64E0h]
		JGE label_01
		LEA EAX, DWORD PTR DS : [EAX + EAX * 02h]
		MOV EDX, DWORD PTR DS : [01EF9B04h]
		LEA EAX, DWORD PTR DS : [EAX + EAX * 04h]
		LEA EAX, DWORD PTR DS : [EAX + EAX * 04h]
		LEA ECX, DWORD PTR DS : [EAX + EAX * 04h]
		LEA EAX, DWORD PTR DS : [EDX + ECX*08h]
		RETN
		label_01 :
		XOR EAX, EAX
			RETN
	}
}
//===================================================================================================
NAKED cl_entity_s* GetLocalPlayer()
{
	__asm
	{
		//CALL DWORD PTR DS:[1ECD4C4]              ; hl.01D63590
		MOV EAX, DWORD PTR DS : [02F5218Ch]
		/*CMP cdeath,01h
		JNZ label_no_cd
		MOV EBX,DWORD PTR DS:[GLP+0Ch]
		ADD EBX,GLP+10h
		CALL EBX
		JMP label_cd*/

		//label_no_cd:

		MOV ECX, DWORD PTR DS : [01EF9B04h]

		//label_cd:

		LEA EAX, DWORD PTR DS : [EAX + EAX * 02h + 03h]
		LEA EAX, DWORD PTR DS : [EAX + EAX * 04h]
		LEA EAX, DWORD PTR DS : [EAX + EAX * 04h]
		LEA EAX, DWORD PTR DS : [EAX + EAX * 04h]
		LEA EAX, DWORD PTR DS : [ECX + EAX*08h]
		RETN
	}
}
//===================================================================================================
NAKED model_s* GetModelByIndex(int index)
{
	__asm
	{
		SUB ESP, 10h
		PUSH ESI
		PUSH EDI
		MOV EDI, DWORD PTR SS : [ESP + 1Ch]
		MOV ESI, DWORD PTR DS : [EDI * 04h + 02F53190h]
		TEST ESI, ESI
		JNZ label_01
		POP EDI
		XOR EAX, EAX
		POP ESI
		ADD ESP, 10h
		RETN

		label_01 :

		MOV EAX, DWORD PTR DS : [ESI + 40h]
			CMP EAX, 01h
			JE label_02
			CMP EAX, 02h
			JNZ label_03

			label_02 :

		FLD DWORD PTR DS : [01EBA270h]
			FCOMP DWORD PTR DS : [01E8708Ch]
			FSTSW AX
			TEST AH, 44h
			JPO label_04
			MOV EAX, 01DB96E0h
			CALL EAX
			FSTP QWORD PTR SS : [ESP + 10h]
			PUSH 00h
			PUSH 00h
			PUSH ESI
			MOV EAX, 01D40B50h
			CALL EAX
			MOV EAX, 01DB96E0h
			CALL EAX
			FSTP QWORD PTR SS : [ESP + 14h]
			FLD QWORD PTR SS : [ESP + 14h]
			FSUB QWORD PTR SS : [ESP + 1Ch]
			ADD ESP, 04h
			FSTP DWORD PTR SS : [ESP + 24h]
			FLD DWORD PTR SS : [ESP + 24h]
			FSTP QWORD PTR SS : [ESP]
			PUSH ESI
			PUSH 01EBC26Ch; ASCII "fs_precache_timings: loaded model %s in time %.3f sec"
			MOV EAX, 01D2F960h
			CALL EAX
			MOV EAX, DWORD PTR DS : [EDI * 04h + 02F53190h]
			ADD ESP, 10h
			POP EDI
			POP ESI
			ADD ESP, 10h
			RETN

			label_04 :

		PUSH 00h
			PUSH 00h
			PUSH ESI
			MOV EAX, 01D40B50h
			CALL EAX
			ADD ESP, 0Ch

			label_03 :

		MOV EAX, DWORD PTR DS : [EDI * 04h + 02F53190h]
			POP EDI
			POP ESI
			ADD ESP, 10h
			RETN
	}
}
//===================================================================================================
NAKED int pfnDrawConsoleString(int x, int y, char* string)
{
	__asm
	{
		PUSH ESI
		PUSH EDI
		MOV EAX, 01D09CA0h
		CALL EAX
		MOV ECX, DWORD PTR SS : [ESP + 10h]
		MOV ESI, DWORD PTR SS : [ESP + 0Ch]
		PUSH EAX
		MOV EAX, DWORD PTR SS : [ESP + 18h]
		PUSH EAX
		PUSH ECX
		PUSH ESI
		MOV EAX, 01D09E40h
		CALL EAX
		ADD ESP, 10h
		MOV EDI, EAX
		MOV EAX, 01D3CD70h
		CALL EAX
		LEA EAX, DWORD PTR DS : [EDI + ESI]
		POP EDI
		POP ESI
		RETN
	}
}
//===================================================================================================
NAKED void pfnDrawConsoleStringLen(const char* string, int* length, int* height)
{
	__asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 0Ch]
		LEA ECX, DWORD PTR SS : [ESP + 08h]
		PUSH EAX
		LEA EDX, DWORD PTR SS : [ESP + 08h]
		PUSH ECX
		PUSH EDX
		//CALL DWORD PTR DS:[1ECD46C]              ; hl.01D63590
		MOV EAX, 01D09CA0h
		CALL EAX
		PUSH EAX
		MOV EAX, DWORD PTR SS : [ESP + 14h]
		PUSH EAX
		MOV EAX, 01D3CB00h
		CALL EAX
		MOV ECX, DWORD PTR SS : [ESP + 1Ch]
		MOV DWORD PTR DS : [ECX], EAX
		MOV EAX, 01D09CA0h
		CALL EAX
		PUSH EAX
		MOV EAX, 01D0A1A0h
		CALL EAX
		MOV EDX, DWORD PTR SS : [ESP + 24h]
		ADD ESP, 18h
		MOV DWORD PTR DS : [EDX], EAX
		RETN
	}
}
//===================================================================================================
NAKED void pfnDrawSetTextColor(float r, float g, float b)
{
	__asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 0Ch]
		LEA ECX, DWORD PTR SS : [ESP + 08h]
		PUSH EAX
		LEA EDX, DWORD PTR SS : [ESP + 08h]
		PUSH ECX
		PUSH EDX
		//CALL DWORD PTR DS:[1ECD468]              ; hl.01D63590
		FLD DWORD PTR SS : [ESP + 18h]
		FMUL DWORD PTR DS : [01E870F4h]
		MOV EAX, 01E63820h
		CALL EAX
		FLD DWORD PTR SS : [ESP + 14h]
		FMUL DWORD PTR DS : [01E870F4h]
		PUSH EAX
		MOV EAX, 01E63820h
		CALL EAX
		FLD DWORD PTR SS : [ESP + 14h]
		FMUL DWORD PTR DS : [01E870F4h]
		PUSH EAX
		MOV EAX, 01E63820h
		CALL EAX
		PUSH EAX
		MOV EAX, 01D0A050h
		CALL EAX
		ADD ESP, 18h
		RETN
	}
}
//===================================================================================================
NAKED void pfnGetPlayerInfo(int ent_num, hud_player_info_t* pinfo)
{
	__asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 08h]
		LEA ECX, DWORD PTR SS : [ESP + 04h]
		PUSH EAX
		PUSH ECX
		//CALL DWORD PTR DS:[1ECD44C]              ; hl.01D63590
		MOV ECX, DWORD PTR SS : [ESP + 0Ch]
		MOV EAX, DWORD PTR DS : [02F599C4h]
		ADD ESP, 08h
		CMP ECX, EAX
		JG label_01
		CMP ECX, 01h
		JL label_01
		LEA EAX, DWORD PTR DS : [ECX + ECX * 02h]
		SHL EAX, 04h
		ADD EAX, ECX
		LEA EAX, DWORD PTR DS : [EAX + EAX * 02h]
		LEA EAX, DWORD PTR DS : [EAX * 04h + 02F5A47Ch]
		TEST EAX, EAX
		JE label_01
		CMP BYTE PTR DS : [EAX], 00h
		JE label_01
		DEC ECX
		MOV DWORD PTR SS : [ESP + 04h], ECX
		LEA EAX, DWORD PTR DS : [ECX + ECX * 02h]
		SHL EAX, 04h
		ADD EAX, ECX
		MOV ECX, DWORD PTR SS : [ESP + 08h]
		LEA EDX, DWORD PTR DS : [EAX + EAX * 02h]
		LEA EAX, DWORD PTR DS : [EDX * 04h + 02F5A6C8h]
		MOV DWORD PTR DS : [ECX], EAX
		MOV ECX, DWORD PTR SS : [ESP + 04h]
		LEA EAX, DWORD PTR DS : [ECX + ECX * 02h]
		SHL EAX, 04h
		ADD EAX, ECX
		MOV ECX, DWORD PTR SS : [ESP + 08h]
		LEA EDX, DWORD PTR DS : [EAX + EAX * 02h]
		MOV AX, WORD PTR DS : [EDX * 04h + 02F5A6ECh]
		MOV WORD PTR DS : [ECX + 04h], AX
		MOV ECX, DWORD PTR SS : [ESP + 04h]
		LEA EAX, DWORD PTR DS : [ECX + ECX * 02h]
		SHL EAX, 04h
		ADD EAX, ECX
		MOV ECX, DWORD PTR SS : [ESP + 08h]
		LEA EDX, DWORD PTR DS : [EAX + EAX * 02h]
		MOV AL, BYTE PTR DS : [EDX * 04h + 02F5A6E8h]
		MOV BYTE PTR DS : [ECX + 07h], AL
		MOV ECX, DWORD PTR SS : [ESP + 04h]
		LEA EAX, DWORD PTR DS : [ECX + ECX * 02h]
		SHL EAX, 04h
		ADD EAX, ECX
		MOV ECX, DWORD PTR SS : [ESP + 08h]
		LEA EDX, DWORD PTR DS : [EAX + EAX * 02h]
		MOV AL, BYTE PTR DS : [EDX * 04h + 02F5A6F0h]
		MOV BYTE PTR DS : [ECX + 08h], AL
		MOV EDX, DWORD PTR SS : [ESP + 04h]
		MOV EAX, DWORD PTR DS : [02F5218Ch]
		MOV ECX, DWORD PTR SS : [ESP + 08h]
		CMP EDX, EAX
		SETE AL
		MOV BYTE PTR DS : [ECX + 06h], AL
		MOV ECX, DWORD PTR SS : [ESP + 04h]
		LEA EAX, DWORD PTR DS : [ECX + ECX * 02h]
		SHL EAX, 04h
		ADD EAX, ECX
		MOV ECX, DWORD PTR SS : [ESP + 08h]
		LEA EDX, DWORD PTR DS : [EAX + EAX * 02h]
		MOV AX, WORD PTR DS : [EDX * 04h + 02F5A734h]
		MOV WORD PTR DS : [ECX + 10h], AX
		MOV ECX, DWORD PTR SS : [ESP + 04h]
		LEA EAX, DWORD PTR DS : [ECX + ECX * 02h]
		SHL EAX, 04h
		ADD EAX, ECX
		MOV ECX, DWORD PTR SS : [ESP + 08h]
		LEA EDX, DWORD PTR DS : [EAX + EAX * 02h]
		MOV AX, WORD PTR DS : [EDX * 04h + 02F5A738h]
		MOV WORD PTR DS : [ECX + 12h], AX
		MOV ECX, DWORD PTR SS : [ESP + 04h]
		LEA EAX, DWORD PTR DS : [ECX + ECX * 02h]
		SHL EAX, 04h
		ADD EAX, ECX
		MOV ECX, DWORD PTR SS : [ESP + 08h]
		LEA EDX, DWORD PTR DS : [EAX + EAX * 02h]
		LEA EAX, DWORD PTR DS : [EDX * 04h + 02F5A6F4h]
		MOV DWORD PTR DS : [ECX + 0Ch], EAX
		RETN

		label_01 :

		MOV EDX, DWORD PTR SS : [ESP + 08h]
			MOV DWORD PTR DS : [EDX], 00h
			MOV EAX, DWORD PTR SS : [ESP + 08h]
			MOV BYTE PTR DS : [EAX + 06h], 00h
			RETN
	}
}
//===================================================================================================
NAKED int pfnGetScreenInfo(SCREENINFO* pscrinfo)
{
	__asm
	{
		LEA EAX, DWORD PTR SS : [ESP + 04h]
		PUSH EAX
		//CALL DWORD PTR DS:[1ECD428]              ; hl.01D63590
		MOV EAX, DWORD PTR SS : [ESP + 08h]
		ADD ESP, 04h
		TEST EAX, EAX
		JNZ label_01
		RETN

		label_01 :

		CMP DWORD PTR DS : [EAX], 0214h
			JE label_02
			XOR EAX, EAX
			RETN

			label_02 :

		MOV ECX, DWORD PTR DS : [0280E13Ch]
			MOV DWORD PTR DS : [EAX + 04h], ECX
			MOV EDX, DWORD PTR SS : [ESP + 04h]
			MOV EAX, DWORD PTR DS : [0280E140h]
			MOV DWORD PTR DS : [EDX + 08h], EAX
			MOV ECX, DWORD PTR SS : [ESP + 04h]
			MOV DWORD PTR DS : [ECX + 0Ch], 01h
			MOV EAX, 01D09CB0h
			CALL EAX
			MOV EDX, DWORD PTR SS : [ESP + 04h]
			PUSH EAX
			ADD EDX, 14h
			PUSH EDX
			MOV EAX, 01D0A250h
			CALL EAX
			MOV ECX, DWORD PTR SS : [ESP + 0Ch]
			ADD ESP, 08h
			MOV DWORD PTR DS : [ECX + 10h], EAX
			MOV EAX, 0214h
			RETN
	}
}
//===================================================================================================
NAKED int WorldToScreen(float* world, float* screen)
{
	__asm
	{
		PUSH ECX
		MOV EAX, DWORD PTR SS : [ESP + 08h]
		MOV ECX, DWORD PTR SS : [ESP + 0Ch]
		FLD DWORD PTR DS : [02C20100h]
		FMUL DWORD PTR DS : [EAX]
		FLD DWORD PTR DS : [02C20120h]
		FMUL DWORD PTR DS : [EAX + 08h]
		FADDP ST(1), ST
		FLD DWORD PTR DS : [02C20110h]
		FMUL DWORD PTR DS : [EAX + 04h]
		FADDP ST(1), ST
		FADD DWORD PTR DS : [02C20130h]
		FSTP DWORD PTR SS : [ESP]
		FLD DWORD PTR DS : [02C20104h]
		FMUL DWORD PTR DS : [EAX]
		FLD DWORD PTR DS : [02C20124h]
		FMUL DWORD PTR DS : [EAX + 08h]
		MOV EDX, DWORD PTR SS : [ESP]
		MOV DWORD PTR DS : [ECX], EDX
		FADDP ST(1), ST
		FLD DWORD PTR DS : [02C20114h]
		FMUL DWORD PTR DS : [EAX + 04h]
		FADDP ST(1), ST
		FADD DWORD PTR DS : [02C20134h]
		FSTP DWORD PTR SS : [ESP + 0Ch]
		FLD DWORD PTR DS : [02C2010Ch]
		FMUL DWORD PTR DS : [EAX]
		FLD DWORD PTR DS : [02C2012Ch]
		FMUL DWORD PTR DS : [EAX + 08h]
		MOV EDX, DWORD PTR SS : [ESP + 0Ch]
		MOV DWORD PTR DS : [ECX + 04h], EDX
		FADDP ST(1), ST
		FLD DWORD PTR DS : [02C2011Ch]
		FMUL DWORD PTR DS : [EAX + 04h]
		FADDP ST(1), ST
		FADD DWORD PTR DS : [02C2013Ch]
		FSTP DWORD PTR SS : [ESP + 08h]
		FLD DWORD PTR SS : [ESP + 08h]
		FCOMP DWORD PTR DS : [01E8708Ch]
		FSTSW AX
		TEST AH, 44h
		JPO label_01
		FLD DWORD PTR SS : [ESP + 08h]
		FDIVR QWORD PTR DS : [01E87540h]
		FSTP DWORD PTR SS : [ESP + 08h]
		FLD DWORD PTR SS : [ESP]
		FMUL DWORD PTR SS : [ESP + 08h]
		FSTP DWORD PTR DS : [ECX]
		FLD DWORD PTR SS : [ESP + 0Ch]
		FMUL DWORD PTR SS : [ESP + 08h]
		FSTP DWORD PTR DS : [ECX + 04h]

		label_01 :

				 FLD DWORD PTR SS : [ESP + 08h]
				 FCOMP DWORD PTR DS : [01E8708Ch]
				 FSTSW AX
				 TEST AH, 41h
				 JPE label_02
				 MOV EAX, 01h
				 POP ECX
				 RETN

				 label_02 :

		XOR EAX, EAX
			POP ECX
			RETN
	}
}
//===================================================================