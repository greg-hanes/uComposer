#pragma once

#include <vector>
#include <algorithm>
#include <cstdint>

#include "uComposer.h"
#include "TrackNote.h"

namespace uComposer
{
	class NoteLane
	{
	public:
		const std::vector<TrackNote> &Notes() const;

		// Insert a new note into sorted place.
		void Insert(Note note, TrackTime time, TrackTime duration);
		void Insert(TrackNote note);

		// Find a note at a given time
		bool QueryAtTime(TrackTime time, TrackNote &found);

		

		// Remove a given note
		bool RemoveNote(TrackNote note);

	private:
		std::vector<TrackNote> m_notes;
	};

	//DLL_EXTERN NoteLane *CreateNoteLane();
	//DLL_EXTERN uint64_t NoteLane_GetNoteCount(NoteLane *notelane);
	//DLL_EXTERN void NoteLane_Insert(NoteLane *notelane, Note note);
	//DLL_EXTERN bool NoteLane_QueryAtTime(NoteLane *notelane, uint64_t time, Note &found);
	//DLL_EXTERN bool NoteLane_RemoveNote(NoteLane *notelane, Note note);
}