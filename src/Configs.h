#pragma once

namespace Configs
{
	struct ConfigData
	{
		std::string SystemEffectFormID;
		double MaxDistance = 5000.0;
		float MaxNDCRadius = 0.25f;
		float DistanceWeightNdc2 = 0.5f;
		bool TargetEnemiesOnly = true;
	};

	extern ConfigData Config;

	void Initialize();
}
