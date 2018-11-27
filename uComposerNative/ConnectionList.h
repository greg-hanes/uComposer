#pragma once
#include <vector>

#include "IEffect.h"

namespace uComposer
{
	class BaseEffect;
	class BaseOutput;
	class BaseInput;

	struct Connection
	{
	public:
		Connection();
		Connection(BaseOutput *source, BaseInput *dest);

		BaseOutput *Source() const;
		BaseInput *Dest() const;

		bool operator==(const Connection &other);
		bool operator!=(const Connection &other);
	private:
		BaseOutput *m_source;
		BaseInput *m_dest;
	};

	class ConnectionList
	{
	public:
		// Adds a new connection to the ConnectionList
		void AddConnection(Connection connection);
		// Removes the given connection.
		void RemoveConnection(Connection connection);

		// Gets the number of unique nodes that have a AudioInput which is the destination of the
		//	given AudioOutput.
		size_t GetOutputDestinationNodeCount(BaseOutput *output);

		// Gets the AudioOutput that is the source for the given AudioInput, or nullptr if 
		//	there is no source.
		BaseOutput *GetSource(BaseInput *input) const;
		AudioOutput *GetSource(AudioInput *input) const;
		EventOutput *GetSource(EventInput *input) const;
		ControlOutput *GetSource(ControlInput *input) const;


		// Gets the connection for which the given AudioInput is the destination. Returns true
		//	if such a connection is found, otherwise false. If a connection is found,
		//	it is assigned to the given ref variable 'out'
		bool GetConnection(BaseInput *input, Connection &out) const;

		// Populates the given vector 'out' with the set of unique AudioOutputs that are
		//	sources for the given BaseEffect's AudioInputs.
		void GetSources(BaseEffect *n, std::vector<AudioOutput *> &out) const;
		void GetSources(BaseEffect *n, std::vector<BaseOutput *> &out) const;

		// Populates the given vector 'out' with the set of unique nodes that have AudioOutputs
		//	that are a source for any of the given node's inputs.
		void GetSourcesNodes(BaseEffect *n, std::vector<BaseEffect *> out) const;

		// Populates the given vector 'out' with all connections where the AudioInput of the connection
		//  belongs to the given node. I.e., returns edges that go to the given node.
		void GetIncomingConnections(const BaseEffect *node, std::vector<Connection> &out) const;

		// Populates the given vector 'out' with all connections where the AudioOutput of the connection
		//	belongs to the given node. I.e., returns edges that come from the given node.
		void GetOutgoingConnections(const BaseEffect *node, std::vector<Connection> &out) const;

		// Populates the given vector 'out' with all connections where the source of the connection
		//	is the given AudioOutput.
		void GetConnections(BaseOutput *output, std::vector<Connection> &out) const;

		// Returns a const vector by reference of all connections in the connection list.
		const std::vector<Connection> &Connections() const;
	private:
		std::vector<Connection> m_connections;
	};
}