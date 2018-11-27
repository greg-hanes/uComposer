#include "TrackTime.h"

namespace uComposer
{
	const TrackTime TrackTime::Zero = TrackTime(0);
	const int64_t TrackTime::kTicksPerBeat = (1LL << 32);
	const int64_t TrackTime::kTicksPerQuarterBeat = kTicksPerBeat >> 2;
	const int64_t TrackTime::kTicksPerSixteenthBeat = kTicksPerQuarterBeat >> 2;

	///////////////////////////
	// Track Time
	///////////////////////////
	TrackTime TrackTime::FromSeconds(double seconds, double bpm)
	{
		double totalBeats = seconds * bpm / 60;
		return TrackTime(static_cast<int64_t>(totalBeats * kTicksPerBeat));
	}

	TrackTime::TrackTime() :
		m_ticks(0)
	{

	}

	TrackTime::TrackTime(int64_t ticks) :
		m_ticks(ticks)
	{
	}

	TrackTime::TrackTime(int64_t beats, int64_t quarterBeats) :
		m_ticks(beats * kTicksPerBeat + quarterBeats * kTicksPerQuarterBeat)
	{
	}

	TrackTime::TrackTime(int64_t beats, int64_t quarterBeats, int64_t sixteenthBeats) :
		m_ticks(beats * kTicksPerBeat + quarterBeats * kTicksPerQuarterBeat + sixteenthBeats * kTicksPerSixteenthBeat)
	{

	}

	int64_t TrackTime::Beats() const
	{
		return m_ticks >> 16;
	}

	int64_t TrackTime::Ticks() const
	{
		return m_ticks % kTicksPerBeat;
	}

	double TrackTime::TotalBeats() const
	{
		return (m_ticks / (double)kTicksPerBeat);
	}

	int64_t TrackTime::TotalTicks() const
	{
		return m_ticks;
	}

	double TrackTime::ToSeconds(double bpm) const
	{
		return 60 * TotalBeats() / bpm;
	}

	
	// Given two TrackTimes (beats) b0 and b1, at which the Tempo (BPM) at those times is v0 and v1,
	//		and a TrackTime b between b0 and b1, returns the amount of real time that will pass
	//		
	double TrackTime::GetTime(TrackTime b0, double v0, TrackTime b1, double v1, TrackTime b)
	{
		// Not necessarily true, but assuming tempo never equal to 0
		if (v0 == 0 || v1 == 0)
			return INFINITY;

		double d = abs(v1 - v0);
		if (d < kEpsilon)
		{
			// Tempo is constant
			return (b - b0).ToSeconds(v0);
		}

		double dbdv = (b1 - b0).ToSeconds(v1 - v0);
		double dvdb = 1.0 / dbdv;
		return dbdv * log(1 + dvdb * ((b - b0).ToSeconds(v0)));
	}

	// Given two TrackTimes b0 and b1, where the tempo at b0 and b1 in BPM is v0 and v1,
	//		and wall-clock seconds since b0, returns the effective TrackTime.
	TrackTime TrackTime::GetBeat(TrackTime b0, double v0, TrackTime b1, double v1, double t)
	{
		if (v0 == 0 || v1 == 0)
			return TrackTime::FromSeconds(INFINITY, 1);

		double d = abs(v1 - v0);

		if (d < kEpsilon)
			return b0 + TrackTime::FromSeconds(t, v0);

		double dbdv = (b1 - b0).ToSeconds(v1 - v0);
		double dvdb = 1.0 / dbdv;
		return b0 + TrackTime::FromSeconds(dbdv * (exp(t * dvdb) - 1), v0);
	}
}