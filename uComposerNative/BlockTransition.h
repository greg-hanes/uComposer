#pragma once

namespace uComposer
{
	class Block;

	class BlockTransition
	{
	public:
		BlockTransition(Block *dest);
		Block *Destination() const;
		bool CanTransition();
		int GetPlayCount() const;

		void Set(bool active);
		void SetPlayCount(int playCount);

	private:
		Block *m_dest;

		int m_playCount;
		bool m_active;
	};
}