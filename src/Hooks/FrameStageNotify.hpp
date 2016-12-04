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
				
				if (weapon_config.item_definition_index != -1) {
					if (ItemDefinitionIndex.find(weapon_config.item_definition_index) != ItemDefinitionIndex.end()) {
						*weapon->GetModelIndex() = modelinfo->GetModelIndex(ItemDefinitionIndex.at(weapon_config.item_definition_index).model);

						// Make sure the original item is in our definition list too.
						if (ItemDefinitionIndex.find(*item_definition_index) != ItemDefinitionIndex.end()) {
							const Item_t& original_item = ItemDefinitionIndex.at(*item_definition_index);
							const Item_t& replacement_item = ItemDefinitionIndex.at(weapon_config.item_definition_index);

							if (original_item.killicon && replacement_item.killicon)
								config.SetKillIconOverride(original_item.killicon, replacement_item.killicon);

							*item_definition_index = weapon_config.item_definition_index;
						}
					}
				}

				*weapon->GetAccountID() = localplayer_info.xuid_low;
				*weapon->GetItemIDHigh() = -1;
			}
		}

		// TODO: Really need to clean this up.
		if (C_BaseViewModel* viewmodel = reinterpret_cast<C_BaseViewModel*>(entitylist->GetClientEntityFromHandle(localplayer->GetViewModel()))) {
			if (C_BaseAttributableItem* active_weapon = reinterpret_cast<C_BaseAttributableItem*>(entitylist->GetClientEntityFromHandle(viewmodel->GetWeapon()))) {
				// Set appropriate viewmodel index based on item definition index.
				if (ItemDefinitionIndex.find(*active_weapon->GetItemDefinitionIndex()) != ItemDefinitionIndex.end()) {
					*viewmodel->GetModelIndex() = modelinfo->GetModelIndex(ItemDefinitionIndex.at(*active_weapon->GetItemDefinitionIndex()).model);
				}
			}
		}		

		break;
	}
	
	// Call the original function.
	return oFrameStageNotify(thisptr, stage);
}