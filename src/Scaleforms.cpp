#include "Scaleforms.h"
#include "Configs.h"

namespace Scaleforms
{
	class MCMHandler : public RE::Scaleform::GFx::FunctionHandler
	{
	public:
		[[nodiscard]] static MCMHandler* GetSingleton()
		{
			static MCMHandler singleton;
			return &singleton;
		}

		virtual void Call(const Params& a_params) override
		{
			if (a_params.argCount != 2 || a_params.args[0].GetType() != RE::Scaleform::GFx::Value::ValueType::kString) {
				return;
			}

			if (strcmp(a_params.args[0].GetString(), "fMaxDistance") == 0) {
				Configs::Config.MaxDistance = static_cast<float>(a_params.args[1].GetNumber());
			}
			else if (strcmp(a_params.args[0].GetString(), "fMaxNDCRadius") == 0) {
				Configs::Config.MaxNDCRadius = static_cast<float>(a_params.args[1].GetNumber());
			}
			else if (strcmp(a_params.args[0].GetString(), "fDistanceWeightNdc2") == 0) {
				Configs::Config.DistanceWeightNdc2 = static_cast<float>(a_params.args[1].GetNumber());
			}
			else if (strcmp(a_params.args[0].GetString(), "bTargetEnemiesOnly") == 0) {
				Configs::Config.TargetEnemiesOnly = a_params.args[1].GetBoolean();
			}
			else if (strcmp(a_params.args[0].GetString(), "bRequireLOS") == 0) {
				Configs::Config.RequireLOS = a_params.args[1].GetBoolean();
			}
		}
	};

	inline void RegisterFunction(RE::Scaleform::GFx::Movie* a_view, RE::Scaleform::GFx::Value* a_f4se_root, RE::Scaleform::GFx::FunctionHandler* a_handler, F4SE::stl::zstring a_name)
	{
		RE::Scaleform::GFx::Value fn;
		a_view->CreateFunction(&fn, a_handler);
		a_f4se_root->SetMember(a_name, fn);
	}

	void Register(RE::Scaleform::GFx::Movie* a_view, RE::Scaleform::GFx::Value* a_value) {
		RegisterFunction(a_view, a_value, MCMHandler::GetSingleton(), "Set");
	}
}
