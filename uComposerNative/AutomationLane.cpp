#include <assert.h>

#include "AutomationLane.h"
#include "VecUtil.h"


namespace uComposer
{
	const std::vector<AutomationKey> &AutomationLane::Keys() const
	{
		return m_keys;
	}

	void AutomationLane::Insert(float value, TrackTime time)
	{
		Insert(AutomationKey(value, time));
		return;
	}

	void AutomationLane::Insert(AutomationKey key)
	{
		if (m_keys.size() == 0)
		{
			m_keys.push_back(key);
			return;
		}

		std::vector<AutomationKey>::iterator it = m_keys.begin();
		while (it != m_keys.end() && it->GetTime() < key.GetTime()) it++;

		m_keys.insert(it, key);
	}


	bool AutomationLane::RemoveKey(AutomationKey key)
	{
		return VecUtil::Remove(m_keys, key);
	}
}