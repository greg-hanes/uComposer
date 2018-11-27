#pragma once

#include <vector>
#include <string>
#include "Tempo.h"
#include "NoteTrack.h"
#include "ControlTrack.h"
#include "BlockTransition.h"
#include "TempoControlTrack.h"

namespace uComposer
{
	class BlockTransitionCondition
	{
	public:
		// ??
		bool IsSatisfied()
		{
			return false;
		}
	};

	class Block
	{
	public:
		~Block();
		bool Loop() const;
		void SetLoops(bool loops);

		bool OverridesTempo() const;
		void SetOverridesTempo(bool overridesTempo);
		TempoControlTrack &GetTempoControlTrack();

		NoteTrack *CreateNoteTrack(std::string name);
		void RemoveNoteTrack(NoteTrack *track);
		const std::vector<NoteTrack *> &NoteTracks() const;

		ControlTrack *CreateParameterTrack(std::string name);
		void RemoveParameterTrack(ControlTrack *track);
		const std::vector<ControlTrack *> &ParameterTracks() const;

		TrackTime GetLength();
		void SetLength(TrackTime time);

		const std::vector<BlockTransition *> &Transitions();
		void AddTransition(BlockTransition *transition);
		void RemoveTransition(BlockTransition *transition);
	private:
		TrackTime m_length;
		bool m_overridesTempo;
		bool m_loops;

		TempoControlTrack m_tempoControlTrack;
		std::vector<NoteTrack *> m_tracks;
		std::vector<ControlTrack *> m_parameterTracks;
		std::vector<BlockTransition *> m_transitions;
	};
}