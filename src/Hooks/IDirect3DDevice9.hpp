#pragma once

#include <d3d9.h>

// Function signature of IDirect3DDevice9::EndScene.
typedef HRESULT(__stdcall *EndScene_t) (IDirect3DDevice9*);

// Function signature of IDirect3DDevice9::Reset.
typedef HRESULT(__stdcall *Reset_t) (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

// Replacement function that will get called when the device is reset.
HRESULT __stdcall hkReset(IDirect3DDevice9* thisptr, D3DPRESENT_PARAMETERS* params) {
	// Get the original function and store it in a static variable for later usage.
	static Reset_t oReset = d3d9_hook->GetOriginalFunction<Reset_t>(16);
	
	// Call the original function.
	return oReset(thisptr, params);
}

// Replacement function that will get called before the scene is rendered.
HRESULT __stdcall hkEndScene(IDirect3DDevice9* thisptr) {
	// Get the original function and store it in a static variable for later usage.
	static EndScene_t oEndScene = d3d9_hook->GetOriginalFunction<EndScene_t>(42);
	
	// Call the original function.
	return oEndScene(thisptr);
}