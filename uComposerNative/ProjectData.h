#pragma once

#include <vector>
#include "EffectGraph.h"
#include "Tempo.h"
#include "BlockCollection.h"

namespace uComposer
{
	class ProjectData
	{
	public:
		ProjectData();
		~ProjectData();

		Tempo *GetTempo() const;
		EffectGraph *GetEffectGraph() const;
		BlockCollection *GetBlocks();

	private:
		Tempo *m_tempo;
		EffectGraph *m_effectGraph;
		BlockCollection *m_blocks;
	};
}