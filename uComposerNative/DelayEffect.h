#pragma once

#include "BaseEffect.h"
#include "CircularBuffer.h"

namespace uComposer
{
	class DelayEffect : public BaseEffect
	{
	public:
		using BaseEffect::BaseEffect;

		void SetDelayAmount(uint32_t samples);
		void SetFeedback(float feedback);
		void SetWetMixAmount(float wetMix);
		void SetDryMixAmount(float dryMix);

		// Inherited via BaseEffect
		virtual void Initialize() override;
		virtual void SetupProcessing() override;
		virtual void Process(ProcessData data, float dt) override;

	private:
		AudioInput *m_audioIn;
		AudioOutput *m_audioOut;

		CircularBuffer m_delayQueue;
		float m_feedback = 0.0f;
		float m_wetMix = 0.5f;
		float m_dryMix = 0.5f;
	};

	inline BaseEffect *DelayNodeFactory()
	{
		return new DelayEffect();
	}
}