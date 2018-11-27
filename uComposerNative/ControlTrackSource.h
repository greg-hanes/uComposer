#pragma once

#include "BaseEffect.h"

namespace uComposer
{
	class ControlTrackSource : public BaseEffect
	{
	public:
		using BaseEffect::BaseEffect;

		// Inherited via BaseEffect
		virtual void Initialize() override;
		virtual void Process(ProcessData data, float dt) override;

		void SendMessage(ControlMessage message);
	private:
		ControlOutput *m_controlOut;
		ControlQueue m_messageQueue;
	};

	inline BaseEffect *ControlTrackSourceFactory()
	{
		return new ControlTrackSource();
	}
}