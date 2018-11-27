#pragma once

#include "Note.h"
#include "TrackTime.h"
#include "Tempo.h"

namespace uComposer
{
	// Represents a Note that will be played at a specific TrackTime for a specific duration.
	struct TrackNote
	{
	public:
		TrackNote();
		TrackNote(TrackTime time, TrackTime duration, Note note);
		TrackTime GetTime() const;
		TrackTime GetReleaseTime() const;
		TrackTime GetDuration() const;
		Note GetNote() const;

		friend bool operator==(const TrackNote &lhs, const TrackNote &rhs)
		{
			return lhs.m_time == rhs.m_time
				&& lhs.m_duration == rhs.m_duration
				&& lhs.m_note == rhs.m_note;
		}

		friend bool operator!=(const TrackNote &lhs, const TrackNote &rhs)
		{
			return !(lhs == rhs);
		}
	private:
		TrackTime m_time;
		TrackTime m_duration;
		Note m_note;
	};
}