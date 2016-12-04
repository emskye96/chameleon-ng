#pragma once

#include <windows.h>
#include <unordered_map>

struct EconomyItem_t {
	bool is_valid = false;
	int entity_quality = -1;
	int fallback_seed = -1;
	int fallback_paint_kit = -1;
	int fallback_stattrak = -1;
	float fallback_wear = -1;
	int item_definition_index = -1;
	char custom_name[32];
	int killicon = -1;
};

class Configuration {
	private:
		std::string base_folder;
		std::unordered_map<size_t, EconomyItem_t> item_config;
		std::unordered_map<std::string, std::string> killicon_config;
	public:
		const bool SetBaseFolder(HMODULE);

		// Weapon related functions.
		const bool HasWeaponConfiguration(size_t);
		EconomyItem_t& GetWeaponConfiguration(size_t);

		// Killicon related functions.
		const bool HasKillIconOverride(std::string weapon);
		const std::string GetKillIconOverride(std::string original_weapon);
		void SetKillIconOverride(std::string original_weapon, std::string override_weapon);
};

extern Configuration config;