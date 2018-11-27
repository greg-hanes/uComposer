#pragma once

#include <vector>
#include "Block.h"

namespace uComposer
{
	
	class BlockCollection
	{
	public:
		BlockCollection();
		const std::vector<Block *> &Blocks();
		Block *CreateBlock();
		void RemoveBlock(Block *block);

		Block *GetDefaultBlock() const;
		Block *GetInitialBlock() const;

		void SetInitialBlock(Block *block);

		// TODO
		// Event<Block *> BlockDeleted;

	private:
		Block *m_initialBlock;
		Block *m_defaultBlock;
		std::vector<Block *> m_blocks;
	};
}