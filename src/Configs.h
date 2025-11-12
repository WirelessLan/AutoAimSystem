#pragma once

namespace Configs
{
	struct ConfigData
	{
		double MaxDistance = 5000.0;
		float MaxNDCRadius = 0.25f;
		float DistanceWeightNdc2 = 0.5f;
		bool TargetEnemiesOnly = true;
		bool RequireLOS = true;
		bool UseAimPredictor = true;
		double AimPredictorR0 = 2.0;
		double AimPredictorAlphaLo = 0.15;
		double AimPredictorAlphaHi = 0.35;
		double AimPredictorBetaLo = 0.04;
		double AimPredictorBetaHi = 0.12;
	};

	extern ConfigData Config;

	void Initialize();
}
