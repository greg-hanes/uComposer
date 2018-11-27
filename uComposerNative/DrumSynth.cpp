#include "DrumSynth.h"

namespace uComposer
{
	void DrumSynth::Initialize()
	{
		m_eventIn = CreateEventInput("Notes");
		m_audioOut = CreateAudioOutput("Out");
	}

	void DrumSynth::SetupProcessing()
	{
		m_ampEnv.SetAttack(1.0f, SampleRate());
		m_ampEnv.SetDecayTime(1000.0f, SampleRate());
		m_ampEnv.SetSustainLevel(0.0f);
		m_ampEnv.SetReleaseTime(0.0f, SampleRate());

		m_cutoffEnv.SetAttack(1.0f, SampleRate());
		m_cutoffEnv.SetDecayTime(5000.0f, SampleRate());
		m_cutoffEnv.SetSustainLevel(0.0f);
		m_cutoffEnv.SetReleaseTime(0.0f, SampleRate());

		m_pitchEnv.SetAttack(1.0f, SampleRate());
		m_pitchEnv.SetDecayTime(100.0f, SampleRate());
		m_pitchEnv.SetSustainLevel(0.0f);
		m_pitchEnv.SetReleaseTime(0.0f, SampleRate());

		m_cutoff = 140.0f;
		m_cutoffEnvAmt = 20000.0f;
		m_pitchEnvAmt = 200.0f;
		m_frequency = 57.0f;

		m_distortion = 4.0f;
		m_postDistort = 15.0f;
	}

	void DrumSynth::Process(ProcessData data, float dt)
	{
		EventQueuePtr noteEvents = m_eventIn->Events();

		for (uint32_t s = 0; s < data.nSamples; s++)
		{
			while (!noteEvents->Empty())
			{
				EventMessage *msg = noteEvents->Peek();
				if (msg->mOffset > s)
					break;

				noteEvents->Remove();

				switch (msg->type)
				{
				case EventType::kNoteOnEvent:
					m_ampEnv.Trigger();
					m_cutoffEnv.Trigger();
					m_pitchEnv.Trigger();
					m_frequency = uComposer::Math::NoteNumberToFrequency(msg->note.GetKey());
					m_filter.Reset();
					break;
				case EventType::kNoteOffEvent:
					break;
				}
			}

			
			
			m_cutoffEnv.Update();
			float cutoff = m_cutoff + m_cutoffEnvAmt * m_cutoffEnv.GetEnvelopeValue();
			m_filter.SetCutoff(cutoff / SampleRate());
			m_filter.SetResonance(0.95f);


			m_pitchEnv.Update();
			float sineFreq = m_frequency + m_pitchEnvAmt * m_pitchEnv.GetEnvelopeValue();
			m_sine.Update(sineFreq, dt);

			float outval = atan(m_sine.GetValue() * m_distortion) * (1.0f / Math::kPI);
			

			outval = atanf(m_postDistort * m_filter.Process(outval));

			m_ampEnv.Update();
			m_audioOut->Buffer()[s] = outval * m_ampEnv.GetEnvelopeValue();
		}
	}
}