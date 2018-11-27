#include "TB303FilterEffect.h"

void uComposer::TB303FilterEffect::Initialize()
{
	m_input = CreateAudioInput("Input");
	m_output = CreateAudioOutput("Output");
}

void uComposer::TB303FilterEffect::SetupProcessing()
{
	m_filter.SetCutoff(7000.0f / SampleRate());
	m_filter.SetResonance(0.1f);
}

void uComposer::TB303FilterEffect::Process(ProcessData data, float dt)
{
	for (uint32_t s = 0; s < data.nSamples; s++)
	{
		float in = m_input->Buffer()[s];

		m_osc.Update(2.0f, dt);

		// 0 to 1
		float cutoff = 0.5f * m_osc.GetValue() + 0.5f;

		float cutoffMin = 3000.0f;
		float cutoffMax = 17000.0f;

		cutoff *= (cutoffMax - cutoffMin);
		cutoff += cutoffMin;

		cutoff /= SampleRate();

		m_filter.SetCutoff(cutoff);
		m_output->Buffer()[s] = m_filter.Process(in);
	}
}
