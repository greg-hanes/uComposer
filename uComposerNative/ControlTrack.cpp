#include "ControlTrack.h"
#include "ControlTrackSource.h"

namespace uComposer
{
	ControlTrack::ControlTrack() : m_name("") {}

	ControlTrack::ControlTrack(std::string name) :
		m_name(name)
	{

	}

	const std::string ControlTrack::GetName() const
	{
		return m_name;
	}

	void ControlTrack::SetName(std::string name)
	{
		m_name = name;
	}

	ControlTrackSource *ControlTrack::GetTrackNode() const
	{
		return m_trackNode;
	}

	void ControlTrack::SetTrackNode(ControlTrackSource *trackNode)
	{
		m_trackNode = trackNode;
	}

	AutomationLane &ControlTrack::GetAutomationLane()
	{
		return m_automationLane;
	}

	void ControlTrack::Process(TrackTime startTime, TrackTime endTime, uint32_t sampleOffset)
	{
		for (size_t i = 0; i < m_automationLane.Keys().size(); i++)
		{
			auto key = m_automationLane.Keys().at(i);

			if (endTime < key.GetTime())
				break;

			if (i < m_automationLane.Keys().size() - 1)
			{
				auto nextKey = m_automationLane.Keys().at(i + 1);
			}

			//m_trackNode->SendMessage(ControlMessage(sampleOffset, key.GetValue()));
		}
	}
}