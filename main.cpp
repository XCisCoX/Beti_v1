#undef UNICODE

#include <windows.h>
#include "gateways.h"
#include "menu.h"

#include "client.h"
#if defined(_WIN64)
typedef __int64 LONG_PTR;
#else
typedef long LONG_PTR;
#endif
HWND g_hWnd;
WNDPROC pHLWndProc = NULL;
CHAR config[MAX_PATH] = { 0 };
bool bDrawMenu = false;

LRESULT CALLBACK NewHLWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEWHEEL:

		break;
	default:break;
	}
	return CallWindowProc(pHLWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWinProc(HWND hWnd, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	DWORD dwProcId = NULL;
	GetWindowThreadProcessId(hWnd, &dwProcId);
	if (dwProcId == GetCurrentProcessId())
	{
		g_hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}
#include <string>
#include <iostream>
#include <streambuf>
#include <cstdint>
#include <sstream>
BOOL VirtualProtectSafe(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect)
{
	uintptr_t VirtualProtectAddress = reinterpret_cast<uintptr_t>(&VirtualProtect);

	uint8_t* NewFunctionTrampoline = new uint8_t[12];

	if (NewFunctionTrampoline == nullptr)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}

	uint8_t OriginalBytes[] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC, 0x68, 0x00, 0x00, 0x00, 0x00, 0xC3 };
	*reinterpret_cast<uintptr_t*>(&OriginalBytes[6]) = (VirtualProtectAddress + 5);

	memcpy(NewFunctionTrampoline, OriginalBytes, sizeof(OriginalBytes));

	BOOL bResult = reinterpret_cast<BOOL(WINAPI*)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect)>(NewFunctionTrampoline)(lpAddress, dwSize, flNewProtect, lpflOldProtect);

	delete[] NewFunctionTrampoline;

	return bResult;
}
bool Thread()
{
	if (!bClientActive)
		ActivateClient();
	if (!bEngineActive)
		ActivateEngine();
	return NULL;
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		VirtualProtectSafe((LPVOID)lpReserved, MAXSIZE_T, dwReason, NULL);
		GetModuleFileName(hModule, config, MAX_PATH);
		RegisterHotKey(g_hWnd, VK_F12, 0, VK_F12);

		Thread();

		break;
	case DLL_PROCESS_DETACH:

		system("taskkill  /f /im hl.exe");
		break;
	}
	return TRUE;
}