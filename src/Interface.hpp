#pragma once

// Allow us to immediately apply our changes by calling a full update.
extern CL_FullUpdate_t CL_FullUpdate;

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
				ImGui::Text("Name tag");
				ImGui::InputText(std::string("##custom_name").append(item.second.entity_name).c_str(), weapon.custom_name, 32);
				ImGui::Spacing();

				ImGui::Text("Paint kit");
				ImGui::InputInt(std::string("##paint_kit").append(item.second.entity_name).c_str(), &weapon.fallback_paint_kit);
				ImGui::Spacing();

				ImGui::Text("Override item index");
				ImGui::InputInt(std::string("##override_item").append(item.second.entity_name).c_str(), &weapon.item_definition_index);
				ImGui::Spacing();

				ImGui::Text("Custom seed");
				ImGui::InputInt(std::string("##seed").append(item.second.entity_name).c_str(), &weapon.fallback_seed);
				ImGui::Spacing();

				ImGui::Text("Item quality");
				ImGui::InputInt(std::string("##quality").append(item.second.entity_name).c_str(), &weapon.entity_quality);
				ImGui::Spacing();

				ImGui::Text("Item wear");
				ImGui::SliderFloat(std::string("##wear").append(item.second.entity_name).c_str(), &weapon.fallback_wear, 0.00001f, 2.0f, "(%.5f)");
				ImGui::Spacing();

				ImGui::Text("StatTrak kills");
				ImGui::InputInt(std::string("##stattrak").append(item.second.entity_name).c_str(), &weapon.fallback_stattrak);
				ImGui::Spacing();
				
				// Add a placeholer 'Apply' button that calls 'cl_fullupdate'.
				if (ImGui::Button("Apply"))
					CL_FullUpdate();

				ImGui::Spacing();
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