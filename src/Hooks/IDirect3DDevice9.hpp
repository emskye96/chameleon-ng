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

	// TODO: Disable 'cl_mouseenable' when GUI is active.
	ImGui::GetIO().MouseDrawCursor = renderer.IsActive();
	
	// Call the original when the GUI isn't ready or is inactive.
	if (!renderer.IsReady() || !renderer.IsActive())
		return oEndScene(thisptr);

	ImGui_ImplDX9_NewFrame();
	
	// Example editor for default CT knife. (TODO: move to GUI file?)
	EconomyItem_t& knife = config.GetWeaponConfiguration(WEAPON_KNIFE);
	knife.is_valid = true;

	ImGui::InputText("Name Tag", knife.custom_name, 32); 
	ImGui::InputInt("Item Index", &knife.item_definition_index);
	ImGui::InputInt("Paint Kit", &knife.fallback_paint_kit);
	ImGui::InputInt("Seed", &knife.fallback_seed);
	ImGui::InputInt("Quality", &knife.entity_quality);
	ImGui::InputInt("StatTrak", &knife.fallback_stattrak);
	ImGui::InputFloat("Wear", &knife.fallback_wear);

	ImGui::Render();

	// Finally, call the original function.
	return oEndScene(thisptr);
}