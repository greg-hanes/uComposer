#pragma once

#include <vector>
#include <string>

#include "Tempo.h"
#include "AutomationLane.h"

namespace uComposer
{
	class ControlTrackSource;

	class ControlTrack
	{
	public:
		ControlTrack();
		ControlTrack(std::string name);

		const std::string GetName() const;
		void SetName(std::string name);

		ControlTrackSource *GetTrackNode() const;
		void SetTrackNode(ControlTrackSource *trackNode);

		AutomationLane &GetAutomationLane();

		void Process(TrackTime startTime, TrackTime endTime, uint32_t sampleOffset);
	private:
		std::string m_name;
		ControlTrackSource *m_trackNode;
		AutomationLane m_automationLane;
	};
}