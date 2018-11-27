#include "MixerEffect.h"

namespace uComposer
{
	void MixerEffect::Initialize()
	{
		m_inputBusCount = 0;
		m_channelsPerBus = 0;

		// Create two channels.
		AddChannel();
		AddChannel();

		// Add two input busses.
		AddInputBus();
		AddInputBus();
	}

	void MixerEffect::Process(ProcessData data, float dt)
	{
		// foreach sample,
		for (uint32_t s = 0; s < data.nSamples; s++)
		{
			// Set the output sample to be zero on all output channels.
			for (AudioOutput *audioOut : AudioOutputs())
			{
				audioOut->Buffer()[s] = 0;
			}

			// foreach input,
			int index = 0;
			for (AudioInput *audioIn : AudioInputs())
			{
				AudioOutputs().at(index % m_channelsPerBus)->Buffer()[s] += audioIn->Buffer()[s];
			}
		}
	}

	void MixerEffect::AddInputBus()
	{
		for (int c = 0; c < m_channelsPerBus; c++)
		{
			CreateAudioInput("Input " + std::to_string(m_inputBusCount) + " C" + std::to_string(c));
		}
		m_inputBusCount++;
	}

	void MixerEffect::RemoveInputBus()
	{
		if (m_inputBusCount == 2)
			return;

		for (int c = 0; c < m_channelsPerBus; c++)
		{
			// Index of last audio input is m_inputBusCount * m_channelsPerBus - 1
			RemoveAudioInput(m_inputBusCount * m_channelsPerBus - c - 1);
		}
		m_inputBusCount--;
	}

	void MixerEffect::AddChannel()
	{
		for (int b = m_inputBusCount - 1; b >= 0; b--)
		{
			// Find index for this bus where a new input will be inserted.
			int insertIndex = b * m_channelsPerBus + m_channelsPerBus;

			std::string inputName = "Input " + std::to_string(b) + " C" + std::to_string(m_channelsPerBus);
			InsertAudioInput(inputName, insertIndex);
		}

		// Add audio outputs as well.
		CreateAudioOutput("C" + std::to_string(m_channelsPerBus));

		m_channelsPerBus++;
	}

	void MixerEffect::RemoveChannel()
	{
		if (m_channelsPerBus == 1)
			return;

		int channelToRemove = m_channelsPerBus - 1;
		for (int b = m_inputBusCount - 1; b >= 0; b--)
		{
			int busIndex = b * m_channelsPerBus; // index of first channel in bus.
			int removeIndex = busIndex + channelToRemove;
			RemoveAudioInput(removeIndex);
		}


		RemoveAudioOutput(m_channelsPerBus - 1);


		m_channelsPerBus--;
	}
}