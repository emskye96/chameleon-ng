#pragma once

#define VENGINE_CLIENT_INTERFACE_VERSION "VEngineClient014"

#define MAX_PLAYER_NAME_LENGTH 32
#define SIGNED_GUID_LEN 32

typedef struct player_info_s {
	private:
		int64_t unknown;
	public:
		union {
			int64_t xuid;
			struct {
				int xuid_low;
				int xuid_high;
			};
		};
		char name[MAX_PLAYER_NAME_LENGTH + 96];
		int userid;
		int m_nUserID;
		char guid[SIGNED_GUID_LEN + 1];
		unsigned int friendsid;
		char friendsname[MAX_PLAYER_NAME_LENGTH + 96];
		bool fakeplayer;
		bool ishltv;
		unsigned int customfiles[4];
		unsigned char filesdownloaded;
} player_info_t;

class IVEngineClient {
	public:
		inline bool GetPlayerInfo(int Index, player_info_t* PlayerInfo) {
			return GetVirtualFunction<bool(__thiscall *)(void *, int, player_info_t*)>(this, 8)(this, Index, PlayerInfo);
		}

		inline int GetPlayerForUserID(int UserID) {
			return GetVirtualFunction<bool(__thiscall *)(void *, int)>(this, 9)(this, UserID);
		}

		inline int GetLocalPlayer() {
			return GetVirtualFunction<int(__thiscall *)(void *)>(this, 12)(this);
		}
};

extern IVEngineClient* engine;