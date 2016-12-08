#pragma once

#include <windows.h>
#include <unordered_map>

struct EconomyItem_t {
	void Reset() {
		this->entity_quality = -1;
		this->fallback_seed = -1;
		this->fallback_paint_kit = -1;
		this->fallback_stattrak = -1;
		this->fallback_wear = -1;
		this->item_definition_index = -1;
		ZeroMemory(this->custom_name, 32);
	}

	bool is_valid = false;
	int entity_quality = -1;
	int fallback_seed = -1;
	int fallback_paint_kit = -1;
	int fallback_stattrak = -1;
	float fallback_wear = -1;
	int item_definition_index = -1;
	char custom_name[32];
};

class Configuration {
	private:
		std::string base_folder;
		std::string config_extension = ".cfg";
		std::unordered_map<size_t, EconomyItem_t> item_config;
		std::unordered_map<std::string, std::string> killicon_config;
	public:
		std::string GetBaseFolder();
		std::string GetConfigExtension();
		const bool SetBaseFolder(HMODULE);

		// Save & load presets from disk.
		const std::vector<std::string> GetPresets();
		const bool LoadPreset(std::string, bool);
		const bool SavePreset(std::string);
		const bool RemovePreset(std::string);

		// Weapon related functions.
		const bool HasWeaponConfiguration(size_t);
		EconomyItem_t& GetWeaponConfiguration(size_t);
		const void ResetWeaponConfiguration();

		// Killicon related functions.
		const bool HasKillIconOverride(std::string weapon);
		const std::string GetKillIconOverride(std::string original_weapon);
		void SetKillIconOverride(std::string original_weapon, std::string override_weapon);
};

extern Configuration config;