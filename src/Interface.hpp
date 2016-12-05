#pragma once

// Perform all the GUI drawing here.
inline void RenderInterface() {
	// Place all item settings under a collapsing header.
	if (ImGui::CollapsingHeader("Items")) {
		// Loop through the game item list.
		for (const auto& item: ItemDefinitionIndex) {
			// Get the configuration for the current item.
			EconomyItem_t& weapon = config.GetWeaponConfiguration(item.first);

			// Ensure that our settings will be used.
			if (!weapon.is_valid)
				weapon.is_valid = true;

			// Create a new node in the tree for this item.
			if (ImGui::TreeNode(item.second.display_name)) {
				// Add input forms to edit values for this item.
				ImGui::InputInt(std::string("Item Index##").append(item.second.entity_name).c_str(), &weapon.item_definition_index);
				ImGui::InputInt(std::string("Paint Kit##").append(item.second.entity_name).c_str(), &weapon.fallback_paint_kit);
				ImGui::InputInt(std::string("Seed##").append(item.second.entity_name).c_str(), &weapon.fallback_seed);
				ImGui::InputInt(std::string("Quality##").append(item.second.entity_name).c_str(), &weapon.entity_quality);
				ImGui::InputInt(std::string("StatTrak##").append(item.second.entity_name).c_str(), &weapon.fallback_stattrak);
				ImGui::InputFloat(std::string("Wear##").append(item.second.entity_name).c_str(), &weapon.fallback_wear);
				ImGui::InputText(std::string("Name Tag##").append(item.second.entity_name).c_str(), weapon.custom_name, 32);

				// Add a placeholer 'Apply' button that calls 'cl_fullupdate'.
				if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvail().x, 20)))
					CL_FullUpdate();

				ImGui::TreePop();
			}
		}
	}

	if (ImGui::CollapsingHeader("Presets")) {
		static std::vector<std::string> presets = config.GetPresets();

		static char preset_filename[64];
		ImGui::Text("Preset filename");
		ImGui::InputText("##current_filename", preset_filename, 64);
		ImGui::SameLine();
		
		if (ImGui::Button("New")) {
			config.SavePreset(preset_filename);
			presets = config.GetPresets();
		}

		static bool reset_on_load = false;
		ImGui::Checkbox("Reset settings on load.", &reset_on_load);

		if (presets.size() >= 1) {
			for (const std::string& preset: presets) {
				ImGui::AlignFirstTextHeightToWidgets();
				
				ImGui::BulletText(preset.c_str());
				ImGui::SameLine();

				if (ImGui::Button(std::string("Save##").append(preset).c_str()))
					config.SavePreset(preset);

				ImGui::SameLine();

				if (ImGui::Button(std::string("Load##").append(preset).c_str())) {
					config.LoadPreset(preset, reset_on_load);
					CL_FullUpdate();
				}

				ImGui::SameLine();
				
				if (ImGui::Button(std::string("Delete##").append(preset).c_str()) && config.RemovePreset(preset.c_str()))
					presets = config.GetPresets();

				ImGui::Spacing();
			}
		} else {
			ImGui::TextWrapped("No presets found. You can create a preset by entering a name above and pressing the 'New' button.");
		}

		if (ImGui::Button("Refresh"))
			presets = config.GetPresets();
	}
}