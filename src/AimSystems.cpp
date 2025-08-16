#include "AimSystems.h"

#include "Configs.h"
#include "Utils.h"

namespace AimSystems
{
	RE::Actor* LockedTarget = nullptr;
	float LockedScore = std::numeric_limits<float>::infinity();

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

		if (Utils::SquaredDistance3D(cam.Position, targetPos) > Configs::Config.MaxDistance * Configs::Config.MaxDistance) {
			return false;
		}

		bool hasLOS = false;
		return Utils::HasLOSToTarget(player, target, hasLOS);
	}

	inline void AimAt(RE::PlayerCharacter* player, const Utils::CameraBasis& cam, RE::Actor* target) {
		auto target3D = target->Get3D();
		if (!target3D) {
			return;
		}
		
		RE::NiPoint3 targetPos;
		auto targetNode = Utils::GetNode(target3D, "HEAD");
		if (!targetNode) {
			targetNode = Utils::GetNode(target3D, "SPINE1");
		}

		if (targetNode) {
			targetPos = targetNode->world.translate;
		} else {
			targetPos = target3D->worldBound.center;
		}

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
		if (!Configs::Config.Enabled) {
			return;
		}

		auto player = RE::PlayerCharacter::GetSingleton();
		if (!player || !Utils::IsSightedState(player)) {
			LockedTarget = nullptr;
			return;
		}

		auto processLists = RE::ProcessLists::GetSingleton();
		if (!processLists) {
			LockedTarget = nullptr;
			return;
		}

		auto camOpt = Utils::GetCameraBasis();
		if (!camOpt) {
			return;
		}
		const Utils::CameraBasis cam = *camOpt;

		if (IsLockedTargetValid(player, cam, LockedTarget)) {
			AimAt(player, cam, LockedTarget);
			return;
		}
		else {	
			LockedTarget = nullptr;
			LockedScore = std::numeric_limits<float>::infinity();
		}

		float bestScore = std::numeric_limits<float>::infinity();
		RE::Actor* bestActor = nullptr;

		for (const auto& actor : processLists->highActorHandles) {
			auto actorPtr = actor.get();
			if (!actorPtr || actorPtr->IsPlayerRef() || actorPtr->IsDead(false)) {
				continue;
			}

			if (Configs::Config.TargetEnemiesOnly && !actorPtr->GetHostileToActor(player)) {
				continue;
			}

			const auto actor3D = actorPtr->Get3D();
			if (!actor3D) {
				continue;
			}

			const auto center = actor3D->worldBound.center;

			const double maxDist2 = Configs::Config.MaxDistance * Configs::Config.MaxDistance;
			const double dist2 = Utils::SquaredDistance3D(cam.Position, center);
			if (dist2 > maxDist2) {
				continue;
			}

			float nx_raw, ny_raw;
			if (!Utils::WorldToNDCCentered(cam, center, nx_raw, ny_raw) || !Utils::IsOnScreenNDC(nx_raw, ny_raw)) {
				continue;
			}

			float nx = nx_raw - cam.CenterNdcX;
			float ny = ny_raw - cam.CenterNdcY;

			const float maxRadius2 = Configs::Config.MaxNDCRadius * Configs::Config.MaxNDCRadius;
            const float angScore = nx * nx + ny * ny;
			if (angScore > maxRadius2) {
				continue;
			}

			bool hasLOS = false;
			if (!Utils::HasLOSToTarget(player, actorPtr.get(), hasLOS)) {
				continue;
			}

			const float invMaxDist2 = static_cast<float>((maxDist2 > 0.0) ? (1.0 / maxDist2) : 0.0);
			const float distNorm = static_cast<float>(dist2) * invMaxDist2;

			// 최종 점수: 각도 우선 + 거리 페널티
			const float score = angScore + Configs::Config.DistanceWeightNdc2 * distNorm;
			if (score < bestScore) {
				bestScore = score;
				bestActor = actorPtr.get();
			}
		}

		if (bestActor) {
			LockedTarget = bestActor;
			LockedScore = bestScore;
			AimAt(player, cam, LockedTarget);
		}
	}
}
