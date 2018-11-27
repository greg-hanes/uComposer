#include "ControlTrackSource.h"

namespace uComposer
{
	void ControlTrackSource::Initialize()
	{
		m_controlOut = CreateControlOutput("Control Events");
	}

	void ControlTrackSource::Process(ProcessData data, float dt)
	{
		//ControlQueuePtr outputQueue = data.controlBus.outputs[0];

		while (!m_messageQueue.Empty())
		{
			ControlMessage *msg = m_messageQueue.Peek();
			if (msg->mOffset > data.nSamples)
				break;;

			m_messageQueue.Remove();

			m_controlOut->ControlChanges()->Add(*msg);
			//outputQueue->Add(*msg);
		}

		m_messageQueue.Flush<true>(data.nSamples);
	}

	void ControlTrackSource::SendMessage(ControlMessage message)
	{
		m_messageQueue.Add(message);
	}
}