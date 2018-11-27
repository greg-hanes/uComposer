#pragma once

#include "ConnectionList.h"

namespace uComposer
{
	class BaseEffect;

	class INodeVisitor
	{
	public:
		virtual void PrepareTrip() {}
		virtual void Visit(BaseEffect *node, ConnectionList connections) = 0;
		virtual void TripFinished() {}
	};
}