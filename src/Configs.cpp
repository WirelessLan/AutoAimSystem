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

		value = GetINIValue("AimSystem", "sSystemEffectFormID");
		if (!value.empty()) {
			Config.SystemEffectFormID = value;
		}
		logger::info("SystemEffectFormID: {}", Config.SystemEffectFormID);

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

		value = GetINIValue("AimSystem", "fMaxNDCRadius");
		if (!value.empty()) {
			try {
				Config.MaxNDCRadius = std::stof(value);
			}
			catch (...) {
				logger::error("Invalid MaxNDCRadius value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("MaxNDCRadius: {}", Config.MaxNDCRadius);

		value = GetINIValue("AimSystem", "fDistanceWeightNdc2");
		if (!value.empty()) {
			try {
				Config.DistanceWeightNdc2 = std::stof(value);
			}
			catch (...) {
				logger::error("Invalid DistanceWeightNdc2 value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("DistanceWeightNdc2: {}", Config.DistanceWeightNdc2);

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
