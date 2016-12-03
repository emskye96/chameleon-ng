#pragma once

#define	LIFE_ALIVE 0

extern NetVars netvars;

class C_BaseEntity: public IClientEntity {
	public:
		int* GetModelIndex() {
			static uintptr_t m_nModelIndex = netvars.GetOffset("CBaseEntity", "m_nModelIndex");
			return reinterpret_cast<int*>(uintptr_t(this) + m_nModelIndex);
		}
};

class C_BasePlayer: public C_BaseEntity {
	public:
		unsigned char GetLifeState() {
			static uintptr_t m_lifeState = netvars.GetOffset("CBasePlayer", "m_lifeState");
			return *reinterpret_cast<unsigned char*>(uintptr_t(this) + m_lifeState);
		}

		CBaseHandle* GetWeapons() {
			static uintptr_t m_hMyWeapons = netvars.GetOffset("CBasePlayer", "m_hMyWeapons");
			return reinterpret_cast<CBaseHandle*>(uintptr_t(this) + m_hMyWeapons);
		}

		CBaseHandle GetViewModel() {
			static uintptr_t m_hViewModel = netvars.GetOffset("CBasePlayer", "m_hViewModel[0]");
			return *reinterpret_cast<CBaseHandle*>(uintptr_t(this) + m_hViewModel);
		}
};

class C_BaseCombatWeapon: public C_BaseEntity {
	public:
		int* GetViewModelIndex() {
			static uintptr_t m_iViewModelIndex = netvars.GetOffset("CBaseCombatWeapon", "m_iViewModelIndex");
			return reinterpret_cast<int*>(uintptr_t(this) + m_iViewModelIndex);
		}
};

class C_BaseAttributableItem: public C_BaseCombatWeapon{
	public:
		int* GetAccountID() {
			static uintptr_t m_iAccountID = netvars.GetOffset("CBaseAttributableItem", "m_iAccountID");
			return reinterpret_cast<int*>(uintptr_t(this) + m_iAccountID);
		}

		int* GetItemDefinitionIndex() {
			static uintptr_t m_iItemDefinitionIndex = netvars.GetOffset("CBaseAttributableItem", "m_iItemDefinitionIndex");
			return reinterpret_cast<int*>(uintptr_t(this) + m_iItemDefinitionIndex);
		}

		int* GetItemIDHigh() {
			static uintptr_t m_iItemIDHigh = netvars.GetOffset("CBaseAttributableItem", "m_iItemIDHigh");
			return reinterpret_cast<int*>(uintptr_t(this) + m_iItemIDHigh);
		}

		int* GetEntityQuality() {
			static uintptr_t m_iEntityQuality = netvars.GetOffset("CBaseAttributableItem", "m_iEntityQuality");
			return reinterpret_cast<int*>(uintptr_t(this) + m_iEntityQuality);
		}

		char* GetCustomName() {
			static uintptr_t m_szCustomName = netvars.GetOffset("CBaseAttributableItem", "m_szCustomName");
			return reinterpret_cast<char*>(uintptr_t(this) + m_szCustomName);
		}

		int* GetFallbackPaintKit() {
			static uintptr_t m_nFallbackPaintKit = netvars.GetOffset("CBaseAttributableItem", "m_nFallbackPaintKit");
			return reinterpret_cast<int*>(uintptr_t(this) + m_nFallbackPaintKit);
		}

		int* GetFallbackSeed() {
			static uintptr_t m_nFallbackSeed = netvars.GetOffset("CBaseAttributableItem", "m_nFallbackSeed");
			return reinterpret_cast<int*>(uintptr_t(this) + m_nFallbackSeed);
		}

		float* GetFallbackWear() {
			static uintptr_t m_flFallbackWear = netvars.GetOffset("CBaseAttributableItem", "m_flFallbackWear");
			return reinterpret_cast<float*>(uintptr_t(this) + m_flFallbackWear);
		}

		int* GetFallbackStatTrak() {
			static uintptr_t m_nFallbackStatTrak = netvars.GetOffset("CBaseAttributableItem", "m_nFallbackStatTrak");
			return reinterpret_cast<int*>(uintptr_t(this) + m_nFallbackStatTrak);
		}
};

class C_BaseViewModel: public C_BaseEntity {
	public:
		int GetWeapon() {
			static uintptr_t m_hWeapon = netvars.GetOffset("CBaseViewModel", "m_hWeapon");
			return *reinterpret_cast<int*>(uintptr_t(this) + m_hWeapon);
		}
};