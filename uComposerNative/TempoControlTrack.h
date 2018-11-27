#pragma once

#include <vector>
#include "TempoControlPoint.h"

namespace uComposer
{
	class TempoControlTrack
	{
	public:
		const std::vector<TempoControlPoint> &ControlPoints() const;
		void AddControlPoint(TempoControlPoint point);
		void RemoveControlPoint(TempoControlPoint point);
		void RemoveControlPoint(int index);

		double GetTempoAtTrackTime(TrackTime trackTime);

		TrackTime GetTrackTimeFromWallTime(double wallTime);
		
	private:
		std::vector<TempoControlPoint> m_tempoControlPoints;
	};
}