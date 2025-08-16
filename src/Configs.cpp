#include "Configs.h"

#include <Windows.h>

namespace Configs
{
	const std::string ConfigPath = "Data\\MCM\\Settings\\" + std::string(Version::PROJECT) + ".ini";

	ConfigData Config;

	inline std::string GetINIValue(const char* section, const char* key)
	{
		char result[256]{};
		GetPrivateProfileStringA(section, key, "", result, sizeof(result), ConfigPath.c_str());
		return result;
	}

	void Initialize() {
		std::string value;

		value = GetINIValue("AimSystem", "bEnabled");
		if (!value.empty()) {
			try {
				Config.Enabled = std::stoi(value) != 0;
			}
			catch (...) {
				logger::error("Invalid Enabled value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("Enabled: {}", Config.Enabled);

		value = GetINIValue("AimSystem", "fMaxDistance");
		if (!value.empty()) {
			try {
				Config.MaxDistance = std::stod(value);
			}
			catch (...) {
				logger::error("Invalid MaxDistance value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("MaxDistance: {}", Config.MaxDistance);

		value = GetINIValue("AimSystem", "bTargetEnemiesOnly");
		if (!value.empty()) {
			try {
				Config.TargetEnemiesOnly = std::stoi(value) != 0;
			}
			catch (...) {
				logger::error("Invalid TargetEnemiesOnly value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("TargetEnemiesOnly: {}", Config.TargetEnemiesOnly);
	}
}
