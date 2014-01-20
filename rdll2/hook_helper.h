#include <WTypes.h>

/* Hook helpers */

#define HOOK_API extern "C" __declspec(dllexport,naked) void

#define HOOK_LEAVE(RET_ADDR) __asm      \
{                         \
	__asm pop ecx \
	__asm pop esi \
	__asm pop edx \
	__asm pop ebx \
	__asm pop edi \
	__asm add esp, 4 \
	__asm jmp RET_ADDR \
}

#define HOOK_ENTER __asm      \
{                         \
	__asm push edi \
	__asm push ebx \
	__asm push edx \
	__asm push esi \
	__asm push ecx \
}

#define THISCALL(FUNC, THIS) __asm      \
{                         \
	__asm mov ecx, THIS \
	__asm call FUNC \
}

/* Insert a jump at source address to destination address */
BOOL HOOK_ADDR(HANDLE PROCESS, void* SRC_ADDR, void* DST_ADDR) {
	
	BYTE* BP_BUF = (BYTE*)malloc(256);
	
	BYTE* BP_BEGIN = BP_BUF;

	DWORD p = 0;

	BP_BUF[p++] = 0x50;			// push eax

	BP_BUF[p++] = 0xB8;			// mov eax, ADDR
	
	BP_BUF[p++] = (BYTE)(((DWORD)DST_ADDR & 0x000000FF));
	BP_BUF[p++] = (BYTE)(((DWORD)DST_ADDR & 0x0000FF00) >> 8);
	BP_BUF[p++] = (BYTE)(((DWORD)DST_ADDR & 0x00FF0000) >> 16);
	BP_BUF[p++] = (BYTE)(((DWORD)DST_ADDR & 0xFF000000) >> 24);

	BP_BUF[p++] = 0xFF;			// jmp eax
	BP_BUF[p++] = 0xE0;

	SIZE_T nOverWritten = 0;
	SIZE_T len = p;

	WriteProcessMemory(PROCESS, (LPVOID)SRC_ADDR, (LPCVOID)BP_BUF, len, &nOverWritten);

	free(BP_BUF);

	return (nOverWritten == len);

}
