
#include <iostream>
#include "NoteLane.h"
#include "Note.h"
#include "VecUtil.h"

namespace uComposer
{
	const std::vector<TrackNote> &NoteLane::Notes() const
	{
		return m_notes;
	}

	// Insert a new note into sorted place.
	void NoteLane::Insert(Note note, TrackTime time, TrackTime duration)
	{
		Insert(TrackNote(time, duration, note));
		return;
	}

	void NoteLane::Insert(TrackNote note)
	{
		// TODO check to see if identical note already exists. If so, just get rid of it.
		//		Will this cause problems with pointers? Probably...
		//		Wonder if I can return some kind of checked auto pointer.
		//		Like, a friend struct by val that has a pointer to the node
		if (m_notes.size() == 0)
		{
			m_notes.push_back(note);
			return;
		}

		std::vector<TrackNote>::iterator it = m_notes.begin();

		while (it != m_notes.end() && (*it).GetTime() < note.GetTime()) it++;
		m_notes.insert(it, note);
	}

	// Find a note at a given time
	bool NoteLane::QueryAtTime(TrackTime time, TrackNote &note)
	{
		for (size_t i = 0; i < m_notes.size(); i++)
		{
			TrackNote n = m_notes.at(i);
			if (n.GetTime() <= time && time < n.GetReleaseTime())
			{
				note = n;
				return true;
			}
		}
		return false;
	}

	// Remove a given note
	bool NoteLane::RemoveNote(TrackNote note)
	{
		return VecUtil::Remove(m_notes, note);
	}

	//DLL_EXTERN NoteLane *CreateNoteLane()
	//{
	//	return new NoteLane();
	//}

	//DLL_EXTERN uint64_t NoteLane_GetNoteCount(NoteLane *notelane)
	//{
	//	return notelane->Notes().size();
	//}

	//DLL_EXTERN void NoteLane_Insert(NoteLane *notelane, Note note)
	//{
	//	notelane->Insert(note);
	//}

	//DLL_EXTERN bool NoteLane_QueryAtTime(NoteLane *notelane, uint64_t time, Note &found)
	//{
	//	return notelane->QueryAtTime(TrackTime(time), found);
	//}

	//DLL_EXTERN bool NoteLane_RemoveNote(NoteLane *notelane, Note note)
	//{
	//	return notelane->RemoveNote(note);
	//}
}