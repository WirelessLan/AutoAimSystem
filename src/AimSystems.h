#pragma once

#include "Configs.h"

namespace AimSystems
{
	class AimPredictor
	{
	public:
		AimPredictor() : AlphaLo(0), AlphaHi(0), BetaLo(0), BetaHi(0), R0(0), Initialized(false)
		{
		}

		RE::NiPoint3 Update(const RE::NiPoint3& z)
		{
			auto now = std::chrono::steady_clock::now();

			if (!Initialized) {
				XHat = z;
				VHat = RE::NiPoint3();
				Initialized = true;
				LastTime = now;
				return XHat;
			}

			std::chrono::duration<double> delta = now - LastTime;
			double dt = delta.count();
			if (dt <= 0.0)  {
				dt = 1e-6;
			}
			LastTime = now;

			RE::NiPoint3 x_pred = XHat + VHat * static_cast<float>(dt);
			RE::NiPoint3 v_pred = VHat;
			RE::NiPoint3 r = z - x_pred;

			double rmag = std::sqrt(static_cast<double>(r.x) * r.x +
									static_cast<double>(r.y) * r.y +
									static_cast<double>(r.z) * r.z);
			double s = (rmag / R0);
			if (s < 0.0) {
				s = 0.0;
			}
			if (s > 1.0) {
				s = 1.0;
			}

			double alpha = AlphaLo + (AlphaHi - AlphaLo) * s;
			double beta = BetaLo + (BetaHi - BetaLo) * s;

			XHat = x_pred + r * static_cast<float>(alpha);
			VHat = v_pred + r * static_cast<float>(beta / dt);

			return XHat + VHat * static_cast<float>(dt);
		}

		void Clear()
		{
			XHat = RE::NiPoint3();
			VHat = RE::NiPoint3();
			Initialized = false;
		}

		void Configure(double r0, double alphaLo, double alphaHi, double betaLo, double betaHi)
		{
			R0 = r0;
			AlphaLo = alphaLo;
			AlphaHi = alphaHi;
			BetaLo = betaLo;
			BetaHi = betaHi;
		}

	private:
		RE::NiPoint3 XHat;
		RE::NiPoint3 VHat;
		bool Initialized;
		std::chrono::steady_clock::time_point LastTime;

		double AlphaLo;
		double AlphaHi;
		double BetaLo;
		double BetaHi;
		double R0;
	};

	extern AimPredictor Predictor;

	void Process();
	void ToggleEnabled();
	void SetEnabled(bool enabled);
	bool IsEnabled();
	bool IsTargetLocked();
}
