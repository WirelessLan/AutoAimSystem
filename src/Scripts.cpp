#include "Scripts.h"

#include "AimSystems.h"

namespace Scripts
{
	inline void SetEnabled(std::monostate, bool a_enabled)
	{
		AimSystems::SetEnabled(a_enabled);
	}

	inline void ToggleEnabled(std::monostate)
	{
		AimSystems::ToggleEnabled();
	}

	inline bool IsEnabled(std::monostate)
	{
		return AimSystems::IsEnabled();
	}

	inline bool IsTargetLocked(std::monostate)
	{
		return AimSystems::IsTargetLocked();
	}

	void Register(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->BindNativeMethod(Version::PROJECT, "SetEnabled"sv, SetEnabled);
		a_vm->BindNativeMethod(Version::PROJECT, "ToggleEnabled"sv, ToggleEnabled);
		a_vm->BindNativeMethod(Version::PROJECT, "IsEnabled"sv, IsEnabled);
		a_vm->BindNativeMethod(Version::PROJECT, "IsTargetLocked"sv, IsTargetLocked);
	}
}
