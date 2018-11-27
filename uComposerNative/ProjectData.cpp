#include "ProjectData.h"
#include "NoteTrackSource.h"

namespace uComposer
{
	ProjectData::ProjectData() :
		m_tempo(new Tempo()),
		m_effectGraph(new EffectGraph()),
		m_blocks(new BlockCollection())
	{
	}

	ProjectData::~ProjectData()
	{
		delete m_tempo;
		delete m_effectGraph;
		delete m_blocks;
	}

	Tempo *ProjectData::GetTempo() const
	{
		return m_tempo;
	}

	EffectGraph *ProjectData::GetEffectGraph() const
	{
		return m_effectGraph;
	}

	BlockCollection *ProjectData::GetBlocks()
	{
		return m_blocks;
	}
}