#include "Events.h"

#include "AimSystems.h"

namespace Events
{
	struct PlayerUpdateEvent
	{
	private:
		using EventSource_t = RE::BSTGlobalEvent::EventSource<PlayerUpdateEvent>;

	public:
		[[nodiscard]] static EventSource_t* GetEventSource()
		{
			REL::Relocation<EventSource_t**> singleton{ REL::ID(587138) };
			if (!*singleton) {
				*singleton = new EventSource_t(&RE::BSTGlobalEvent::GetSingleton()->eventSourceSDMKiller);
			}
			return *singleton;
		}
	};

	class PlayerUpdateEventSink : public RE::BSTEventSink<PlayerUpdateEvent>
	{
	public:
		[[nodiscard]] static PlayerUpdateEventSink* GetSingleton()
		{
			static PlayerUpdateEventSink singleton;
			return &singleton;
		}

		RE::BSEventNotifyControl ProcessEvent(const PlayerUpdateEvent&, RE::BSTEventSource<PlayerUpdateEvent>*) override
		{
			AimSystems::Process();
			return RE::BSEventNotifyControl::kContinue;
		}
	};

	void Install()
	{
		PlayerUpdateEvent::GetEventSource()->RegisterSink(PlayerUpdateEventSink::GetSingleton());
	}
}
