#include "TrackNote.h"

namespace uComposer
{
	TrackNote::TrackNote()
	{
	}

	TrackNote::TrackNote(TrackTime time, TrackTime duration, Note note) :
		m_time(time),
		m_duration(duration),
		m_note(note)
	{
	}

	TrackTime TrackNote::GetTime() const
	{
		return m_time;
	}

	TrackTime TrackNote::GetReleaseTime() const
	{
		return m_time + m_duration;
	}

	TrackTime TrackNote::GetDuration() const
	{
		return m_duration;
	}

	Note TrackNote::GetNote() const
	{
		return m_note;
	}
}