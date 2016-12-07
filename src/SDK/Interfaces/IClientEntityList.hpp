#pragma once

#define VCLIENTENTITYLIST_INTERFACE_VERSION "VClientEntityList003"

class IClientEntity;
class IClientUnknown;
class IClientNetworkable;

class IClientEntityList {
	public:
		virtual IClientNetworkable* GetClientNetworkable(int entindex) = 0;
		virtual IClientNetworkable* GetClientNetworkableFromHandle(CBaseHandle entity_handle) = 0;
		virtual IClientUnknown* GetClientUnknownFromHandle(CBaseHandle entity_handle) = 0;
		virtual IClientEntity* GetClientEntity(int entindex) = 0;
		virtual IClientEntity* GetClientEntityFromHandle(CBaseHandle entity_handle) = 0;
		virtual int NumberOfEntities(bool include_non_networkable) = 0;
		virtual int GetHighestEntityIndex(void) = 0;
		virtual void SetMaxEntities(int maxents) = 0;
		virtual int GetMaxEntities() = 0;
};

extern IClientEntityList* entitylist;