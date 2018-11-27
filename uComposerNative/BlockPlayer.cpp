#include "BlockPlayer.h"
#include "VecUtil.h"
#include "NoteTrack.h"
#include <iostream>

namespace uComposer
{
	BlockPlayer::BlockPlayer(AudioDeviceInterface *audioDevice, ProjectData *projectData) :
		m_audioDevice(audioDevice),
		m_tempo(projectData->GetTempo()),
		m_blocks(projectData->GetBlocks()),
		m_activeBlock(projectData->GetBlocks()->GetInitialBlock())
	{
	}

	// Should this check for transitions at every sample?
	//		Probably not...
	void BlockPlayer::Process()
	{
		double dt = m_audioDevice->GetSampleDeltaTime();
		for (uint32_t sample = 0; sample < (uint32_t)m_audioDevice->GetBufferSize(); sample++)
		{
			ProcessSample(dt, sample);
		}
	}

	void BlockPlayer::ProcessSample(double deltaWallTime, uint32_t sampleOffset)
	{
		// if the active block is the default block, then we need to check for transitions.
		// The default block is a special case. It has zero length, and immediately transitions
		//	to another block for seamless looping.
		if (m_activeBlock == m_blocks->GetDefaultBlock())
		{
			CheckBlockTransitions();
		}

		// If the active block is *still* the default block, then there's nothing we can really do.
		// Next time, we'll check to see if any transition conditions are true.
		if (m_activeBlock == m_blocks->GetDefaultBlock())
			return;

		m_lastTrackTime = m_currentTime;
		m_lastBPM = m_currentBPM;
		m_currentTime = m_lastTrackTime + TrackTime::FromSeconds(deltaWallTime, m_lastBPM);
		
		if (m_activeBlock->OverridesTempo())
		{
			m_currentBPM = m_activeBlock->GetTempoControlTrack().GetTempoAtTrackTime(m_currentTime);
		}
		else
		{
			m_currentBPM = m_tempo->GetBPM();
		}

		TrackTime dt = m_currentTime - m_lastTrackTime;

		// if dt == zero, there's no processing to be done.
		while (dt != TrackTime::Zero)
		{
			// if the active block is the default block, then we need to check for transitions.
			// The default block is a special case. It has zero length, and immediately transitions
			//	to another block for seamless looping.
			if (m_activeBlock == m_blocks->GetDefaultBlock())
			{
				CheckBlockTransitions();
			}

			// If the active block is *still* the default block, then there's nothing we can really do.
			// Next time, we'll check to see if any transition conditions are true.
			if (m_activeBlock == m_blocks->GetDefaultBlock())
			{
				return;
			}

			// If the end time is past the length,
			// process the tracks from startTime to block length.
			// Extra time (endTime - blockLength) still needs to be processed,
			//	but it will be processed on the next block.
			if (m_currentTime < m_activeBlock->GetLength())
			{
				ProcessTracks(m_lastTrackTime, m_currentTime, sampleOffset);
				return;
			}
			else // implicit condition: if (m_currentTime + dt >= m_activeBlock->GetLength())
			{
				ProcessTracks(m_lastTrackTime, m_activeBlock->GetLength(), sampleOffset);

				// Subtract the interval that we already processed from dt.
				dt -= (m_activeBlock->GetLength() - m_currentTime);
				m_currentBlockPlayCount++;

				if (m_currentBlockPlayCount >= m_targetBlockPlayCount)
				{
					if (!m_activeBlock->Loop())
					{
						m_activeBlock = m_blocks->GetDefaultBlock();
					}
					CheckBlockTransitions();
				}
			}
		}
	}

	void BlockPlayer::ProcessTracks(TrackTime startTime, TrackTime endTime, uint32_t sample)
	{
		for (auto track : m_activeBlock->NoteTracks())
		{
			track->Process(startTime, endTime, sample);
		}
		for (auto parameterTrack : m_activeBlock->ParameterTracks())
		{
			parameterTrack->Process(startTime, endTime, sample);
		}
	}

	void BlockPlayer::CheckBlockTransitions()
	{
		for (auto transition : m_activeBlock->Transitions())
		{
			if (transition->CanTransition())
			{
				PlayBlock(transition->Destination());
				
				m_currentBlockPlayCount = 0;
				m_targetBlockPlayCount = transition->GetPlayCount();
				break;
			}
		}
	}

	void BlockPlayer::PlayBlock(Block *block)
	{
		m_time = 0;
		m_currentTime = TrackTime::Zero;
		if (block->OverridesTempo())
		{
			m_currentBPM = block->GetTempoControlTrack().GetTempoAtTrackTime(TrackTime::Zero);
		}
		else
		{
			m_currentBPM = m_tempo->GetBPM();
		}

		m_activeBlock = block;
	}

	Block *BlockPlayer::GetActiveBlock()
	{
		return m_activeBlock;
	}

	void BlockPlayer::SetActiveBlock(Block *block)
	{
		PlayBlock(block);
	}

	void BlockPlayer::Stop()
	{
		SetActiveBlock(m_blocks->GetDefaultBlock());
	}
}