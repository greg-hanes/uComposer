#pragma once

#include "BaseEffect.h"
#include "Synthesis.h"
#include "VoiceProcessor.h"
#include "TB303LPFComponent.h"
#include "ADSREnvelopeComponent.h"
#include <iostream>
#include "KarplusStrongString.h"
#include "DigitalWaveGuide.h"
#include "DWGPiano.h"

namespace uComposer
{
	class ADSRControlGroup : public SynthComponent
	{
	public:
		ADSRControlGroup(ADSR::Configuration &configuration) :
			m_configuration(configuration)
		{
		}

		void Update(uint32_t sampleOffset)
		{
			if (m_adsrAttack->Update(sampleOffset))
				m_configuration.SetAttack(m_adsrAttack->GetValue());

			if (m_adsrDecay->Update(sampleOffset))
				m_configuration.SetDecayTime(m_adsrDecay->GetValue());

			if (m_adsrSustain->Update(sampleOffset))
				m_configuration.SetSustainLevel(m_adsrSustain->GetValue());

			if (m_adsrRelease->Update(sampleOffset))
				m_configuration.SetReleaseTime(m_adsrRelease->GetValue());
		}

		virtual void OnRegister(BaseEffect *effect) override
		{
			m_adsrAttack = effect->CreateControlInput("ADSR/Attack Time (ms)");
			m_adsrDecay = effect->CreateControlInput("ADSR/Decay Time (ms)");
			m_adsrSustain = effect->CreateControlInput("ADSR/Sustain Level");
			m_adsrRelease = effect->CreateControlInput("ADSR/Release Time (ms)");
		}

		virtual void OnUnregister(BaseEffect *effect) override
		{
			effect->RemoveControlInput(m_adsrAttack);
			effect->RemoveControlInput(m_adsrDecay);
			effect->RemoveControlInput(m_adsrSustain);
			effect->RemoveControlInput(m_adsrRelease);
		}

	private:
		ADSR::Configuration &m_configuration;

		ControlInput *m_adsrAttack;
		ControlInput *m_adsrDecay;
		ControlInput *m_adsrSustain;
		ControlInput *m_adsrRelease;
	};

	class MonoSynthEffect : public BaseEffect
	{
	public:
		MonoSynthEffect() :
			m_adsrConfig(),
			m_adsrController(m_adsrConfig)
		{

		}

		// Inherited via BaseEffect
		virtual void Initialize() override;
		virtual void SetupProcessing() override;
		virtual void Process(ProcessData data, float dt) override;

	private:
		void ProcessEvents(EventQueuePtr events, uint32_t sample);
		void ProcessControls(ControlQueuePtr controls, uint32_t sample);

		ADSR::Configuration m_adsrConfig;
		AudioOutput *m_audioOut;
		EventInput *m_eventIn;
		ADSRControlGroup m_adsrController;

		ControlInput *m_gain;

		TB303LPFComponent m_filter;

		struct MonoSynthVoice
		{
			MonoSynthVoice(ADSR::Configuration *adsrConfig, int sampleRate) :
				m_adsrConfig(adsrConfig),
				m_sampleRate((float)sampleRate)
			{
				m_string.Initialize(m_sampleRate);

				m_filterEnvelopeConfig.SetSampleRate(m_sampleRate);
				m_filterEnvelopeConfig.SetAttack(50);
				m_filterEnvelopeConfig.SetDecayTime(200);
				m_filterEnvelopeConfig.SetSustainLevel(0.5f);
				m_filterEnvelopeConfig.SetReleaseTime(1500);
			}
			

			Key_t NoteKey()
			{
				return m_notePitch;
			}

			bool IsActive()
			{
				return m_active;
			}

			void SetupProcessing()
			{
				m_sine.Reset();
				m_tri.Reset();
				m_saw.Reset();
				m_square.Reset();
				m_tri2.Reset();

				m_notePitch = 0;
				m_noteFrequency = 0;
				m_active = false;
			}

			void Trigger(Note note)
			{
				m_notePitch = note.GetKey();
				m_noteFrequency = uComposer::Math::NoteNumberToFrequency(m_notePitch);
				m_velocity = note.GetVelocity();
				std::cout << "velocity: " << m_velocity << std::endl;
				m_adsrState.Trigger();
				m_filterEnvelope.Trigger();

				m_active = true;
				m_string.SetDecay(1.0f);
				m_piano.SetDecay(1.0f);
				m_piano.Pluck(1.0f, m_noteFrequency, m_sampleRate);
				m_string.Pluck(1.0f, m_noteFrequency, m_sampleRate);

				m_sine.Reset();
				m_tri.Reset();
			}

			void Release(Note note)
			{
				m_adsrState.Release();
				m_filterEnvelope.Release();
				m_string.SetDecay(0.95f);
				m_piano.SetDecay(0.98f);
			}

			void Process(float *buffer, float dt, uint32_t sample)
			{
				m_sine.Update(0.5f, dt);
				m_tri.Update(0.1f, dt);
				m_saw.Update(m_noteFrequency, dt);
				m_square.Update(m_noteFrequency, dt);
				m_tri2.Update(m_noteFrequency, dt);

				ADSR::Update(*m_adsrConfig, m_adsrState);
				ADSR::Update(m_filterEnvelopeConfig, m_filterEnvelope);

				if (m_adsrState.GetState() == Envelope::ADSREnvelopeMode::Idle)
				{
					m_active = false;
				}

				// 0 to 1
				//float cutoff = 0.5f * m_tri.GetValue() + 0.5f;

				float cutoff = 1 - m_filterEnvelope.GetEnvelopeValue();

				float cutoffMin = 10000.0f;
				float cutoffMax = 5000.0f;

				cutoff *= (cutoffMax - cutoffMin);
				cutoff += cutoffMin;
				cutoff = cutoff / m_sampleRate;
				//cutoff = Math::FastSine(Math::kPI * 0.5f * cutoff / m_sampleRate);

				m_filter.SetCutoff(cutoff);

				// 0 to 1
				float res = 0.5f * m_tri.GetValue() + 0.5f;
				// 0 to 0.2
				res *= 0.2f;
				res += 0.5f;

				m_filter.SetResonance(0.15f);
				

				//buffer[sample] += 0.5f * m_velocity * m_adsrState.GetEnvelopeValue() * Math::Bounded(0.8f * atan(m_filter.Process(m_saw.GetValue())), -1.0f, 1.0f);
				buffer[sample] += m_velocity * 0.5f * m_adsrState.GetEnvelopeValue() * Math::Bounded(0.5f * atan(m_filter.Process(m_piano.Update())), -1.0f, 1.0f);
				//buffer[sample] += m_filter.Process(m_string.Update());
				//buffer[sample] += m_string.Update();
				//buffer[sample] += m_piano.Update();
			}

			Key_t m_notePitch;
			float m_noteFrequency;
			float m_velocity;
			bool m_active;

			float m_sampleRate;
			Oscillator::Sine m_sine;
			Oscillator::Triangle m_tri2;
			Oscillator::Triangle m_tri;
			Oscillator::Sawtooth m_saw;
			Oscillator::Square m_square;
			Filters::TB303LPF m_filter;
			ADSR::Configuration *m_adsrConfig;
			ADSR::Configuration m_filterEnvelopeConfig;
			ADSR::State m_adsrState;
			ADSR::State m_filterEnvelope;
			DigitalWaveGuide m_string;
			DWGPiano m_piano;
		};

		VoiceProcessor<MonoSynthVoice> m_voiceProcessor;

	};

	inline BaseEffect *MonoSynthFactory()
	{
		return new MonoSynthEffect();
	}
}