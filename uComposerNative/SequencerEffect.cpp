#include "SequencerEffect.h"
#include <iostream>

namespace uComposer
{
	void SequencerEffect::Initialize()
	{
		m_eventOut = CreateEventOutput("Notes");
	}

	void SequencerEffect::Process(ProcessData data, float dt)
	{
		if (m_eventMessages.size() == 0) return;
		//EventQueuePtr out = data.eventBus.outputs[0];

		int startSampleOffset = m_sampleOffset;
		int endSampleOffset = m_sampleOffset + data.nSamples;

		while (true)
		{
			int currentMessageOffset = (int)(m_eventMessages.at(m_currentIndex).mOffset);
			if (currentMessageOffset > endSampleOffset)
				break;

			// Calculate offset relative to the buffer we're processing.
			int relativeOffset = currentMessageOffset - startSampleOffset;

			// Copy the message with the relative offset.
			EventMessage outMsg = EventMessage(m_eventMessages.at(m_currentIndex));
			outMsg.mOffset = relativeOffset;
			// Push the message to the output queue.
			m_eventOut->Events()->Add(outMsg);
			//out->Add(outMsg);

			m_currentIndex++;

			// Wrap around event message sequence
			if (m_currentIndex >= m_eventMessages.size())
			{
				m_currentIndex = 0;
				m_sampleOffset -= m_sequencerLength;
				endSampleOffset -= m_sequencerLength;
			}
		}

		m_sampleOffset += data.nSamples;
	}

	std::vector<EventMessage> &SequencerEffect::GetSequencerMessages()
	{
		return m_eventMessages;
	}

	int SequencerEffect::GetSequenceLength() const
	{
		return m_sequencerLength;
	}

	void SequencerEffect::SetSequenceLength(int length)
	{
		m_sequencerLength = length;
	}
}