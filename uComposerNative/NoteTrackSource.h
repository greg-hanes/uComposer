#pragma once

#include <mutex>
#include "BaseEffect.h"

namespace uComposer
{
	class NoteTrackSource : public BaseEffect
	{
	public:
		using BaseEffect::BaseEffect;

		// Inherited via BaseEffect
		virtual void Initialize() override;
		virtual void Process(ProcessData data, float dt) override;

		void SendMessage(EventMessage message);
	private:
		EventOutput *m_eventOut;
		std::mutex m_lock;
		EventQueue m_queuedMessages;
	};

	inline BaseEffect *TrackNodeFactory()
	{
		return new NoteTrackSource();
	}
}