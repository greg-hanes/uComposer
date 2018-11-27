#include "ControlMessage.h"


namespace uComposer
{
	ControlMessage::ControlMessage() :
		mOffset(0),
		m_value(0)
	{

	}

	ControlMessage::ControlMessage(uint32_t offset, float value) :
		mOffset(offset),
		m_value(value)
	{

	}

	uint32_t ControlMessage::GetOffset() const
	{
		return mOffset;
	}
}