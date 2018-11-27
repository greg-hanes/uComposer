#pragma once

#include <vector>
#include "AudioDeviceInterface.h"
#include "ProjectData.h"
#include "BlockPlayer.h"
#include "GraphPlayer.h"

namespace uComposer
{
	enum PlaybackState
	{
		Playing,
		Paused,
		Stopped
	};

	class PlaybackController
	{
	public:
		PlaybackController(AudioDeviceInterface *deviceInterface, ProjectData *projectData);

		BlockPlayer *GetBlockController() const;
		GraphPlayer *GetGraphController() const;

		PlaybackState State() const;
		double CurrentTime() const;

		void Play();
		void Pause();
		void Stop();

		void Process(float *buffer);

	private:
		PlaybackState m_state = PlaybackState::Stopped;
		double m_time;
		TrackTime m_lastTrackTime;

		AudioDeviceInterface *m_audioDevice;
		Tempo *m_tempo;
		BlockPlayer *m_blockController;
		GraphPlayer *m_graphController;

	};
}