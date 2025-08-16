#pragma once

namespace Configs
{
	struct ConfigData
	{
		bool Enabled = true;
		double MaxDistance = 3000.0;
		bool TargetEnemiesOnly = true;
	};

	extern ConfigData Config;

	void Initialize();
}
