#pragma once

#include <vector>

#include "ProjectData.h"
#include "EffectGraph.h"
#include "BaseEffect.h"

namespace uComposer
{
	class AudioDeviceInterface;

	class GraphPlayer
	{
	public:
		GraphPlayer(AudioDeviceInterface *audioDevice, ProjectData *projectData);

		EffectGraph *GetEffectGraph() const;

		void Process(float *data);
		void Update();
		
	private:
		Tempo *m_tempo;

		float **m_audioBuffers = nullptr;
		void AllocateAudioBuffers(std::vector<BaseEffect *> effects);

		EventQueue *m_eventQueues = nullptr;
		void AllocateEventQueues(std::vector<BaseEffect *> effects);

		ControlQueue *m_controlQueues = nullptr;
		void AllocateControlQueues(std::vector<BaseEffect *> effects);

		void NodeAddedHandler(BaseEffect *node);
		void SampleRateChangedHandler(int sampleRate);
		void BufferSizeChangedHandler(int bufferSize);

		AudioDeviceInterface *m_audioDevice;
		EffectGraph *m_effectGraph;
		float m_dt;

		std::vector<BaseEffect *> m_processors;
		ProcessData m_processData;
	};
}