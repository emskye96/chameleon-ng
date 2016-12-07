#pragma once

#include <d3d9.h>

#include "../Interface.hpp"

// Function signature of IDirect3DDevice9::EndScene.
typedef HRESULT (__stdcall *EndScene_t) (IDirect3DDevice9*);

// Function signature of IDirect3DDevice9::Reset.
typedef HRESULT (__stdcall *Reset_t) (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

// Replacement function that will get called when the device is reset.
HRESULT __stdcall hkReset(IDirect3DDevice9* thisptr, D3DPRESENT_PARAMETERS* params) {
	// Get the original function and store it in a static variable for later usage.
	static Reset_t oReset = d3d9_hook->GetOriginalFunction<Reset_t>(16);
	
	// Nothing special to do until our renderer is ready.
	if (!renderer.IsReady())
		return oReset(thisptr, params);

	// Destroy any ImGui related resources..
	ImGui_ImplDX9_InvalidateDeviceObjects();
	
	// ..call the original 'Reset' function..
	HRESULT result = oReset(thisptr, params);
	
	// ..then recreate them.
	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}

// Replacement function that will get called before the scene is rendered.
HRESULT __stdcall hkEndScene(IDirect3DDevice9* thisptr) {
	// Get the original function and store it in a static variable for later usage.
	static EndScene_t oEndScene = d3d9_hook->GetOriginalFunction<EndScene_t>(42);

	// Determines whether the mouse is enabled in-game.
	static bool mouse_enabled = false;

	// Check whether the GUI is currently open.
	bool is_renderer_active = renderer.IsActive();

	if (is_renderer_active) {
		if (mouse_enabled) {
			// Disable the mouse while the menu is open.
			engine->ClientCmd_Unrestricted("cl_mouseenable 0");
			mouse_enabled = false;
		}
	} else {
		if (!mouse_enabled) {
			// Re-enable the mouse while the menu is closed.
			engine->ClientCmd_Unrestricted("cl_mouseenable 1");
			mouse_enabled = true;
		}
	}
	
	// Enable the in-built cursor while the GUI is active.
	ImGui::GetIO().MouseDrawCursor = is_renderer_active;
	
	// Don't do anything else if the GUI is inactive.
	if (!is_renderer_active)
		return oEndScene(thisptr);

	// Let ImGui know we've started a new frame.
	ImGui_ImplDX9_NewFrame();
	
	// Queue up draw calls for the interface.
	RenderInterface();

	// Render everything in the draw lists.
	ImGui::Render();

	// Finally, call the original function.
	return oEndScene(thisptr);
}