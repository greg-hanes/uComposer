#pragma once

#include <cstdint>

namespace uComposer
{
	// Represents a control message to change the value of a parameter.
	// TODO the system that utilizes this isn't complete.s
	struct ControlMessage
	{
		// Default constructor initializes offset and value to 0.
		ControlMessage();

		// Constructs a ControlMessage at the given sample offset with the given value.
		ControlMessage(uint32_t offset, float value);
		uint32_t GetOffset() const;

		uint32_t mOffset;
		float m_value;

	};
}