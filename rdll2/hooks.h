#include "client.h"
#include "hook_helper.h"
#include "parser.h"

struct HOOK_TABLE_ENTRY {
	char* ext_name;
	void* entry_addr;
};

/* Add more entries to install new hooks */
struct HOOK_TABLE_ENTRY HOOK_MAP[] = {
	{ "Set_CWeather_Enter", CGameMode_Initialize_EntryPtr },
	{ "Set_ClearColor_Enter", CGameMode_OnInit_EntryPtr },
};

/* Helper function to intialize CWeather effect assigned by config */
__declspec(cdecl) void Set_CWeather(char* mapName) {
	
	if(*(int*)G_USEEFFECT == 0) {
		return;
	}

	mapConf* mc = getMapConfig(mapName);

	DWORD param = (DWORD)mc->param;
	
	DWORD CWeather_InitFunc = CWeather_EffectId2LaunchFuncAddr[(UINT)mc->effectType];

	if(CWeather_InitFunc != NULL) {

		if(mc->effectType == CEFFECT_SKY) 
			__asm push param
		THISCALL(CWeather_InitFunc, G_WEATHER);

	}

	return;

}

/* Entry point from CGameMode::Initialize */
HOOK_API Set_CWeather_Enter() {

	HOOK_ENTER;

	__asm {
		push ecx // CGameSession->m_rswName
		call Set_CWeather
		add esp, 4
	}

	HOOK_LEAVE(CGameMode_Initialize_RetPtr);

}

/* Helper function to set clear color from map config */
__declspec(cdecl) void Set_ClearColor(char* mapname) {

	// G_CRENDERER->clearColor
	*(DWORD*)((DWORD*)*G_CRENDERER + 31) = getMapConfig(mapname)->clearColor;
	
}

/* Entry point from CGameMode::OnInit */
HOOK_API Set_ClearColor_Enter() {

	HOOK_ENTER;

	__asm {
		push ecx // CGameSession->m_rswName
		call Set_ClearColor
		add esp, 4
	}

	HOOK_LEAVE(CGameMode_OnInit_RetPtr);

}

// Ignore this for now...

/*
void* G_MODEMGR = (void**)0x008E2958;
void (*GModeMgr_GetGameMode)(void) = (void(*)(void))(0x00588350);
void (*CAbleToMakeEffect_LaunchEffect)(void) = (void(*)(void))(0x00705E70);

void CWeather_LaunchBackGroundTexture(void) {

	float vector3[3];

	void* gameMode = 0;

	char str[255];

	_asm {
		mov ecx, G_MODEMGR
		call GModeMgr_GetGameMode
		mov gameMode, eax
	}

	void* m_world = (void*)*((DWORD*)gameMode + 208); // CWorld*
	
	MessageBoxA(NULL, "3", "", MB_OK);
	_itoa((DWORD)m_world, str, 16);
	MessageBoxA(NULL, str, "", MB_OK);
	
	void* m_player = (void*)*((DWORD*)m_player + 60); // CPlayer*

	MessageBoxA(NULL, "4", "", MB_OK);

	void* player_cAbleToMakeEffect = (void*)m_player; // *(void **)(*(DWORD *)((DWORD*)gameMode + 208) + 60);

	// G_WEATHER->m_Player = cAbleToMakeEffect
	*((DWORD *)G_WEATHER + 5) = (DWORD)player_cAbleToMakeEffect;
	
	// *(_DWORD *)(v2 + 20) = v3; this->m_Effect

	_asm {
		
		mov ecx, player_cAbleToMakeEffect // this = ecx

		push 0 // float fRot

		push 0 // vector3d fDeltaPos
		push 0
		push 0

		push 703 // effect ID

		call CAbleToMakeEffect_LaunchEffect

	}

	return;

}
*/
