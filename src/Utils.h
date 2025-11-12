#pragma once

namespace Utils
{
	struct CameraBasis
	{
		RE::NiPoint3 Position;
		RE::NiPoint3 Right;
		RE::NiPoint3 Up;
		RE::NiPoint3 Forward;
		float TanHalfX, TanHalfY;
		float CenterNdcX, CenterNdcY;
	};

	std::optional<CameraBasis> GetCameraBasis();
	bool WorldToNDCCentered(const CameraBasis& c, const RE::NiPoint3& P, float& ndcX, float& ndcY);
	bool IsOnScreenNDC(float ndcX, float ndcY);

	double SquaredDistance3D(const RE::NiPoint3& a, const RE::NiPoint3& b);
	float Normalize0ToTwoPi(float a);
	float ClampPitch(float p);

	RE::NiAVObject* GetNode(RE::NiAVObject* node, std::string_view nodeName);

	bool IsSightedState(const RE::PlayerCharacter* player);
	bool HasLOSToTarget(RE::PlayerCharacter* player, const RE::TESObjectREFR* a_ref);
	bool HasActiveMagicEffect(RE::Actor* actor, RE::EffectSetting* effect);

	RE::TESForm* GetFormFromIdentifier(std::string_view a_pluginName, std::uint32_t a_formID);
}
