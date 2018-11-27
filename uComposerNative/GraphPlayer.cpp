#include <map>

#include "GraphPlayer.h"
#include "AudioDeviceInterface.h"
#include "OutputEffect.h"
#include "NoteTrackSource.h"
#include "EffectDescriptor.h"

namespace uComposer
{
	GraphPlayer::GraphPlayer(AudioDeviceInterface *audioDevice, ProjectData *projectData) :
		m_audioDevice(audioDevice),
		m_tempo(projectData->GetTempo()),
		m_effectGraph(projectData->GetEffectGraph()),
		m_dt(1.0f / audioDevice->GetSampleRate())
	{

		m_audioDevice->SampleRateChanged.Register(this, &GraphPlayer::SampleRateChangedHandler);
		m_audioDevice->BufferSizeChanged.Register(this, &GraphPlayer::BufferSizeChangedHandler);

		m_effectGraph->NodeAddedEvent().Register(this, &GraphPlayer::NodeAddedHandler);


		m_effectGraph->GetOutputNode()->SetOutputChannels(audioDevice->GetOutputChannelCount());
		for (auto n : m_effectGraph->GetNodes())
		{
			n->SetSampleRate(audioDevice->GetSampleRate());
			n->SetupProcessing();
		}

		Update();
	}

	EffectGraph *GraphPlayer::GetEffectGraph() const
	{
		return m_effectGraph;
	}

	void GraphPlayer::Process(float *data)
	{
		m_effectGraph->GetOutputNode()->SetOutputBuffer(data);

		//  double t = m_audioDevice->GetSampleDeltaTime() * m_audioDevice->GetBufferSize();
		//	m_time += t;
		//	TrackTime trackTime = TrackTime::FromSeconds(m_time, m_tempo->GetBPM());
		//  m_processData.trackTime = trackTime;

		for (int i = 0; i < m_processors.size(); i++)
		{
			m_processors.at(i)->Process(m_processData, m_dt);
			m_processors.at(i)->FlushQueues(m_processData);
		}
	}

	void GraphPlayer::Update()
	{
		m_processors.clear();
		m_effectGraph->GetFlattenedGraph(m_processors);


		///////////////////////////////////////
		// Allocate node playback resouces
		AllocateAudioBuffers(m_processors);
		AllocateEventQueues(m_processors);
		AllocateControlQueues(m_processors);

		m_processData.nSamples = m_audioDevice->GetBufferSize();
	}

	void GraphPlayer::SampleRateChangedHandler(int sampleRate)
	{
		m_dt = 1.0f / sampleRate;

		for (auto node : m_effectGraph->GetNodes())
		{
			node->SetSampleRate(sampleRate);
		}
	}

	void GraphPlayer::BufferSizeChangedHandler(int bufferSize)
	{
		// TODO reallocate exist buffers.
	}

	void GraphPlayer::NodeAddedHandler(BaseEffect *node)
	{
		node->SetSampleRate(m_audioDevice->GetSampleRate());
	}

	void GraphPlayer::AllocateAudioBuffers(std::vector<BaseEffect *> effects)
	{
		// Each element represents a buffer to be allocated, and the value represents
		//	a running total of how many nodes will yet need to read from the buffer.
		std::vector<int> bufferSemaphores;

		// Mapping from each AudioOutput to the index of the buffer it will write to.
		std::map<AudioOutput *, int> bufferIndexByOutput;
		// Mapping from each AudioInput to the index of the buffer it will read from.
		std::map<AudioInput *, int> bufferIndexByInput;

		for (auto node : effects)
		{
			//std::cout << node->Type()->EffectName() << std::endl;
			for (auto output : node->AudioOutputs())
			{
				// If we have an unused buffer, use that.
				// If not, allocate a new buffer and set it's semaphore to
				//		the number of nodes containing an input reading from
				//		the current output.
				// Create mapping from current output to this buffer.
				int bufferIndex;
				for (bufferIndex = 0; bufferIndex < bufferSemaphores.size(); bufferIndex++)
				{
					if (bufferSemaphores[bufferIndex] == 0)
						break;
				}

				if (bufferIndex == bufferSemaphores.size())
				{
					// Create a new buffer.
					int destinationNodeCount = m_effectGraph->GetConnectionList().GetOutputDestinationNodeCount(output);
					bufferSemaphores.push_back(destinationNodeCount);
					bufferIndexByOutput.insert({ output, bufferIndex });
				}
				else
				{
					// already have a buffer.
					bufferIndexByOutput.insert({ output, bufferIndex });
				}
			}

			std::vector<AudioOutput *> audioSources;
			m_effectGraph->GetConnectionList().GetSources(node, audioSources);

			for (auto sourceOutput : audioSources)
			{
				int bufferIndex = bufferIndexByOutput.at(sourceOutput);
				bufferSemaphores[bufferIndex]--;
			}
			
		}

		// Plus one for a dedicated empty buffer.
		int bufferCount = bufferSemaphores.size() + 1;

		if (m_audioBuffers != nullptr)
		{
			Memory::Delete2DArray(m_audioBuffers);
		}

		m_audioBuffers = Memory::Create2DArray(bufferCount, m_audioDevice->GetBufferSize());

		for (auto node : effects)
		{
			for (auto output : node->AudioOutputs())
			{
				output->m_buffer = m_audioBuffers[bufferIndexByOutput.at(output)];
			}

			for (auto input : node->AudioInputs())
			{
				AudioOutput *source = static_cast<AudioOutput*>(m_effectGraph->GetConnectionList().GetSource(input));

				if (source != nullptr)
				{
					input->m_buffer = m_audioBuffers[bufferIndexByOutput.at(source)];
				}
				else
				{
					input->m_buffer = m_audioBuffers[bufferCount - 1];
				}
			}
		}
	}

	void GraphPlayer::AllocateEventQueues(std::vector<BaseEffect *> effects)
	{
		int eventQueueCount = 0;

		for (auto node : effects)
		{
			for (EventOutput *output : node->EventOutputs())
			{
				eventQueueCount++;
			}
		}

		if (m_eventQueues != nullptr)
		{
			delete[] m_eventQueues;
		}

		m_eventQueues = new EventQueue[eventQueueCount + 1];

		int index = 0;
		for (auto node : effects)
		{
			for (EventOutput * output : node->EventOutputs())
			{
				output->m_events = m_eventQueues + index;
				index++;
			}
			for (EventInput *input : node->EventInputs())
			{
				EventOutput *source = static_cast<EventOutput *>(m_effectGraph->GetConnectionList().GetSource(input));
				if (source != nullptr)
				{
					input->m_events = source->m_events;
				}
				else
					input->m_events = m_eventQueues + eventQueueCount;
			}
		}
	}

	void GraphPlayer::AllocateControlQueues(std::vector<BaseEffect *> effects)
	{
		int controlQueueCount = 0;
		for (auto node : effects)
		{
			for (ControlOutput *output : node->ControlOutputs())
			{
				controlQueueCount++;
			}
		}

		delete[] m_controlQueues;
		m_controlQueues = new ControlQueue[controlQueueCount + 1];
		int index = 0;
		for (auto node : effects)
		{
			for (ControlOutput *output : node->ControlOutputs())
			{
				output->m_controlChanges = m_controlQueues + index;
				index++;
			}
			for (ControlInput *input : node->ControlInputs())
			{
				ControlOutput *source = static_cast<ControlOutput *>(m_effectGraph->GetConnectionList().GetSource(input));
				if (source != nullptr)
					input->m_controlChanges = source->m_controlChanges;
				else
					input->m_controlChanges = m_controlQueues + controlQueueCount;
			}
		}
	}
}