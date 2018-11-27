#include "ConnectionList.h"
#include "BaseEffect.h"
#include "VecUtil.h"


namespace uComposer
{
	////////////////////////////////////////////////////
	// Connection Definition
	////////////////////////////////////////////////////
	Connection::Connection() : m_source(nullptr), m_dest(nullptr) {}

	Connection::Connection(BaseOutput *source, BaseInput *dest) :
		m_source(source),
		m_dest(dest)
	{

	}

	BaseOutput *Connection::Source() const
	{
		return m_source;
	}

	BaseInput *Connection::Dest() const
	{
		return m_dest;
	}

	bool Connection::operator==(const Connection &other)
	{
		return m_source == other.m_source && m_dest == other.m_dest;
	}

	bool Connection::operator!=(const Connection &other)
	{
		return !(*this == other);
	}


	////////////////////////////////////////////////////
	// ConnectionList Definition
	////////////////////////////////////////////////////

	// Adds a new connection to the ConnectionList
	void ConnectionList::AddConnection(Connection connection)
	{
		m_connections.push_back(connection);
	}

	// Removes the given connection.
	void ConnectionList::RemoveConnection(Connection connection)
	{
		VecUtil::Remove(m_connections, connection);
	}

	// Gets the number of unique nodes that have a AudioInput which is the destination of the
	//	given AudioOutput.
	size_t ConnectionList::GetOutputDestinationNodeCount(BaseOutput *output)
	{
		std::vector<Connection> connections;
		GetConnections(output, connections);
		std::vector<BaseEffect *> nodeDestinations = std::vector<BaseEffect *>();
		for (auto c : connections)
		{
			BaseEffect *owner = c.Dest()->Owner();
			if (std::find(nodeDestinations.begin(), nodeDestinations.end(), owner) == nodeDestinations.end())
			{
				nodeDestinations.push_back(owner);
			}
		}

		return nodeDestinations.size();
	}

	BaseOutput *ConnectionList::GetSource(BaseInput *input) const
	{
		for (auto c : m_connections)
		{
			if (c.Dest() == input)
				return c.Source();
		}
		return nullptr;
	}

	// Gets the AudioOutput that is the source for the given AudioInput, or nullptr if 
	//	there is no source.
	AudioOutput *ConnectionList::GetSource(AudioInput *input) const
	{
		for (auto c : m_connections)
		{
			if (c.Dest() == input)
				return static_cast<AudioOutput*>(c.Source());
		}
		return nullptr;
	}

	EventOutput *ConnectionList::GetSource(EventInput *input) const
	{
		for (auto c : m_connections)
		{
			if (c.Dest() == input)
				return static_cast<EventOutput*>(c.Source());
		}
		return nullptr;
	}

	ControlOutput *ConnectionList::GetSource(ControlInput *input) const
	{
		for (auto c : m_connections)
		{
			if (c.Dest() == input)
				return static_cast<ControlOutput*>(c.Source());
		}
		return nullptr;
	}

	bool ConnectionList::GetConnection(BaseInput *input, Connection &out) const
	{
		for (auto c : m_connections)
		{
			if (c.Dest() == input)
			{
				out = c;
				return true;
			}
		}
		return false;
	}

	void ConnectionList::GetSources(BaseEffect *n, std::vector<BaseOutput *> &out) const
	{
		std::vector<BaseOutput *> outputs = std::vector<BaseOutput*>();
		for (auto input : n->AudioInputs())
		{
			BaseOutput *source = GetSource(input);
			if (source != nullptr && std::find(outputs.begin(), outputs.end(), source) == outputs.end())
			{
				outputs.push_back(source);
				out.push_back(source);
			}
		}
		for (auto input : n->ControlInputs())
		{
			BaseOutput *source = GetSource(input);
			if (source != nullptr && std::find(outputs.begin(), outputs.end(), source) == outputs.end())
			{
				outputs.push_back(source);
				out.push_back(source);
			}
		}
		for (auto input : n->EventInputs())
		{
			BaseOutput *source = GetSource(input);
			if (source != nullptr && std::find(outputs.begin(), outputs.end(), source) == outputs.end())
			{
				outputs.push_back(source);
				out.push_back(source);
			}
		}
	}

	// Populates the given vector 'out' with the set of unique AudioOutputs that are
	//	sources for the given BaseEffect's AudioInputs.
	void ConnectionList::GetSources(BaseEffect *n, std::vector<AudioOutput *> &out) const
	{
		std::vector<AudioOutput *> outputs = std::vector<AudioOutput*>();
		for (auto input : n->AudioInputs())
		{
			AudioOutput *source = GetSource(input);
			if (source != nullptr && std::find(outputs.begin(), outputs.end(), source) == outputs.end())
			{
				outputs.push_back(source);
				out.push_back(source);
			}
		}
	}

	// Populates the given vector 'out' with the set of unique nodes that have AudioOutputs
	//	that are a source for any of the given node's inputs.
	void ConnectionList::GetSourcesNodes(BaseEffect *n, std::vector<BaseEffect *> out) const
	{
		// Connections.Where((c) => c.Destination.Owner == n).Select((c) => c.Source.Owner).Distinct().ToList();
		std::vector<BaseEffect *> nodes = std::vector<BaseEffect *>();

		for (auto c : m_connections)
		{
			if (c.Dest()->Owner() == n && (std::find(nodes.begin(), nodes.end(), c.Source()->Owner()) == nodes.end()))
			{
				nodes.push_back(c.Source()->Owner());
				out.push_back(c.Source()->Owner());
			}
		}
	}

	void ConnectionList::GetIncomingConnections(const BaseEffect *node, std::vector<Connection> &out) const
	{
		for (auto c : m_connections)
		{
			if (c.Dest()->Owner() == node)
				out.push_back(c);
		}
	}

	// Populates the given vector 'out' with all connections where the AudioOutput of the connection
	//	belongs to the given node. I.e., returns edges that come from the given node.
	void ConnectionList::GetOutgoingConnections(const BaseEffect *node, std::vector<Connection> &out) const
	{
		for (auto c : m_connections)
		{
			if (c.Source()->Owner() == node)
				out.push_back(c);
		}
	}

	// Populates the given vector 'out' with all connections where the source of the connection
	//	is the given AudioOutput.
	void ConnectionList::GetConnections(BaseOutput *output, std::vector<Connection> &out) const
	{
		for (auto c : m_connections)
		{
			if (c.Source() == output)
			{
				out.push_back(c);
			}
		}
	}

	// Returns a const vector by reference of all connections in the connection list.
	const std::vector<Connection> &ConnectionList::Connections() const
	{
		return m_connections;
	}
}