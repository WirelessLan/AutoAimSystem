#pragma once

namespace Configs
{
	struct ConfigData
	{
		bool Enabled = true;
		double MaxDistance = 5000.0;
		float MaxNDCRadius = 0.5f;
		float DistanceWeightNdc2 = 0.5f;
		bool TargetEnemiesOnly = true;
	};

	extern ConfigData Config;

	void Initialize();
}
