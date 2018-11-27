#pragma once


#include <vector>
#include "BaseEffect.h"
#include "Event.h"

#include "INodeVisitor.h"
#include "ConnectionList.h"

namespace uComposer
{
	class NoteTrackSource;
	class ControlTrackSource;
	class OutputNode;

	typedef std::vector<NoteTrackSource *> TrackNodeCollection;
	typedef std::vector<ControlTrackSource *> ParameterTrackNodeCollection;

	enum CreateConnectionResult
	{
		Success = 0,
		CreatesCycle = -1,
		DestinationAlreadyHasSource = -2,
		WrongGraph = -3
	};

	// Effect graph is a wrapper for Graph that exposes specific types of nodes.
	//		TrackNodes, OutputNode
	class EffectGraph
	{
	public:
		EffectGraph();
		~EffectGraph();

		OutputNode *GetOutputNode() const;
		const std::vector<NoteTrackSource *> &TrackNodes();
		const std::vector<ControlTrackSource *> &ParameterTrackNodes();
		
		void AddNode(BaseEffect *node);
		void RemoveNode(BaseEffect *node);

		const std::vector<Connection> &GetConnections();
		ConnectionList &GetConnectionList();
		
		CreateConnectionResult CreateConnection(AudioOutput *source, AudioInput *destination);
		CreateConnectionResult CreateConnection(EventOutput *source, EventInput *destination);
		CreateConnectionResult CreateConnection(ControlOutput *source, ControlInput *destination);
		void RemoveConnection(Connection connection);
		void GetFlattenedGraph(std::vector<BaseEffect *> &out);

		Event<BaseEffect *> &NodeAddedEvent();
		const std::vector<BaseEffect *> &GetNodes();

		void TopoTraverse(INodeVisitor *visitor);

	private:
		CreateConnectionResult CreateConnection(BaseOutput *source, BaseInput *destination);
		void GetRootNodes(std::vector<BaseEffect *> &out) const;

		OutputNode *m_outputNode;
		TrackNodeCollection m_trackNodes;
		ParameterTrackNodeCollection m_parameterTrackNodes;

		//////////////////////////////
		// From Graph:
		ConnectionList m_connectionList;
		std::vector<BaseEffect *> m_nodes;
		Event<BaseEffect *> m_nodeAddedEvent;

		bool CheckForCycles(ConnectionList connectionList);
		ConnectionList TopoTraverse(ConnectionList connections, INodeVisitor *visitor);

		void NodeInputAddedHandler(BaseEffect *node, BaseInput *input);
		void NodeInputRemovedHandler(BaseEffect *node, BaseInput *input);
		void NodeOutputAddedHandler(BaseEffect *node, BaseOutput *output);
		void NodeOutputRemovedHandler(BaseEffect *node, BaseOutput *output);
		//////////////////////////////
	};
}