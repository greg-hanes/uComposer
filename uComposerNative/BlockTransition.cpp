#include "BlockTransition.h"

namespace uComposer
{
	// Add "play count" to this.
	BlockTransition::BlockTransition(Block *dest) :
		m_dest(dest),
		m_playCount(1)
	{

	}

	Block *BlockTransition::Destination() const { return m_dest; }

	bool BlockTransition::CanTransition()
	{
		// TODO
		// this
		return m_active;
	}

	int BlockTransition::GetPlayCount() const
	{
		return m_playCount;
	}

	void BlockTransition::Set(bool active)
	{
		m_active = active;
	}

	void BlockTransition::SetPlayCount(int playCount)
	{
		if (playCount < 1) playCount = 1;
		m_playCount = playCount;
	}
}