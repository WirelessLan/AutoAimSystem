#include "Utils.h"

namespace Utils
{
	constexpr float PI = 3.14159265358979323846f;
	constexpr float TWO_PI = PI * 2.0f;
	constexpr float PITCH_LIMIT = 1.3962634f;
	constexpr float EPS_Z = 1e-6f;

	inline float GetScreenAspect()
	{
		const auto state = RE::BSGraphics::State::GetSingleton();
		return static_cast<float>(state.screenWidth) / static_cast<float>(state.screenHeight);
	}

    inline void Normalize(RE::NiPoint3& v)
	{
		const float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		if (len > 1e-12f) {
			v.x /= len;
			v.y /= len;
			v.z /= len;
		}
	}

	std::optional<CameraBasis> GetCameraBasis()
	{
		auto cam = RE::PlayerCamera::GetSingleton();
		if (!cam || !cam->cameraRoot) {
			return std::nullopt;
		}

		CameraBasis cb{};
		const auto& w = cam->cameraRoot->world;
		cb.Position = w.translate;

		cb.Right.x = w.rotate.entry[0].pt[0];
		cb.Right.y = w.rotate.entry[0].pt[1];
		cb.Right.z = w.rotate.entry[0].pt[2];

		cb.Forward.x = w.rotate.entry[1].pt[0];
		cb.Forward.y = w.rotate.entry[1].pt[1];
		cb.Forward.z = w.rotate.entry[1].pt[2];

		cb.Up.x = w.rotate.entry[2].pt[0];
		cb.Up.y = w.rotate.entry[2].pt[1];
		cb.Up.z = w.rotate.entry[2].pt[2];

		Normalize(cb.Right);
		Normalize(cb.Forward);
		Normalize(cb.Up);

		float aspect = GetScreenAspect();

		float fovX = cam->worldFOV * (PI / 180.0f);
		float fovY = 2.0f * std::atan(std::tan(fovX * 0.5f) / aspect);

		cb.TanHalfX = std::tan(fovX * 0.5f);
		cb.TanHalfY = std::tan(fovY * 0.5f);

		return cb;
	}

	inline bool WorldToNDC_NoOffset(const CameraBasis& c, const RE::NiPoint3& P, float& ndcX, float& ndcY)
	{
		const float vx = P.x - c.Position.x, vy = P.y - c.Position.y, vz = P.z - c.Position.z;
		const float xprime = vx * c.Right.x + vy * c.Right.y + vz * c.Right.z;
		const float yprime = vx * c.Up.x + vy * c.Up.y + vz * c.Up.z;
		const float zprime = vx * c.Forward.x + vy * c.Forward.y + vz * c.Forward.z;
		if (zprime <= EPS_Z) {
			return false;
		}
		ndcX = xprime / (zprime * c.TanHalfX);
		ndcY = yprime / (zprime * c.TanHalfY);
		return true;
	}


    bool WorldToNDCCentered(const CameraBasis& c, const RE::NiPoint3& P, float& ndcX, float& ndcY)
	{
		if (!WorldToNDC_NoOffset(c, P, ndcX, ndcY)) {
			return false;
		}
		ndcX -= c.CenterNdcX;
		ndcY -= c.CenterNdcY;
		return true;
	}

	bool IsOnScreenNDC(float ndcX, float ndcY)
	{
		return std::abs(ndcX) <= 1.0f && std::abs(ndcY) <= 1.0f;
	}

	double SquaredDistance3D(const RE::NiPoint3& a, const RE::NiPoint3& b)
	{
		double dx = a.x - b.x;
		double dy = a.y - b.y;
		double dz = a.z - b.z;
		return dx * dx + dy * dy + dz * dz;
	}

	float Normalize0ToTwoPi(float a)
	{
		a = std::fmod(a, TWO_PI);
		if (a < 0.0f) {
			a += TWO_PI;
		}
		return a;
	}

	float ClampPitch(float p)
	{
		if (p > PITCH_LIMIT) {
			return PITCH_LIMIT;
		}
		if (p < -PITCH_LIMIT) {
			return -PITCH_LIMIT;
		}
		return p;
	}

	RE::NiAVObject* GetHeadNode(RE::Actor* actor)
	{
		if (!actor || !actor->Get3D()) {
			return nullptr;
		}

		auto headNode = actor->Get3D()->GetObjectByName("HEAD");
		if (!headNode) {
			return nullptr;
		}

		return headNode;
	}

	bool IsSightedState(const RE::PlayerCharacter* player)
	{
		return player->gunState == RE::GUN_STATE::kSighted ||
		       player->gunState == RE::GUN_STATE::kFireSighted;
	}
}
