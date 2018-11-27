#pragma once

#include <vector>
#include <string>

#include "Tempo.h"
#include "NoteLane.h"

namespace uComposer
{
	class NoteTrackSource;

	class NoteTrack
	{
	public:
		NoteTrack();
		NoteTrack(std::string name);
		~NoteTrack();
		const std::string GetName() const;
		void SetName(std::string name);


		NoteTrackSource *GetTrackNode() const;
		void SetTrackNode(NoteTrackSource *trackNode);

		const std::vector<NoteLane *> &NoteLanes();

		NoteLane *CreateNoteLane();
		void DeleteNoteLane(NoteLane *);

		void Process(TrackTime startTime, TrackTime endTime, uint32_t sampleOffset);

	private:
		std::string m_name;
		NoteTrackSource *m_trackNode;
		std::vector<NoteLane *> m_notelanes;
	};
/*
	DLL_EXTERN void Track_CreateNoteLane(Track *);
	DLL_EXTERN int Track_GetNoteLaneCount(Track *);
	DLL_EXTERN NoteLane **Track_GetNoteLanes(Track *);
	DLL_EXTERN NoteLane *Track_GetNoteLane(Track *, int index);
	DLL_EXTERN void Track_EraseNoteLane(Track *, NoteLane *);
*/
}