#include "PlaybackController.h"
#include "VecUtil.h"

#include <iostream>

namespace uComposer
{
	PlaybackController::PlaybackController(AudioDeviceInterface *audioDevice, ProjectData *projectData)
	{
		m_audioDevice = audioDevice;
		m_tempo = projectData->GetTempo();
		
		m_graphController = new GraphPlayer(audioDevice, projectData);
		m_blockController = new BlockPlayer(audioDevice, projectData);
	}

	BlockPlayer *PlaybackController::GetBlockController() const
	{
		return m_blockController;
	}

	GraphPlayer *PlaybackController::GetGraphController() const
	{
		return m_graphController;
	}

	// How to handle playing blocks? At some point, there will need to be a playback state
	// machine.
	void PlaybackController::Play()
	{
		m_state = PlaybackState::Playing;
	}

	void PlaybackController::Pause()
	{
		if (m_state == PlaybackState::Playing)
		{
			m_state = PlaybackState::Paused;
		}
	}

	void PlaybackController::Stop()
	{
		m_state = PlaybackState::Stopped;
		m_time = 0;
		m_blockController->Stop();
	}

	PlaybackState PlaybackController::State() const
	{
		return m_state;
	}

	double PlaybackController::CurrentTime() const
	{
		return m_time;
	}

	void PlaybackController::Process(float *buffer)
	{
		if (m_state == PlaybackState::Playing)
		{
			m_blockController->Process();
		}
		m_graphController->Process(buffer);
	}
}