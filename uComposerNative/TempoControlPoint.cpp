#include "TempoControlPoint.h"

namespace uComposer
{
	/////////////////////////
	// TempoControlPoint
	/////////////////////////
	TempoControlPoint::TempoControlPoint() :
		m_tempoBPM(1),
		m_beat(0)
	{

	}

	TempoControlPoint::TempoControlPoint(double tempo, TrackTime beat) :
		m_beat(beat),
		m_tempoBPM(tempo)
	{

	}

	double TempoControlPoint::GetTempo() const
	{
		return m_tempoBPM;
	}

	TrackTime TempoControlPoint::GetBeat() const
	{
		return m_beat;
	}

	TrackTime TempoControlPoint::GetBeat(TempoControlPoint p0, TempoControlPoint p1, double t)
	{
		return TrackTime::GetBeat(p0.GetBeat(), p0.GetTempo(), p1.GetBeat(), p1.GetTempo(), t);
	}

	double TempoControlPoint::GetTime(TempoControlPoint p0, TempoControlPoint p1, TrackTime b)
	{
		return TrackTime::GetTime(p0.GetBeat(), p0.GetTempo(), p1.GetBeat(), p1.GetTempo(), b);
	}
}