#include "MonoSynthEffect.h"
#include "VecUtil.h"

namespace uComposer
{
	void MonoSynthEffect::Initialize()
	{
		m_eventIn = CreateEventInput("Notes");
		m_audioOut = CreateAudioOutput("Out");
		m_gain = CreateControlInput("Gain");
		m_gain->SetValue(1.0f);

		RegisterComponent(&m_adsrController);
		RegisterComponent(&m_filter);
	}

	void MonoSynthEffect::SetupProcessing()
	{
		m_adsrConfig.SetSampleRate((float)SampleRate());
		m_adsrConfig.SetAttack(50);
		m_adsrConfig.SetDecayTime(2000);
		m_adsrConfig.SetSustainLevel(0.1f);
		m_adsrConfig.SetReleaseTime(1500);

		m_filter.GetCutoffControl()->SetValue(800.0f / SampleRate());
		m_filter.GetResonanceControl()->SetValue(1.0f);

		for (int i = 0; i < 32; i++)
			m_voiceProcessor.AddVoice(MonoSynthVoice(&m_adsrConfig, SampleRate()));

		m_voiceProcessor.SetupProcessing();
	}

	void MonoSynthEffect::Process(ProcessData data, float dt)
	{	
		for (uint32_t s = 0; s < data.nSamples; s++)
		{
			//m_adsrController.Update(s);
			m_gain->Update(s);
			ProcessEvents(m_eventIn->Events(), s);

			m_voiceProcessor.Process(m_audioOut->Buffer(), dt, s);
			m_audioOut->Buffer()[s] *= m_gain->GetValue();
			//m_audioOut->Buffer()[s] = m_filter.Process(s, m_audioOut->Buffer()[s]);
		}
	}

	void MonoSynthEffect::ProcessEvents(EventQueuePtr noteEvents, uint32_t sample)
	{
		while (!noteEvents->Empty())
		{
			EventMessage *msg = noteEvents->Peek();

			if (msg->mOffset > sample)
				break;

			noteEvents->Remove();

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

	void MonoSynthEffect::ProcessControls(ControlQueuePtr controls, uint32_t sample)
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
}