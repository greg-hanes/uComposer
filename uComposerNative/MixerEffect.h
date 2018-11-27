#pragma once

#include "BaseEffect.h"

namespace uComposer
{
	class MixerEffect : public BaseEffect
	{
	public:
		using BaseEffect::BaseEffect;

		// Inherited via BaseEffect
		virtual void Initialize() override;
		virtual void Process(ProcessData data, float dt) override;

		// These need to be editor operations.
		void AddInputBus();
		void RemoveInputBus();
		void AddChannel();
		void RemoveChannel();

	private:
		int m_inputBusCount = 0;
		int m_channelsPerBus = 0;
	};

	inline BaseEffect *MixerNodeFactory()
	{
		return new MixerEffect();
	}
}