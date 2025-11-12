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

		value = GetINIValue("AimSystem", "bRequireLOS");
		if (!value.empty()) {
			try {
				Config.RequireLOS = std::stoi(value) != 0;
			}
			catch (...) {
				logger::error("Invalid RequireLOS value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("RequireLOS: {}", Config.RequireLOS);

		value = GetINIValue("AimSystem", "bUseAimPredictor");
		if (!value.empty()) {
			try {
				Config.UseAimPredictor = std::stoi(value) != 0;
			}
			catch (...) {
				logger::error("Invalid UseAimPredictor value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("UseAimPredictor: {}", Config.UseAimPredictor);

		value = GetINIValue("AimSystem", "fAimPredictorR0");
		if (!value.empty()) {
			try {
				Config.AimPredictorR0 = std::stod(value);
			}
			catch (...) {
				logger::error("Invalid AimPredictorR0 value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("AimPredictorR0: {}", Config.AimPredictorR0);

		value = GetINIValue("AimSystem", "fAimPredictorAlphaLo");
		if (!value.empty()) {
			try {
				Config.AimPredictorAlphaLo = std::stod(value);
			}
			catch (...) {
				logger::error("Invalid AimPredictorAlphaLo value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("AimPredictorAlphaLo: {}", Config.AimPredictorAlphaLo);

		value = GetINIValue("AimSystem", "fAimPredictorAlphaHi");
		if (!value.empty()) {
			try {
				Config.AimPredictorAlphaHi = std::stod(value);
			}
			catch (...) {
				logger::error("Invalid AimPredictorAlphaHi value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("AimPredictorAlphaHi: {}", Config.AimPredictorAlphaHi);

		value = GetINIValue("AimSystem", "fAimPredictorBetaLo");
		if (!value.empty()) {
			try {
				Config.AimPredictorBetaLo = std::stod(value);
			}
			catch (...) {
				logger::error("Invalid AimPredictorBetaLo value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("AimPredictorBetaLo: {}", Config.AimPredictorBetaLo);

		value = GetINIValue("AimSystem", "fAimPredictorBetaHi");
		if (!value.empty()) {
			try {
				Config.AimPredictorBetaHi = std::stod(value);
			}
			catch (...) {
				logger::error("Invalid AimPredictorBetaHi value in {}: {}", ConfigPath, value);
			}
		}
		logger::info("AimPredictorBetaHi: {}", Config.AimPredictorBetaHi);
	}
}
