#pragma once

#include <cstdint>

#include "EventCommon.h"
#include "Note.h"

namespace uComposer
{
	struct EventMessage
	{
		static EventMessage CreateNoteOn(uint32_t offset, Note note);
		static EventMessage CreateNoteOff(uint32_t offset, Note note);
		static EventMessage CreateKillAllNotes(uint32_t offset);



		uint32_t GetOffset() const;


		EventType type;
		uint32_t mOffset;
		Note note;
	};
}