#include "DelayEffect.h"
#include "MathUtil.h"
#include <iostream>

namespace uComposer
{
	void DelayEffect::Initialize()
	{
		m_audioIn = CreateAudioInput("In");
		m_audioOut = CreateAudioOutput("Out");
	}

	void DelayEffect::SetupProcessing()
	{

	}

	void DelayEffect::Process(ProcessData data, float dt)
	{
		// Ohhh. Don't use a queue. Just add into it backwards.
		for (uint32_t s = 0; s < data.nSamples; s++)
		{
			float in = m_audioIn->Buffer()[s];
			float delayed = m_delayQueue.Dequeue();
			float wet = m_wetMix * delayed;
			float dry = in * m_dryMix;

			m_audioOut->Buffer()[s] = wet + dry;
			m_delayQueue.Enqueue(in + m_feedback * (wet));
		}
	}

	void DelayEffect::SetDelayAmount(uint32_t samples)
	{
		m_delayQueue.SetSize((int)samples);
	}

	void DelayEffect::SetFeedback(float feedback)
	{
		m_feedback = feedback;
	}
	void DelayEffect::SetWetMixAmount(float wetMix)
	{
		m_wetMix = wetMix;
	}
	void DelayEffect::SetDryMixAmount(float dryMix)
	{
		m_dryMix = dryMix;
	}
}