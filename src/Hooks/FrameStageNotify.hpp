#pragma once

// Function signature of IBaseClientDLL::FrameStageNotify.
typedef void (__thiscall *FrameStageNotify_t) (IBaseClientDLL*, ClientFrameStage_t);

// Replacement function that will get called at various points during frame rendering.
void __fastcall hkFrameStageNotify(IBaseClientDLL* thisptr, void* edx, ClientFrameStage_t stage) {
	// Get the original function and store it in a static variable for later usage.
	static FrameStageNotify_t oFrameStageNotify = clientdll_hook->GetOriginalFunction<FrameStageNotify_t>(36);

	// Only perform our replacements during the PostDataUpdate start stage.
	while (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		// Get a pointer to our local player entity
		int localplayer_index = engine->GetLocalPlayer();
		C_BasePlayer* localplayer = reinterpret_cast<C_BasePlayer*>(entitylist->GetClientEntity(localplayer_index));

		// Nothing to if we aren't alive.
		if (!localplayer || localplayer->GetLifeState() != LIFE_ALIVE)
			break;

		// Get our player information.
		player_info_t localplayer_info;

		if (!engine->GetPlayerInfo(localplayer_index, &localplayer_info))
			break;

		// Get an array containing handles of our weapons.
		CBaseHandle* weapons = localplayer->GetWeapons();

		for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++) {
			// Convert the weapon handle to an entity pointer.
			C_BaseAttributableItem* weapon = reinterpret_cast<C_BaseAttributableItem*>(entitylist->GetClientEntityFromHandle(weapons[i]));
			
			if (!weapon)
				continue;

			// Get the current item definition index to uniquely identify this weapon.
			unsigned int* item_definition_index = weapon->GetItemDefinitionIndex();

			// Check if we have an existing override for this weapon.
			if (config.HasWeaponConfiguration(*item_definition_index)) {
				// Get a reference to the configuration structure for this weapon.
				const EconomyItem_t& weapon_config = config.GetWeaponConfiguration(*item_definition_index);

				// Force fallback values to be used.
				*weapon->GetItemIDHigh() = -1;

				// Set the owner of the weapon to our lower XUID. (fixes StatTrak)
				*weapon->GetAccountID() = localplayer_info.xuid_low;

				// Check if this weapon should be replaced by another.
				if (weapon_config.item_definition_index != -1) {
					// Make sure the replacement index is inside our item definition list.
					if (ItemDefinitionIndex.find(weapon_config.item_definition_index) != ItemDefinitionIndex.end()) {
						// Set the weapon model index -- required for paint kits to work on replacement items after the 29/11/2016 update.
						*weapon->GetModelIndex() = modelinfo->GetModelIndex(ItemDefinitionIndex.at(weapon_config.item_definition_index).model);

						// Make sure the original item is in our definition list too.
						if (ItemDefinitionIndex.find(*item_definition_index) != ItemDefinitionIndex.end()) {
							// Get references to both items.
							const Item_t& original_item = ItemDefinitionIndex.at(*item_definition_index);
							const Item_t& replacement_item = ItemDefinitionIndex.at(weapon_config.item_definition_index);

							// Attempt to set the appropriate kill icon for the new item.
							if (original_item.killicon && replacement_item.killicon)
								config.SetKillIconOverride(original_item.killicon, replacement_item.killicon);

							// Finally, update the item definition index.
							*item_definition_index = weapon_config.item_definition_index;
						}
					}
				}

				// Set weapon quality. (eg. StatTrak, Genuine, Souvenir)
				if (weapon_config.entity_quality != -1)
					*weapon->GetEntityQuality() = weapon_config.entity_quality;

				// Apply custom name tag.
				if (weapon_config.custom_name)
					snprintf(weapon->GetCustomName(), 32, "%s", weapon_config.custom_name);

				// Apply the paint kit.
				if (weapon_config.fallback_paint_kit != -1)
					*weapon->GetFallbackPaintKit() = weapon_config.fallback_paint_kit;

				// Set weapon seed for paint kits with patterns.
				if (weapon_config.fallback_seed != -1)
					*weapon->GetFallbackSeed() = weapon_config.fallback_seed;

				// Set weapon wear. (eg. Factory New - Battle Scarred)
				if (weapon_config.fallback_wear != -1)
					*weapon->GetFallbackWear() = weapon_config.fallback_wear;

				// Apply StatTrak kill counter. (-1 disables, max 999999)
				if (weapon_config.fallback_stattrak != -1)
					*weapon->GetFallbackStatTrak() = weapon_config.fallback_stattrak;
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