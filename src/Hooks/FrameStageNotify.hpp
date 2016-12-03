#pragma once

// Function signature of IBaseClientDLL::FrameStageNotify.
typedef void (__thiscall *FrameStageNotify_t) (IBaseClientDLL*, ClientFrameStage_t);

// Replacement function that will get called at various points during frame rendering.
void __fastcall hkFrameStageNotify(IBaseClientDLL* thisptr, void* edx, ClientFrameStage_t stage) {
	// Get the original function and store it in a static variable for later usage.
	static FrameStageNotify_t oFrameStageNotify = clientdll_hook->GetOriginalFunction<FrameStageNotify_t>(36);

	// Only perform our replacements during the PostDataUpdate start stage.
	while (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		// Placeholder code (practically) verbatim from aixxe's Linux skin changer.
		int localplayer_idx = engine->GetLocalPlayer();
		C_BasePlayer* localplayer = reinterpret_cast<C_BasePlayer*>(entitylist->GetClientEntity(localplayer_idx));

		if (!localplayer || localplayer->GetLifeState() != LIFE_ALIVE)
			break;

		player_info_t localplayer_info;

		if (!engine->GetPlayerInfo(localplayer_idx, &localplayer_info))
			break;

		CBaseHandle* weapons = localplayer->GetWeapons();

		for (size_t i = 0; i < weapons[i]; i++) {
			C_BaseAttributableItem* weapon = reinterpret_cast<C_BaseAttributableItem*>(entitylist->GetClientEntityFromHandle(weapons[i]));
			
			if (!weapon)
				continue;

			switch (*weapon->GetItemDefinitionIndex()) {
				case WEAPON_AWP:
					*weapon->GetFallbackPaintKit() = 344; break;
				case WEAPON_KNIFE:
					*weapon->GetItemDefinitionIndex() = WEAPON_KNIFE_KARAMBIT;
					*weapon->GetModelIndex() = modelinfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
					*weapon->GetViewModelIndex() = modelinfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
					*weapon->GetFallbackPaintKit() = 409; break;
				case WEAPON_KNIFE_T:
					*weapon->GetItemDefinitionIndex() = WEAPON_KNIFE_M9_BAYONET;
					*weapon->GetModelIndex() = modelinfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
					*weapon->GetViewModelIndex() = modelinfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
					*weapon->GetFallbackPaintKit() = 12; break;
			}

			snprintf(weapon->GetCustomName(), 32, "%s", "chameleon-ng");

			*weapon->GetFallbackWear() = 0.f;
			*weapon->GetEntityQuality() = 9;
			*weapon->GetFallbackStatTrak() = 133337;
			*weapon->GetAccountID() = localplayer_info.xuid_low;
			*weapon->GetItemIDHigh() = -1;
		}

		C_BaseViewModel* viewmodel = reinterpret_cast<C_BaseViewModel*>(entitylist->GetClientEntityFromHandle(localplayer->GetViewModel()));

		if (!viewmodel)
			break;

		C_BaseAttributableItem* active_weapon = reinterpret_cast<C_BaseAttributableItem*>(entitylist->GetClientEntityFromHandle(viewmodel->GetWeapon()));

		if (!active_weapon)
			break;

		switch (*active_weapon->GetItemDefinitionIndex()) {
			case WEAPON_KNIFE_KARAMBIT:
				*viewmodel->GetModelIndex() = modelinfo->GetModelIndex("models/weapons/v_knife_karam.mdl"); break;
			case WEAPON_KNIFE_M9_BAYONET:
				*viewmodel->GetModelIndex() = modelinfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl"); break;
		}

		break;
	}
	
	// Call the original function.
	return oFrameStageNotify(thisptr, stage);
}