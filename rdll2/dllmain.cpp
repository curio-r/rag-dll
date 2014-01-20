// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <string>
#include <stdlib.h>

#include "hooks.h"

BOOL CALLBACK DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{

	if(dwReason != DLL_PROCESS_ATTACH)
		return TRUE;

	HANDLE process = GetCurrentProcess();
	FARPROC addr;
	
	IMAGE_DOS_HEADER* IDH;
	IMAGE_NT_HEADERS * INT;
	
	BOOL ret = FALSE;

	/* Verify NT_HEADER time stamp match */

	IDH = (IMAGE_DOS_HEADER*)GetModuleHandle(NULL);
	INT = (IMAGE_NT_HEADERS *)((int *)IDH + (int)(IDH->e_lfanew/4));
	
	if(INT->FileHeader.TimeDateStamp != CLIENT_TIMESTAMP) {
		return TRUE;
	}

	/* Parse config file */

	G_CONFDATA = parseConfFile();
	
	if(G_CONFDATA == NULL)
		return TRUE;

	/* Install hooks */

	for(UINT i = 0; i * sizeof(HOOK_TABLE_ENTRY) < sizeof(HOOK_MAP); i++ ) {
		
		addr = GetProcAddress(GetModuleHandle(TEXT("rdll2.asi")), HOOK_MAP[i].ext_name);

		if(addr == NULL) {
			MessageBoxA(NULL, "Failed to get proc address. Aborting.", "CWeatherPlugin", MB_OK);
			break;
		}

		ret = HOOK_ADDR(process, HOOK_MAP[i].entry_addr, addr);
		
		if(!ret) {
			MessageBoxA(NULL, "Couldn't apply memory patch. Plugin will not work and client may crash.", "CWeatherPlugin", MB_OK);
			break;
		}

	}

    return TRUE;
}
