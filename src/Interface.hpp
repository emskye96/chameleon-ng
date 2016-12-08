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
				// Values for checkboxes.
				static bool update_on_paintkit = false;
				static bool update_on_seed = false;

				// Add input forms to edit values for this item.
				ImGui::Text("Name tag");
				ImGui::InputText(std::string("##custom_name").append(item.second.entity_name).c_str(), weapon.custom_name, 32);
				ImGui::Spacing();

				ImGui::Text("Paint kit");
				if (ImGui::InputInt(std::string("##paint_kit").append(item.second.entity_name).c_str(), &weapon.fallback_paint_kit) && update_on_paintkit)
					CL_FullUpdate();
				ImGui::Checkbox(std::string("Update when paint kit is changed.##").append(item.second.entity_name).c_str(), &update_on_paintkit);
				ImGui::Spacing();

				ImGui::Text("Override item index");
				ImGui::InputInt(std::string("##override_item").append(item.second.entity_name).c_str(), &weapon.item_definition_index);
				ImGui::Spacing();

				ImGui::Text("Custom seed");
				if (ImGui::InputInt(std::string("##seed").append(item.second.entity_name).c_str(), &weapon.fallback_seed) && update_on_seed)
					CL_FullUpdate();
				ImGui::Checkbox(std::string("Update when seed is changed.##").append(item.second.entity_name).c_str(), &update_on_seed);
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

				if (ImGui::Button("Reset")) {
					// Reset all settings for this weapon to default.
					weapon.Reset();

					// Perform a full update.
					CL_FullUpdate();
				}

				ImGui::SameLine();
				
				if (ImGui::Button("Apply")) {
					// Perform a full update.
					CL_FullUpdate();
				}

				ImGui::Spacing();
				ImGui::TreePop();
			}
		}
	}

	if (ImGui::CollapsingHeader("Presets")) {
		// Get the configuration file extension.
		static std::string extension = config.GetConfigExtension();

		// Get a list of configuration files in the working directory.
		static std::vector<std::string> presets = config.GetPresets();

		// Show a text box for creating new presets.
		static char preset_filename[64];
		ImGui::Text("Preset filename");
		ImGui::InputText("##current_filename", preset_filename, 64);
		ImGui::SameLine();
		
		if (ImGui::Button("New")) {
			// Append the file extension if the user did not specify it.
			std::string filename(preset_filename);
			
			if (!std::equal(extension.rbegin(), extension.rend(), filename.rbegin()))
				filename = filename.append(extension);

			// Save the new preset to disk.
			config.SavePreset(filename);

			// Refresh the list of presets.
			presets = config.GetPresets();
		}

		// Whether we should clear other item settings when loading a preset.
		static bool reset_on_load = false;
		ImGui::Checkbox("Reset settings on load.", &reset_on_load);

		if (presets.size() >= 1) {
			for (const std::string& preset: presets) {
				// Display the filename of this preset.
				ImGui::AlignFirstTextHeightToWidgets();
				ImGui::BulletText(preset.c_str());
				ImGui::SameLine();

				if (ImGui::Button(std::string("Save##").append(preset).c_str())) {
					// Overwrite this file with current item settings.
					config.SavePreset(preset);
				}

				ImGui::SameLine();

				if (ImGui::Button(std::string("Load##").append(preset).c_str())) {
					// Load current settings from this file.
					config.LoadPreset(preset, reset_on_load);

					// Perform a full update.
					CL_FullUpdate();
				}

				ImGui::SameLine();
				
				if (ImGui::Button(std::string("Delete##").append(preset).c_str()) && config.RemovePreset(preset.c_str())) {
					// Attempt to delete the preset file from disk.
					presets = config.GetPresets();
				}

				ImGui::Spacing();
			}
		} else {
			// Show default text when no presets exist.
			ImGui::TextWrapped("No presets found. You can create a preset by entering a name above and pressing the 'New' button.");
		}

		if (ImGui::Button("Refresh preset list")) {
			// Check the folder again for more presets.
			presets = config.GetPresets();
		}

		ImGui::SameLine();

		if (ImGui::Button("Reset all settings")) {
			// Reset all user settings.
			config.ResetWeaponConfiguration();

			// Perform a full update.
			CL_FullUpdate();
		}
	}
}