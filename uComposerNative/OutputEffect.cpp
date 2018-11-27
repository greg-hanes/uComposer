#include "OutputEffect.h"
#include <iostream>

namespace uComposer
{
	void OutputNode::SetOutputChannels(int channels)
	{
		m_outputChannels = channels;
	}

	void OutputNode::SetOutputBuffer(float *buffer)
	{
		m_outputBuffer = buffer;
	}

	void OutputNode::Initialize()
	{
		for (int i = 0; i < 6; i++)
		{
			m_inputs[i] = CreateAudioInput("Out " + std::to_string(i));
		}
	}

	void OutputNode::Process(ProcessData data, float dt)
	{
		for (int i = 0; i < m_outputChannels; i++)
		{
			for (uint32_t s = 0; s < data.nSamples; s++)
			{
				//m_outputBuffer[s * m_outputChannels + i] = data.audioBus.inputs[i][s];
				m_outputBuffer[s * m_outputChannels + i] = m_inputs[i]->Buffer()[s];
			}
		}
	}
}