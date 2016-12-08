#include <fstream>

#include "JSON\src\json.hpp"

#include "Configuration.hpp"
#include "ItemDefinitions.hpp"

// Return the base folder containing all configuration files.
std::string Configuration::GetBaseFolder() {
	return this->base_folder;
}

// Always call this before reading or writing to any files.
const bool Configuration::SetBaseFolder(HMODULE dll_instance) {
	// Construct a buffer large enough to contain a filename.
	char filename_buffer[MAX_PATH];

	// Get the full filename of the specified DLL.
	if (GetModuleFileNameA(dll_instance, filename_buffer, MAX_PATH) == 0)
		return false;

	// Split the filename at the last separator to get the folder path.
	std::string::size_type pos = std::string(filename_buffer).find_last_of("\\/");
	base_folder = std::string(filename_buffer).substr(0, pos);

	return true;
}

const std::vector<std::string> Configuration::GetPresets() {
	std::vector<std::string> names;

	WIN32_FIND_DATAA find_data;
	HANDLE preset_file = FindFirstFileA(config.GetBaseFolder().append("\\*.cfg").c_str(), &find_data);

	if (preset_file != INVALID_HANDLE_VALUE) {
		do {
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;

			names.push_back(find_data.cFileName);
		} while (FindNextFileA(preset_file, &find_data));

		FindClose(preset_file);
	}

	return names;
}

// Load configuration values from a file.
const bool Configuration::LoadPreset(std::string filename, bool reset = false) {
	// Resolve the relative filename to a full path.
	std::string input_filename = this->base_folder + "\\" + filename;

	// Open the input configuration file for reading.
	std::ifstream input_file = std::ifstream(input_filename);

	if (!input_file.good())
		return false;

	nlohmann::json preset;
	
	try {
		// Attempt to parse the file.
		preset << input_file;
	} catch (...) {
		// Catch parsing exceptions.
		input_file.close();
		return false;
	}
	
	if (preset["items"].empty()) {
		input_file.close();
		return false;
	}
	
	// Start reading the configuration for each item.
	for (const auto& item: ItemDefinitionIndex) {
		EconomyItem_t& item_config = this->GetWeaponConfiguration(item.first);
		item_config.is_valid = true;

		if (reset)
			item_config.Reset();

		std::string item_key = std::to_string(item.first);

		if (preset["items"][item_key].empty())
			continue;

		if (preset["items"][item_key].find("entity_quality") != preset["items"][item_key].end())
			item_config.entity_quality = preset["items"][item_key]["entity_quality"].get<int>();

		if (preset["items"][item_key].find("fallback_seed") != preset["items"][item_key].end())
			item_config.fallback_seed = preset["items"][item_key]["fallback_seed"].get<int>();

		if (preset["items"][item_key].find("fallback_paint_kit") != preset["items"][item_key].end())
			item_config.fallback_paint_kit = preset["items"][item_key]["fallback_paint_kit"].get<int>();

		if (preset["items"][item_key].find("fallback_stattrak") != preset["items"][item_key].end())
			item_config.fallback_stattrak = preset["items"][item_key]["fallback_stattrak"].get<int>();
		
		if (preset["items"][item_key].find("fallback_wear") != preset["items"][item_key].end())
			item_config.fallback_wear = preset["items"][item_key]["fallback_wear"].get<float>();

		if (preset["items"][item_key].find("item_definition_index") != preset["items"][item_key].end())
			item_config.item_definition_index = preset["items"][item_key]["item_definition_index"].get<int>();
		
		if (preset["items"][item_key].find("custom_name") != preset["items"][item_key].end())
			snprintf(item_config.custom_name, 32, "%s", preset["items"][item_key]["custom_name"].get<std::string>().c_str());
	}

	input_file.close();

	return true;
}

// Save configuration values to a file.
const bool Configuration::SavePreset(std::string filename) {
	// Resolve the relative filename to a full path.
	std::string output_filename = this->base_folder + "\\" + filename + ".cfg";

	// Open the output configuration file for writing.
	std::ofstream output_file = std::ofstream(output_filename);
	
	if (!output_file.good())
		return false;

	// Create a JSON object to serialize the configuration.
	nlohmann::json preset;
	
	for (const auto& item: this->item_config) {
		const EconomyItem_t& item_config = item.second;

		if (!item_config.is_valid)
			continue;

		std::string item_key = std::to_string(item.first);

		if (item_config.entity_quality != -1)
			preset["items"][item_key]["entity_quality"] = item_config.entity_quality;

		if (item_config.fallback_seed != -1)
			preset["items"][item_key]["fallback_seed"] = item_config.fallback_seed;

		if (item_config.fallback_paint_kit != -1)
			preset["items"][item_key]["fallback_paint_kit"] = item_config.fallback_paint_kit;

		if (item_config.fallback_stattrak != -1)
			preset["items"][item_key]["fallback_stattrak"] = item_config.fallback_stattrak;

		if (item_config.fallback_wear != -1)
			preset["items"][item_key]["fallback_wear"] = item_config.fallback_wear;

		if (item_config.item_definition_index != -1)
			preset["items"][item_key]["item_definition_index"] = item_config.item_definition_index;

		if (strlen(item_config.custom_name) != 0)
			preset["items"][item_key]["custom_name"] = item_config.custom_name;
	}

	// Write the contents to disk with pretty formatting.
	output_file << std::setw(4) << preset << std::endl;
	output_file.close();
	
	return true;
}

const bool Configuration::RemovePreset(std::string filename) {
	filename = config.GetBaseFolder() + "\\" + filename;

	return std::remove(filename.c_str()) == 0;
}

// Checks if there is a valid configuration for the specified item.
const bool Configuration::HasWeaponConfiguration(size_t item_definition_index) {
	if (item_config.find(item_definition_index) == item_config.end())
		return false;

	return item_config.at(item_definition_index).is_valid;
}

// Returns an item configuration reference at a specified index.
EconomyItem_t& Configuration::GetWeaponConfiguration(size_t item_definition_index) {
	// Create a new item at the specified index if none exist.
	if (item_config.find(item_definition_index) == item_config.end())
		item_config[item_definition_index] = EconomyItem_t();

	return item_config.at(item_definition_index);
}

// Reset all weapon configurations.
const void Configuration::ResetWeaponConfiguration() {
	for (auto& item: this->item_config) {
		item.second.Reset();
	}
}

// Checks if an override exists for this kill icon.
const bool Configuration::HasKillIconOverride(std::string weapon) {
	return this->killicon_config.find(weapon) != this->killicon_config.end();
};

// Get the override assigned to a specified kill icon.
const std::string Configuration::GetKillIconOverride(std::string original_weapon) {
	if (!this->HasKillIconOverride(original_weapon))
		return original_weapon;

	return this->killicon_config.at(original_weapon);
}

// Use this to replace a kill icon with another, mainly useful for when you're using a custom knife.
void Configuration::SetKillIconOverride(std::string original_weapon, std::string override_weapon) {
	this->killicon_config[original_weapon] = override_weapon;
}
