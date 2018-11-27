#pragma once

#include "BaseEffect.h"
#include "Synthesis.h"
#include "VoiceProcessor.h"

namespace uComposer
{
	class DrumSynth : public BaseEffect
	{
	public:
		using BaseEffect::BaseEffect;

		// Inherited via BaseEffect
		virtual void Initialize() override;
		virtual void SetupProcessing() override;
		virtual void Process(ProcessData data, float dt) override;

	private:
		Envelope::ADSREnvelope m_ampEnv;

		Envelope::ADSREnvelope m_cutoffEnv;
		Filters::TB303LPF m_filter;

		Oscillator::Sine m_sine;
		Envelope::ADSREnvelope m_pitchEnv;

		float m_cutoff;
		float m_cutoffEnvAmt;
		float m_distortion;
		float m_postDistort;
		float m_pitchEnvAmt;
		float m_frequency;


		EventInput *m_eventIn;
		AudioOutput *m_audioOut;
	};

	inline BaseEffect *DrumSynthFactory()
	{
		return new DrumSynth();
	}
}