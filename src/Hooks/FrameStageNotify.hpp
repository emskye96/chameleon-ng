#pragma once

// Function signature of IBaseClientDLL::FrameStageNotify.
typedef void (__thiscall *FrameStageNotify_t) (IBaseClientDLL*, ClientFrameStage_t);

// Placeholder function to get the model filename by item index.
inline const char* GetModelByItemIndex(int id) {
	switch (id) {
		case WEAPON_KNIFE_GUT:
			return "models/weapons/v_knife_gut.mdl";
		case WEAPON_KNIFE_FLIP:
			return "models/weapons/v_knife_flip.mdl";
		case WEAPON_KNIFE_BAYONET:
			return "models/weapons/v_knife_bayonet.mdl";
		case WEAPON_KNIFE_M9_BAYONET:
			return "models/weapons/v_knife_m9_bay.mdl";
		case WEAPON_KNIFE_KARAMBIT:
			return "models/weapons/v_knife_karam.mdl";
		case WEAPON_KNIFE_TACTICAL:
			return "models/weapons/v_knife_tactical.mdl";
		case WEAPON_KNIFE_BUTTERFLY:
			return "models/weapons/v_knife_butterfly.mdl";
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
			return "models/weapons/v_knife_survival_bowie.mdl";
		case WEAPON_KNIFE_FALCHION:
			return "models/weapons/v_knife_falchion_advanced.mdl";
		case WEAPON_KNIFE_PUSH:
			return "models/weapons/v_knife_push.mdl";
		default:
			return NULL;
	}
}

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

			unsigned int* item_definition_index = weapon->GetItemDefinitionIndex();

			if (config.HasWeaponConfiguration(*item_definition_index)) {
				EconomyItem_t& weapon_config = config.GetWeaponConfiguration(*item_definition_index);

				if (weapon_config.custom_name)
					snprintf(weapon->GetCustomName(), 32, "%s", weapon_config.custom_name);

				if (weapon_config.entity_quality != -1)
					*weapon->GetEntityQuality() = weapon_config.entity_quality;

				if (weapon_config.fallback_wear != -1)
					*weapon->GetFallbackWear() = weapon_config.fallback_wear;

				if (weapon_config.fallback_stattrak != -1)
					*weapon->GetFallbackStatTrak() = weapon_config.fallback_stattrak;

				if (weapon_config.fallback_seed != -1)
					*weapon->GetFallbackSeed() = weapon_config.fallback_seed;

				if (weapon_config.fallback_paint_kit != -1)
					*weapon->GetFallbackPaintKit() = weapon_config.fallback_paint_kit;
				
				if (weapon_config.item_definition_index != -1)
					*item_definition_index = weapon_config.item_definition_index;

				const char* real_model = GetModelByItemIndex(weapon_config.item_definition_index);
				
				if (real_model)
					*weapon->GetModelIndex() = modelinfo->GetModelIndex(real_model);

				*weapon->GetAccountID() = localplayer_info.xuid_low;
				*weapon->GetItemIDHigh() = -1;
			}
		}

		C_BaseViewModel* viewmodel = reinterpret_cast<C_BaseViewModel*>(entitylist->GetClientEntityFromHandle(localplayer->GetViewModel()));

		if (!viewmodel)
			break;

		C_BaseAttributableItem* active_weapon = reinterpret_cast<C_BaseAttributableItem*>(entitylist->GetClientEntityFromHandle(viewmodel->GetWeapon()));

		if (!active_weapon)
			break;

		// Set appropriate viewmodel index based on item definition index.
		const char* real_model = GetModelByItemIndex(*active_weapon->GetItemDefinitionIndex());

		if (real_model)
			*viewmodel->GetModelIndex() = modelinfo->GetModelIndex(real_model);

		break;
	}
	
	// Call the original function.
	return oFrameStageNotify(thisptr, stage);
}