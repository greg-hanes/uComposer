#include "EventMessage.h"

namespace uComposer
{
	// Static
	EventMessage EventMessage::CreateNoteOn(uint32_t offset, Note note)
	{
		EventMessage msg;
		msg.type = EventType::kNoteOnEvent;
		msg.mOffset = offset;
		msg.note = note;

		return msg;
	}

	// Static
	EventMessage EventMessage::CreateNoteOff(uint32_t offset, Note note)
	{
		EventMessage msg;
		msg.type = EventType::kNoteOffEvent;
		msg.mOffset = offset;
		msg.note = note;

		return msg;
	}

	EventMessage EventMessage::CreateKillAllNotes(uint32_t offset)
	{
		EventMessage msg;
		msg.type = EventType::kAllNotesOffEvent;
		msg.mOffset = offset;
		msg.note = Note();

		return msg;
	}

	uint32_t EventMessage::GetOffset() const
	{
		return mOffset;
	}
}