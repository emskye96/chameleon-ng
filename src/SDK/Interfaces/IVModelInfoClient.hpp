#pragma once

#define VMODELINFO_CLIENT_INTERFACE_VERSION "VModelInfoClient004"

struct model_t;

class IVModelInfoClient {
	public:
		virtual ~IVModelInfoClient (void) {};
		virtual const model_t* GetModel(int modelindex) const = 0;
		virtual int GetModelIndex(const char* name) const = 0;
		virtual const char* GetModelName(const model_t* model) const = 0;
};

extern IVModelInfoClient* modelinfo;