#pragma once

#include <vector>
#include <queue>
#include "Note.h"
#include "VecUtil.h"

namespace uComposer
{
	//struct ExampleVoice
	//{
	//	Key_t NoteKey();
	//	bool IsActive();
	//	void SetupProcessing(int sampleRate);
	//	void Trigger(Note note);
	//	void Release(Note note);
	//	void Process(AudioBus bus, float dt, uint32_t sample);
	//};

	template<class TVoice>
	class VoiceProcessor
	{
	public:
		void AddVoice(TVoice voice)
		{
			m_inactiveVoices.push_back(voice);
		}

		void SetupProcessing()
		{
			for (size_t i = 0; i < m_inactiveVoices.size(); i++)
			{
				m_inactiveVoices.at(i).SetupProcessing();
			}
		}

		void OnNoteOn(Note note)
		{
			for (size_t i = 0; i < m_voices.size(); i++)
			{
				TVoice &v = m_voices.at(i);
				if (v.NoteKey() == note.GetKey())
				{
					v.Trigger(note);
					goto Skip;
				}
			}

			if (m_inactiveVoices.size() > 0)
			{
				TVoice v = m_inactiveVoices.at(m_inactiveVoices.size() - 1);
				VecUtil::RemoveAt(m_inactiveVoices, m_inactiveVoices.size() - 1);
				v.Trigger(note);
				m_voices.push_back(v);
			}

		Skip:
			return;
		}

		void OnNoteOff(Note note)
		{
			for (size_t i = 0; i < m_voices.size(); i++)
			{
				TVoice &v = m_voices.at(i);
				if (v.NoteKey() == note.GetKey())
				{
					v.Release(note);
					break;
				}
			}
		}

		void KillAllNotes()
		{
			for (size_t i = 0; i < m_voices.size(); i++)
			{
				TVoice &v = m_voices.at(i);
				Note note = Note(v.NoteKey(), 1);
				v.Release(note);
			}
		}

		void Process(float *buffer, float dt, uint32_t sample)
		{
			buffer[sample] = 0.0f;
			for (size_t i = 0; i < m_voices.size(); i++)
			{
				TVoice &v = m_voices.at(i);
				v.Process(buffer, dt, sample);
				if (!v.IsActive())
				{
					m_inactiveVoices.push_back(v);
					VecUtil::RemoveAt(m_voices, i);
					i--;
				}
			}
		}


	private:
		std::vector<TVoice> m_voices;
		std::vector<TVoice> m_inactiveVoices;
	};
}