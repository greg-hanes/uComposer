#pragma once

#include <vector>
#include "AudioDeviceInterface.h"
#include "ProjectData.h"
#include "Block.h"

namespace uComposer
{
	class BlockPlayer
	{
	public:
		BlockPlayer(AudioDeviceInterface *audioDevice, ProjectData *projectData);
		void Process();

		Block *GetActiveBlock();
		void SetActiveBlock(Block *block);

		void Stop();

	private:
		void ProcessSample(double deltaWallTime, uint32_t sample);
		void ProcessTracks(TrackTime startTime, TrackTime endTime, uint32_t sample);
		void CheckBlockTransitions();

		void PlayBlock(Block *block);

		AudioDeviceInterface *m_audioDevice;
		double m_time = 0;
		TrackTime m_lastTrackTime;
		Tempo *m_tempo;
		
		BlockCollection *m_blocks;

		Block *m_activeBlock;
		TrackTime m_currentTime;
		double m_currentBPM = 0;
		double m_lastBPM = 0;


		int m_currentBlockPlayCount = 0;
		int m_targetBlockPlayCount = 0;
	};
}