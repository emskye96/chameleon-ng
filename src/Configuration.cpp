#include "Configuration.hpp"

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

// Load configuration values from a file.
const bool Configuration::LoadPreset(std::string filename) {
	return true;
}

// Save configuration values to a file.
const bool Configuration::SavePreset(std::string filename) {
	return false;
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