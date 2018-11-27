#include "TempoControlTrack.h"
#include "VecUtil.h"
#include <iostream>

// Create TempoControl class
//		-vector of TempoControlPoint
//		-Calculate real time length of sample buffer
//		-Given current TrackTime, find surrounding tempo automation control points
//		-Given control points, find current real time, then find current beat from current real time + buffer time
//		-Process tracks: given current track time and end of buffer track time, enqueue events/control changes
//		-Send messages at sample offset:
//			foreach message, convert track time to sample via tempo (GetTime: takes 2 points and beat, returns real time)
//				convert real time to sample time via SR
//			Issue messages

namespace uComposer
{
	const std::vector<TempoControlPoint> &TempoControlTrack::ControlPoints() const
	{
		return m_tempoControlPoints;
	}

	void TempoControlTrack::AddControlPoint(TempoControlPoint point)
	{
		if (m_tempoControlPoints.size() == 0)
		{
			m_tempoControlPoints.push_back(point);
			return;
		}

		int insertIndex = 0;
		for (;insertIndex < m_tempoControlPoints.size(); insertIndex++)
		{
			if (m_tempoControlPoints.at(insertIndex).GetBeat() == point.GetBeat())
			{
				VecUtil::RemoveAt(m_tempoControlPoints, insertIndex);
				VecUtil::Insert(m_tempoControlPoints, point, insertIndex);
				return;
			}
			if (m_tempoControlPoints.at(insertIndex).GetBeat() > point.GetBeat())
			{
				VecUtil::Insert(m_tempoControlPoints, point, insertIndex);
				return;
			}
		}

		m_tempoControlPoints.push_back(point);
	}

	void TempoControlTrack::RemoveControlPoint(TempoControlPoint point)
	{
		VecUtil::Remove(m_tempoControlPoints, point);
	}

	void TempoControlTrack::RemoveControlPoint(int index)
	{
		VecUtil::RemoveAt(m_tempoControlPoints, index);
	}

	double TempoControlTrack::GetTempoAtTrackTime(TrackTime trackTime)
	{
		assert(m_tempoControlPoints.size() != 0);

		for (int i = 0; i < m_tempoControlPoints.size() - 1; i++)
		{
			TempoControlPoint p0 = m_tempoControlPoints[i];
			TempoControlPoint p1 = m_tempoControlPoints[i + 1];

			if (p0.GetBeat() <= trackTime && trackTime < p1.GetBeat())
			{
				double t = (trackTime.TotalTicks() - p0.GetBeat().TotalTicks()) / (double)(p1.GetBeat().TotalTicks() - p0.GetBeat().TotalTicks());
				return p0.GetTempo() + t * (p1.GetTempo() - p0.GetTempo());
			}
		}
		return m_tempoControlPoints[m_tempoControlPoints.size() - 1].GetTempo();
	}

	TrackTime TempoControlTrack::GetTrackTimeFromWallTime(double wallTime)
	{
		assert(m_tempoControlPoints.size() != 0);

		if (m_tempoControlPoints.size() == 1)
		{
			return TrackTime::FromSeconds(wallTime, m_tempoControlPoints[0].GetTempo());
		}

		double currentTime = 0;
		for (int i = 0; i < m_tempoControlPoints.size() - 1; i++)
		{
			TempoControlPoint p0 = m_tempoControlPoints[i];
			TempoControlPoint p1 = m_tempoControlPoints[i + 1];

			double segmentDuration = TempoControlPoint::GetTime(p0, p1, p1.GetBeat());
			
			if (wallTime >= currentTime && currentTime > (wallTime + segmentDuration))
			{
				// These are the points we want!
				double tWallTime = wallTime - currentTime;
				return TempoControlPoint::GetBeat(p0, p1, tWallTime);
			}

			currentTime += segmentDuration;
		}

		// currentTime is the amount of wall-time required to reach the last control point.
		TempoControlPoint lastControlPoint = m_tempoControlPoints[m_tempoControlPoints.size() - 1];
		return lastControlPoint.GetBeat() + TrackTime::FromSeconds(wallTime - currentTime, lastControlPoint.GetTempo());
	}
}