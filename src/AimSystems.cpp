#include "AimSystems.h"

#include "Configs.h"
#include "Utils.h"

namespace AimSystems
{
	bool Enabled = false;
	RE::Actor* LockedTarget = nullptr;

	inline bool IsLockedTargetValid(RE::PlayerCharacter* player, const Utils::CameraBasis& cam, RE::Actor* target) {
		if (!target || target->IsDead(false)) {
			return false;
		}

		if (Configs::Config.TargetEnemiesOnly && !target->GetHostileToActor(player)) {
			return false;
		}

		const auto target3D = target->Get3D();
		if (!target3D) {
			return false;
		}

		const auto targetPos = target3D->worldBound.center;

		const double maxDist2 = Configs::Config.MaxDistance * Configs::Config.MaxDistance;
		if (Utils::SquaredDistance3D(cam.Position, targetPos) > maxDist2) {
			return false;
		}

		bool dummy;
		return Configs::Config.RequireLOS ? Utils::HasLOSToTarget(player, target, dummy) : true;
	}

	inline RE::NiPoint3 GetAimPoint(RE::NiAVObject* target3D)
	{
		static constexpr const char* HeadNodes[] = {
			"HEAD",
			"LHead",
			"RHead",
			"Dogmeat_Head"
		};

		RE::NiAVObject* targetNode = nullptr;
		for (const auto& nodeName : HeadNodes) {
			targetNode = Utils::GetNode(target3D, nodeName);
			if (targetNode) {
				break;
			}
		}

		if (!targetNode) {
			targetNode = Utils::GetNode(target3D, "SPINE1");
			if (!targetNode) {
				targetNode = Utils::GetNode(target3D, "COM");
			}
		}

		return targetNode ? targetNode->world.translate : target3D->worldBound.center;
	}

	inline void AimAt(RE::PlayerCharacter* player, const Utils::CameraBasis& cam, RE::Actor* target) {
		auto target3D = target->Get3D();
		if (!target3D) {
			return;
		}

		const auto targetPos = GetAimPoint(target3D);

		RE::NiPoint3 d{
			targetPos.x - cam.Position.x,
			targetPos.y - cam.Position.y,
			targetPos.z - cam.Position.z
		};

		const float horiz = std::hypot(d.x, d.y);

		float pitch = Utils::ClampPitch(-std::atan2(d.z, horiz));
		float yaw = Utils::Normalize0ToTwoPi(std::atan2(d.x, d.y));

		player->data.angle.x = pitch;
		player->data.angle.z = yaw;
	}

	void Process() {
		if (!Enabled) {
			return;
		}

		auto player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}

		if (!Utils::IsSightedState(player)) {
			LockedTarget = nullptr;
			return;
		}

		auto camOpt = Utils::GetCameraBasis();
		if (!camOpt) {
			return;
		}
		const auto& cam = *camOpt;

		if (IsLockedTargetValid(player, cam, LockedTarget)) {
			AimAt(player, cam, LockedTarget);
			return;
		}
		else {
			LockedTarget = nullptr;
		}

		auto processLists = RE::ProcessLists::GetSingleton();
		if (!processLists) {
			return;
		}

		float bestScore = std::numeric_limits<float>::infinity();
		RE::Actor* bestActor = nullptr;

		for (const auto& actor : processLists->highActorHandles) {
			auto actorNiPtr = actor.get();
			if (!actorNiPtr) {
				continue;
			}

			auto actorPtr = actorNiPtr.get();
			if (actorPtr->IsPlayerRef() || actorPtr->IsDead(false)) {
				continue;
			}

			if (Configs::Config.TargetEnemiesOnly && !actorPtr->GetHostileToActor(player)) {
				continue;
			}

			const auto actor3D = actorPtr->Get3D();
			if (!actor3D) {
				continue;
			}

			const auto actorPos = GetAimPoint(actor3D);

			const double maxDist2 = Configs::Config.MaxDistance * Configs::Config.MaxDistance;
			const double dist2 = Utils::SquaredDistance3D(cam.Position, actorPos);
			if (dist2 > maxDist2) {
				continue;
			}

			float nx_raw, ny_raw;
			if (!Utils::WorldToNDCCentered(cam, actorPos, nx_raw, ny_raw) || !Utils::IsOnScreenNDC(nx_raw, ny_raw)) {
				continue;
			}

			float nx = nx_raw - cam.CenterNdcX;
			float ny = ny_raw - cam.CenterNdcY;

			const float maxRadius2 = Configs::Config.MaxNDCRadius * Configs::Config.MaxNDCRadius;
            const float angScore = nx * nx + ny * ny;
			if (angScore > maxRadius2) {
				continue;
			}

			bool dummy;
			if (Configs::Config.RequireLOS && !Utils::HasLOSToTarget(player, actorPtr, dummy)) {
				continue;
			}

			const float invMaxDist2 = static_cast<float>((maxDist2 > 0.0) ? (1.0 / maxDist2) : 0.0);
			const float distNorm = static_cast<float>(dist2) * invMaxDist2;

			// 최종 점수: 각도 우선 + 거리 페널티
			const float score = angScore + Configs::Config.DistanceWeightNdc2 * distNorm;

			if (score < bestScore) {
				bestScore = score;
				bestActor = actorPtr;
			}
		}

		if (bestActor) {
			LockedTarget = bestActor;
			AimAt(player, cam, LockedTarget);
		}
	}

	void ToggleEnabled() {
		SetEnabled(!Enabled);
	}

	void SetEnabled(bool enabled) {
		if (!enabled) {
			LockedTarget = nullptr;
		}
		Enabled = enabled;
	}

	bool IsEnabled() {
		return Enabled;
	}

	bool IsTargetLocked() {
		return LockedTarget != nullptr;
	}
}
