#pragma once

#include <vector>
#include <cstdint>

#include "TrackTime.h"

namespace uComposer
{
	class AutomationKey
	{
	public:
		AutomationKey()
		{
			
		}

		AutomationKey(float value, TrackTime time) :
			m_value(value),
			m_time(time)
		{

		}

		TrackTime GetTime() const
		{
			return m_time;
		}

		float GetValue() const
		{
			return m_value;
		}
		
		friend bool operator==(const AutomationKey &lhs, const AutomationKey& rhs)
		{
			return lhs.m_time == rhs.m_time
				&& lhs.m_value == rhs.m_value;
		}

		friend bool operator!=(const AutomationKey &lhs, const AutomationKey &rhs)
		{
			return !(lhs == rhs);
		}
	private:
		TrackTime m_time;
		float m_value;
	};

	class AutomationLane
	{
	public:
		const std::vector<AutomationKey> &Keys() const;

		// Insert a new note into sorted place.
		void Insert(float value, TrackTime time);
		void Insert(AutomationKey key);

		// Remove a given key
		bool RemoveKey(AutomationKey key);

	private:
		std::vector<AutomationKey> m_keys;
	};
}