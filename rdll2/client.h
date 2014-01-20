#include <WTypes.h>

// Client build: Mon, 09 Apr 2012 19:14:49 GMT
#define CLIENT_TIMESTAMP 1333998889

// Client offsets
void* G_WEATHER = (void*)0x00971BA0;
void** G_CRENDERER = (void**)0x008F3DD8;
void* G_USEEFFECT = (void*)0x009A0EEC;

DWORD CWeather_EffectId2LaunchFuncAddr[] = {
	NULL, // CEFFECT_NONE
	0x006C6980, // CEFFECT_SKY -> void CWeather::LaunchCloud(CWeather this<ecx>, char param)
	0x006C6660, // CEFFECT_SNOW -> void CWeather::LaunchSnow(CWeather this<ecx>)
	0x006C68C0, // CEFFECT_MAPLE -> void CWeather::LaunchMaple(CWeather this<ecx>)
	0x006C6840, // CEFFECT_SAKURA -> void CWeather::LaunchSakura(CWeather this<ecx>)
	0x006C6B30, // CEFFECT_POKJUK -> void CWeather::LaunchPokJuk(CWeather this<ecx>)
	0x006C6BE0, // CEFFECT_NIGHT -> void CWeather::LaunchNight(CWeather this<ecx>)
};

void* CGameMode_Initialize_EntryPtr = (void*)0x0072AB61;
void* CGameMode_Initialize_RetPtr = (void*)0x0072B448;
void* CGameMode_OnInit_EntryPtr = (void*)0x007337ED;
void* CGameMode_OnInit_RetPtr = (void*)0x00734008;
