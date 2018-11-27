#pragma once

#include <vector>
#include "INodeVisitor.h"

namespace uComposer
{
	class FlattenGraphVisitor : public INodeVisitor
	{
	public:
		void GetFlattenedGraph(std::vector<BaseEffect *> &out) const;

		// Inherited via INodeVisitor
		virtual void PrepareTrip() override;

		// Inherited via INodeVisitor
		virtual void Visit(BaseEffect * node, ConnectionList connections) override;

	private:
		std::vector<BaseEffect *> m_flattenedGraph;
	};
}