#include "NoteTrackSource.h"
#include <iostream>

namespace uComposer
{
	void NoteTrackSource::Initialize()
	{
		m_eventOut = CreateEventOutput("Track Events");
	}

	void NoteTrackSource::Process(ProcessData data, float dt)
	{
		std::lock_guard<std::mutex> lock(m_lock);

		while (!m_queuedMessages.Empty())
		{
			EventMessage *msg = m_queuedMessages.Peek();
			if (msg->mOffset > data.nSamples)
				return;

			m_queuedMessages.Remove();

			//outputQueue->Add(*msg);
			m_eventOut->Events()->Add(*msg);
		}

		m_queuedMessages.Flush<true>(data.nSamples);
	}

	void NoteTrackSource::SendMessage(EventMessage message)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_queuedMessages.Add(message);
	}
}