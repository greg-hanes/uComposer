#include <queue>
#include "VecUtil.h"
#include "EffectGraph.h"
#include "OutputEffect.h"
#include "NoteTrackSource.h"
#include "ControlTrackSource.h"
#include "FlattenGraphVisitor.h"
#include "StandardEffects.h"

namespace uComposer
{
	EffectGraph::EffectGraph() : m_outputNode(StandardEffects::AudioOutput.CreateInstance<OutputNode>())
	{
		AddNode(m_outputNode);
	}

	EffectGraph::~EffectGraph()
	{
		// Ahhh shit this is no good.
		VecUtil::Clear(m_nodes);
	}

	// TODO change this so it takes an EffectDescriptor.
	void EffectGraph::AddNode(BaseEffect *node)
	{
		node->m_inputAddedEvent.Register(this, &EffectGraph::NodeInputAddedHandler);
		node->m_inputRemovedEvent.Register(this, &EffectGraph::NodeInputRemovedHandler);
		node->m_outputAddedEvent.Register(this, &EffectGraph::NodeOutputAddedHandler);
		node->m_outputRemovedEvent.Register(this, &EffectGraph::NodeOutputRemovedHandler);

		m_nodes.push_back(node);
		m_nodeAddedEvent(node);

		node->Initialize();

		if (*node->Type() == StandardEffects::Track)
		{
			m_trackNodes.push_back(static_cast<NoteTrackSource *>(node));
		}
		if (*node->Type() == StandardEffects::ControlTrack)
		{
			m_parameterTrackNodes.push_back(static_cast<ControlTrackSource *>(node));
		}
	}

	void EffectGraph::RemoveNode(BaseEffect *node)
	{
		node->m_inputAddedEvent.Unregister(this, &EffectGraph::NodeInputAddedHandler);
		node->m_inputRemovedEvent.Unregister(this, &EffectGraph::NodeInputRemovedHandler);
		node->m_outputAddedEvent.Unregister(this, &EffectGraph::NodeOutputAddedHandler);
		node->m_outputRemovedEvent.Unregister(this, &EffectGraph::NodeOutputRemovedHandler);

		std::vector<Connection> connections;
		m_connectionList.GetOutgoingConnections(node, connections);
		for (auto c : connections)
			m_connectionList.RemoveConnection(c);

		connections.clear();

		m_connectionList.GetIncomingConnections(node, connections);
		for (auto c : connections)
			m_connectionList.RemoveConnection(c);

		VecUtil::Remove(m_nodes, node);
	}

	void EffectGraph::RemoveConnection(Connection connection)
	{
		m_connectionList.RemoveConnection(connection);
	}

	CreateConnectionResult EffectGraph::CreateConnection(BaseOutput *source, BaseInput *destination)
	{
		if (m_connectionList.GetSource(destination) != nullptr)
			return DestinationAlreadyHasSource;

		Connection connection = Connection(source, destination);

		ConnectionList preliminaryConnectionList = m_connectionList;
		preliminaryConnectionList.AddConnection(connection);

		if (CheckForCycles(preliminaryConnectionList))
		{
			return CreateConnectionResult::CreatesCycle;
		}

		m_connectionList.AddConnection(connection);
		return Success;
	}

	CreateConnectionResult EffectGraph::CreateConnection(AudioOutput *source, AudioInput *destination)
	{
		CreateConnectionResult result = CreateConnection(static_cast<BaseOutput*>(source), static_cast<BaseInput *>(destination));
		return result;
	}

	CreateConnectionResult EffectGraph::CreateConnection(EventOutput *source, EventInput *destination)
	{
		CreateConnectionResult result = CreateConnection(static_cast<BaseOutput*>(source), static_cast<BaseInput *>(destination));
		return result;
	}

	CreateConnectionResult EffectGraph::CreateConnection(ControlOutput *source, ControlInput *destination)
	{
		CreateConnectionResult result = CreateConnection(static_cast<BaseOutput*>(source), static_cast<BaseInput *>(destination));
		return result;
	}

	void EffectGraph::GetRootNodes(std::vector<BaseEffect *> &out) const
	{
		for (auto n : m_nodes)
		{
			for (auto c : m_connectionList.Connections())
			{
				if (c.Dest()->Owner() == n)
					goto Skip;
			}
			out.push_back(n);
		Skip:
			continue;
		}
	}

	void EffectGraph::GetFlattenedGraph(std::vector<BaseEffect *> &out)
	{
		FlattenGraphVisitor flatten;
		TopoTraverse(&flatten);
		flatten.GetFlattenedGraph(out);
	}

	const std::vector<Connection> &EffectGraph::GetConnections()
	{
		return m_connectionList.Connections();
	}

	ConnectionList &EffectGraph::GetConnectionList()
	{
		return m_connectionList;
	}

	Event<BaseEffect *> &EffectGraph::NodeAddedEvent()
	{
		return m_nodeAddedEvent;
	}

	const std::vector<BaseEffect *> &EffectGraph::GetNodes()
	{
		return m_nodes;
	}

	OutputNode *EffectGraph::GetOutputNode() const
	{
		return m_outputNode;
	}

	const std::vector<NoteTrackSource *> &EffectGraph::TrackNodes()
	{
		return m_trackNodes;
	}

	const std::vector<ControlTrackSource *> &EffectGraph::ParameterTrackNodes()
	{
		return m_parameterTrackNodes;
	}

	void EffectGraph::TopoTraverse(INodeVisitor *visitor)
	{
		TopoTraverse(m_connectionList, visitor);
	}

	ConnectionList EffectGraph::TopoTraverse(ConnectionList connections, INodeVisitor *visitor)
	{
		// Intentional copy!
		ConnectionList connectionsCopy = ConnectionList(connections);
		std::vector<BaseEffect *> rootNodes;
		GetRootNodes(rootNodes);

		std::queue<BaseEffect *> s = std::queue<BaseEffect *>();
		std::vector<BaseEffect *> l = std::vector<BaseEffect *>();

		for (auto node : rootNodes)
		{
			s.push(node);
		}

		if (visitor != nullptr)
		{
			visitor->PrepareTrip();
		}

		while (s.size() != 0)
		{
			BaseEffect *n = s.front();
			s.pop();

			if (visitor != nullptr)
				visitor->Visit(n, m_connectionList);

			// Get connections where n->Source()->Owner() == n
			//		i.e., outgoing connections from n
			std::vector<Connection> out;
			connectionsCopy.GetOutgoingConnections(n, out);
			for (auto c : out)
			{
				connectionsCopy.RemoveConnection(c);
				BaseEffect *m = c.Dest()->Owner();
				std::vector<BaseOutput *> out;
				connectionsCopy.GetSources(m, out);
				if (out.size() == 0)
					s.push(m);
			}
		}

		if (visitor != nullptr)
		{
			visitor->TripFinished();
		}

		return connectionsCopy;
	}


	bool EffectGraph::CheckForCycles(ConnectionList connectionList)
	{
		return TopoTraverse(connectionList, nullptr).Connections().size() != 0;
	}

	void EffectGraph::NodeInputAddedHandler(BaseEffect *node, BaseInput *input)
	{
		// ?
	}

	void EffectGraph::NodeInputRemovedHandler(BaseEffect *node, BaseInput *input)
	{
		Connection c;
		if (m_connectionList.GetConnection(input, c))
		{
			m_connectionList.RemoveConnection(c);
		}
	}

	void EffectGraph::NodeOutputAddedHandler(BaseEffect *node, BaseOutput *output)
	{

	}

	void EffectGraph::NodeOutputRemovedHandler(BaseEffect *node, BaseOutput *output)
	{
		std::vector<Connection> connections;
		m_connectionList.GetConnections(output, connections);

		for (auto c : connections)
		{
			m_connectionList.RemoveConnection(c);
		}
	}
}