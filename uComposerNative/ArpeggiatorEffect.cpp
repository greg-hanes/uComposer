
#include "ArpeggiatorEffect.h"
#include <iostream>

namespace uComposer
{
	void ArpeggiatorEffect::Initialize()
	{
		m_eventIn = CreateEventInput("In");
		m_eventOut = CreateEventOutput("Out");
	}

	void ArpeggiatorEffect::Process(ProcessData data, float dt)
	{
		EventQueuePtr inputQueue = m_eventIn->Events();
		EventQueuePtr outputQueue = m_eventOut->Events();

		while (!inputQueue->Empty())
		{
			EventMessage *msg = inputQueue->Peek();
			if (msg->mOffset > data.nSamples)
				return;

			inputQueue->Remove();

			outputQueue->Add(*msg);
			
			if (msg->type == kAllNotesOffEvent)
			{
				return;
			}

			uint32_t offset = 0;
			Key_t key = msg->note.GetKey();
			for (auto note : m_noteOffsets)
			{
				offset += (uint32_t)(msg->mOffset + (m_delay / dt));
				key += (Key_t)note;

				if (msg->type == kNoteOnEvent)
				{
					EventMessage noteOn = EventMessage::CreateNoteOn(offset, Note(key, msg->note.GetVelocity()));
					outputQueue->Add(noteOn);
				}
				else if (msg->type == kNoteOffEvent)
				{
					EventMessage noteOff = EventMessage::CreateNoteOff(offset, Note(key, msg->note.GetVelocity()));
					outputQueue->Add(noteOff);
				}
			}
		}
	}


	float &ArpeggiatorEffect::Delay()
	{
		return m_delay;
	}

	std::vector<int> &ArpeggiatorEffect::NoteOffsets()
	{
		return m_noteOffsets;
	}
}