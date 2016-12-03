#pragma once

#include <psapi.h>

inline bool Compare(const unsigned char* pData, const unsigned char* bMask, const char* szMask) {
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;

	return (*szMask) == 0;
}

uintptr_t FindPattern(const char* szModule, unsigned char* bMask, const char* szMask) {
	MODULEINFO module_info = {0};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &module_info, sizeof(MODULEINFO));

	DWORD module_start = DWORD(module_info.lpBaseOfDll);

	for (uintptr_t i = 0; i < module_info.SizeOfImage; i++)
		if (Compare((unsigned char*)(module_start + i), bMask, szMask))
			return (uintptr_t)(module_start + i);

	return 0;
}