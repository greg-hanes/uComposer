#include "BlockCollection.h"
#include "VecUtil.h"

namespace uComposer
{
	BlockCollection::BlockCollection()
	{
		m_defaultBlock = new Block();
		m_initialBlock = m_defaultBlock;
		m_blocks.push_back(m_defaultBlock);
	}

	const std::vector<Block *> &BlockCollection::Blocks()
	{
		return m_blocks;
	}

	Block *BlockCollection::CreateBlock()
	{
		Block *block = new Block();
		m_blocks.push_back(block);

		return block;
	}

	void BlockCollection::RemoveBlock(Block *block)
	{
		if (block == m_defaultBlock)
			return;

		if (block == m_initialBlock)
			m_initialBlock = m_defaultBlock;

		for (auto b : m_blocks)
		{
			std::vector<BlockTransition *> toRemove;

			for (auto t : b->Transitions())
			{
				if (t->Destination() == block)
				{
					toRemove.push_back(t);
				}
			}

			for (auto t : toRemove)
			{
				b->RemoveTransition(t);
			}	
		}

		VecUtil::Delete(m_blocks, block);
	}

	Block *BlockCollection::GetInitialBlock() const
	{
		return m_initialBlock;
	}

	Block *BlockCollection::GetDefaultBlock() const
	{
		return m_defaultBlock;
	}

	void BlockCollection::SetInitialBlock(Block *block)
	{
		m_initialBlock = block;
	}
}