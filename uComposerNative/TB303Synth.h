#pragma once

#include "BaseEffect.h"
#include "Synthesis.h"

namespace uComposer
{
	class TB303Synth : public BaseEffect
	{
	public:
		using BaseEffect::BaseEffect;

		// Inherited via BaseEffect
		virtual void Initialize() override;
		virtual void SetupProcessing() override;
		virtual void Process(ProcessData data, float dt) override;

		float GetGlide() const;
		void SetGlide(float glide);

		float GetFilterResonance() const;
		void SetFilterResonance(float resonance);

		float GetFilterCutoff() const;
		void SetFilterCutoff(float cutoff);

	private:
		AudioOutput *m_audioOut;

		EventInput *m_eventIn;

		ControlInput *m_ampEnvAttack;
		ControlInput *m_ampEnvDecay;
		ControlInput *m_ampEnvSustain;
		ControlInput *m_ampEnvRelease;
		ControlInput *m_filterResonance;
		ControlInput *m_filterCutoff;

		bool m_playing;
		float m_currentFrequency;
		float m_targetFrequency;
		Key_t m_currentNote;
		float m_velocity;
		std::vector<Key_t> m_pressedKeys;

		float m_glide;
		float m_lfoFrequency;
		Envelope::ADSREnvelope m_ampEnvelope;
		Envelope::ADSREnvelope m_filterEnvelope;
		Oscillator::Square m_oscillator;
		Oscillator::Sine m_lfo;
		Filters::TB303LPF m_filter;
	};

	inline BaseEffect *TB303SynthFactory()
	{
		return new TB303Synth();
	}
}