#include "Configuration.hpp"

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