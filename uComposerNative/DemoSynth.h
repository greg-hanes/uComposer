#pragma once

#include "BaseEffect.h"
#include "Synthesis.h"
#include "VoiceProcessor.h"

namespace uComposer
{
	class DemoSynthEffect : public BaseEffect
	{
	public:
		DemoSynthEffect() :
			m_adsrConfig()
		{

		}

		// Inherited via BaseEffect
		inline virtual void Initialize() override
		{
			m_eventIn = CreateEventInput("Notes");
			m_audioOut = CreateAudioOutput("Out");
		}

		inline virtual void SetupProcessing() override
		{
			m_adsrConfig.SetSampleRate(SampleRate());
			m_adsrConfig.SetAttack(50);
			m_adsrConfig.SetDecayTime(2000);
			m_adsrConfig.SetSustainLevel(0.1f);
			m_adsrConfig.SetReleaseTime(1500);

			for (int i = 0; i < 32; i++)
				m_voiceProcessor.AddVoice(DemoSynthVoice(&m_adsrConfig, SampleRate()));

			m_voiceProcessor.SetupProcessing();
		}

		inline virtual void Process(ProcessData data, float dt) override
		{
			for (uint32_t s = 0; s < data.nSamples; s++)
			{
				ProcessEvents(m_eventIn->Events(), s);

				m_voiceProcessor.Process(m_audioOut->Buffer(), dt, s);
			}
		}

	private:
		inline void ProcessEvents(EventQueuePtr events, uint32_t sample)
		{
			while (!events->Empty())
			{
				EventMessage *msg = events->Peek();

				if (msg->mOffset > sample)
					break;

				events->Remove();

				switch (msg->type)
				{
				case EventType::kNoteOnEvent:
					m_voiceProcessor.OnNoteOn(msg->note);
					break;
				case EventType::kNoteOffEvent:
					m_voiceProcessor.OnNoteOff(msg->note);
					break;
				case EventType::kAllNotesOffEvent:
					m_voiceProcessor.KillAllNotes();
					break;
				}
			}
		}

		inline void ProcessControls(ControlQueuePtr controls, uint32_t sample)
		{
			while (!controls->Empty())
			{
				ControlMessage *msg = controls->Peek();
				if (msg->mOffset > sample)
					break;

				controls->Remove();

				// TODO
			}
		}


		ADSR::Configuration m_adsrConfig;
		AudioOutput *m_audioOut;
		EventInput *m_eventIn;
		
		
		struct DemoSynthVoice
		{
			DemoSynthVoice(ADSR::Configuration *adsrConfig, int sampleRate) :
				m_adsrConfig(adsrConfig),
				m_sampleRate(sampleRate)
			{
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
				m_adsrState.Trigger();
				m_active = true;

				m_sine.Reset();
				m_tri.Reset();
			}

			void Release(Note note)
			{
				m_adsrState.Release();
			}

			void Process(float *buffer, float dt, uint32_t sample)
			{
				m_sine.Update(0.5f, dt);
				m_tri.Update(0.1f, dt);
				m_saw.Update(m_noteFrequency, dt);
				m_square.Update(m_noteFrequency, dt);
				m_tri2.Update(m_noteFrequency, dt);
				ADSR::Update(*m_adsrConfig, m_adsrState);
				if (m_adsrState.GetState() == Envelope::ADSREnvelopeMode::Idle)
				{
					m_active = false;
				}

				// 0 to 1
				//float cutoff = 0.5f * m_tri.GetValue() + 0.5f;

				float cutoff = 1 - m_adsrState.GetEnvelopeValue();

				float cutoffMin = 10000.0f;
				float cutoffMax = 100.0f;

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
				//buffer[sample] += m_adsrState.GetEnvelopeValue() * Math::Bounded(0.8f * atan(m_filter.Process(m_string.Update())), -1.0f, 1.0f);
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
			ADSR::State m_adsrState;
		};

		VoiceProcessor<DemoSynthVoice> m_voiceProcessor;
		

	};

	inline BaseEffect *DemoSynthFactory()
	{
		return new DemoSynthEffect();
	}
}