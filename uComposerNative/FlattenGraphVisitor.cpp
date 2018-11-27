#include "FlattenGraphVisitor.h"

namespace uComposer
{
	void FlattenGraphVisitor::GetFlattenedGraph(std::vector<BaseEffect *> &out) const
	{
		for (auto n : m_flattenedGraph)
			out.push_back(n);
	}

	void FlattenGraphVisitor::PrepareTrip()
	{
		m_flattenedGraph.clear();
	}

	void FlattenGraphVisitor::Visit(BaseEffect * node, ConnectionList connections)
	{
		m_flattenedGraph.push_back(node);
	}
}