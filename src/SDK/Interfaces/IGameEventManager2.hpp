#pragma once

#define INTERFACEVERSION_GAMEEVENTSMANAGER2 "GAMEEVENTSMANAGER002"

struct bf_read;
struct bf_write;

class IGameEvent;
class KeyValues;

class IGameEventListener2 {
	public:
		virtual ~IGameEventListener2(void) {};
		virtual void FireGameEvent(IGameEvent* event) = 0;
		virtual int GetEventDebugID(void) = 0;
};

class IGameEventManager2 {
	public:
		virtual ~IGameEventManager2(void) {};
		virtual int LoadEventsFromFile(const char* filename) = 0;
		virtual void Reset() = 0;
		virtual bool AddListener(IGameEventListener2* listener, const char* name, bool serverside) = 0;
		virtual bool FindListener(IGameEventListener2* listener, const char* name) = 0;
		virtual void RemoveListener(IGameEventListener2* listener) = 0;
		virtual void AddListenerGlobal(IGameEventListener2* listener, bool serverside) = 0;
		virtual IGameEvent* CreateEvent(const char* name, bool force = false, int* cookie = nullptr) = 0;
		virtual bool FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0;
		virtual bool FireEventClientSide(IGameEvent* event) = 0;
		virtual IGameEvent* DuplicateEvent(IGameEvent* event) = 0;
		virtual void FreeEvent(IGameEvent* event) = 0;
		virtual bool SerializeEvent(IGameEvent* event, bf_write* buffer) = 0;
		virtual IGameEvent* UnserializeEvent(bf_read* buffer) = 0;
		virtual KeyValues* GetEventDataTypes(IGameEvent* event) = 0;
};

extern IGameEventManager2* gameevents;