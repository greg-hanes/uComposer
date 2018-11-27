#pragma once

#include "TrackTime.h"


namespace uComposer
{
	// Will need:
	// operator==
	// operator!=
	// operator=
	// TempoControlPoint(TempoControlPoint &)
	// TempoControlPoint(const TempoControlPoint &)
	struct TempoControlPoint
	{
	public:
		TempoControlPoint();
		TempoControlPoint(double tempo, TrackTime beat);

		static double GetTime(TempoControlPoint p0, TempoControlPoint p1, TrackTime b);
		static TrackTime GetBeat(TempoControlPoint p0, TempoControlPoint p1, double t);

		double GetTempo() const;
		TrackTime GetBeat() const;

		friend bool operator==(const TempoControlPoint &lhs, const TempoControlPoint &rhs)
		{
			return lhs.m_tempoBPM == rhs.m_tempoBPM && lhs.m_beat == rhs.m_beat;
		}

		friend bool operator!=(const TempoControlPoint &lhs, const TempoControlPoint &rhs)
		{
			return !(lhs == rhs);
		}

	private:
		double m_tempoBPM;
		TrackTime m_beat;
	};


}