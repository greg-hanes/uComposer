#include "TB303Synth.h"
#include "VecUtil.h"

namespace uComposer
{
	void TB303Synth::Initialize()
	{
		m_audioOut = CreateAudioOutput("Out");
		m_eventIn = CreateEventInput("Notes");

		// Note: control inputs aren't used
		m_ampEnvAttack = CreateControlInput("AmpEnv.Attack");
		m_ampEnvDecay = CreateControlInput("AmpEnv.Decay");
		m_ampEnvSustain= CreateControlInput("AmpEnv.Sustain");
		m_ampEnvRelease = CreateControlInput("AmpEnv.Release");
		m_filterResonance = CreateControlInput("Filter.Resonance");
		m_filterCutoff = CreateControlInput("Filter.Cutoff");
	}

	void TB303Synth::SetupProcessing()
	{
		m_ampEnvelope.SetAttack(10, SampleRate());
		m_ampEnvelope.SetDecayTime(100, SampleRate());
		m_ampEnvelope.SetReleaseTime(250, SampleRate());
		m_ampEnvelope.SetSustainLevel(0.8f);

		m_filterEnvelope.SetAttack(500, SampleRate());
		m_filterEnvelope.SetDecayTime(700, SampleRate());
		m_filterEnvelope.SetReleaseTime(250, SampleRate());
		m_filterEnvelope.SetSustainLevel(0.25f);


		m_filter.SetCutoff(6400.0f / SampleRate());
		m_filter.SetResonance(0.35f);
		m_glide = 0.999f;
		m_lfoFrequency = 0.1f;
	}

	void TB303Synth::Process(ProcessData data, float dt)
	{
		// foreach sample...
		for (uint32_t s = 0; s < data.nSamples; s++)
		{
			// Check for incoming events
			EventQueuePtr noteEvents = m_eventIn->Events();
			while (!noteEvents->Empty())
			{
				EventMessage *msg = noteEvents->Peek();
				if (msg->mOffset > s)
					break;

				noteEvents->Remove();

				switch (msg->type)
				{
				case EventType::kNoteOnEvent:
					m_playing = true;
					m_currentNote = msg->note.GetKey();
					m_velocity = msg->note.GetVelocity();
					m_targetFrequency = Math::NoteNumberToFrequency(m_currentNote);
					m_pressedKeys.push_back(msg->note.GetKey());
					m_ampEnvelope.Trigger();
					m_filterEnvelope.Trigger();
					break;
				case EventType::kNoteOffEvent:
					Key_t key = msg->note.GetKey();
					VecUtil::Remove(m_pressedKeys, key);
					if (m_currentNote == msg->note.GetKey())
					{
						// LIFO priority on keys (since this synth is non-polyphonic)
						if (m_pressedKeys.size() > 0)
						{
							m_currentNote = m_pressedKeys.at(m_pressedKeys.size() - 1);
							m_targetFrequency = Math::NoteNumberToFrequency(m_currentNote);
						}
						else
						{
							// No keysp ressed, so release the envelopes
							m_playing = false;
							m_ampEnvelope.Release();
							m_filterEnvelope.Release();
						}
					}

					break;
				}
			}

			// Control for envelope attack (unused)
			ControlQueuePtr ampEnvAttackControl = m_ampEnvAttack->ControlChanges();
			while (!ampEnvAttackControl->Empty())
			{
				ControlMessage *msg = ampEnvAttackControl->Peek();
				if (msg->mOffset > s)
					break;

				ampEnvAttackControl->Remove();

			}

			// Audio:

			// Update frequency based on m_glide (portamento). Kinda ghetto. Exponential decay from current to target.
			m_currentFrequency = (1 - m_glide) * m_targetFrequency + m_glide * m_currentFrequency;

			// Update amplifier envelope and filter envelope
			m_ampEnvelope.Update();
			m_filterEnvelope.Update();

			// Update LFO and oscillator
			m_lfo.Update(m_lfoFrequency, dt);
			m_oscillator.Update(m_currentFrequency, dt);

			// Update filter cutoff based on filter envelope and LFO
			float filterEnv = m_filterEnvelope.GetEnvelopeValue();
			m_filter.SetCutoff((m_lfo.GetValue() * 0.25f + 0.5f) * 0.5f * filterEnv);

			float prefilter = m_ampEnvelope.GetEnvelopeValue() * m_oscillator.GetValue();

			// atan for some distortion
			float filtered = Math::Bounded(atan(m_filter.Process(prefilter)), -1.0f, 1.0f);
			float value = m_velocity * m_ampEnvelope.GetEnvelopeValue() * filtered;

			// Write sample
			m_audioOut->Buffer()[s] = value;
		}
	}

	float TB303Synth::GetGlide() const
	{
		return m_glide;
	}

	void TB303Synth::SetGlide(float glide)
	{
		m_glide = glide;
	}

	float TB303Synth::GetFilterResonance() const
	{
		return (float)m_filter.GetResonance();
	}

	void TB303Synth::SetFilterResonance(float resonance)
	{
		m_filter.SetResonance(resonance);
	}

	float TB303Synth::GetFilterCutoff() const
	{
		return (float)m_filter.GetCutoff();
	}

	void TB303Synth::SetFilterCutoff(float cutoff)
	{
		m_filter.SetCutoff(cutoff);
	}
}