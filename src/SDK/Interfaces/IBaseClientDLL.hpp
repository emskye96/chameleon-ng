#pragma once

#define CLIENT_DLL_INTERFACE_VERSION "VClient018"

enum ClientFrameStage_t: int {
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class ClientClass;

class IBaseClientDLL {
	public:
		inline ClientClass* GetAllClasses() {
			return GetVirtualFunction<ClientClass*(__thiscall *)(IBaseClientDLL*)>(this, 8)(this);
		}
};

extern IBaseClientDLL* clientdll;