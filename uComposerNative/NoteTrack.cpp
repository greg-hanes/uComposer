#include <algorithm>
#include "NoteTrack.h"
#include "VecUtil.h"
#include "NoteTrackSource.h"
#include <iostream>

namespace uComposer
{
	NoteTrack::NoteTrack() : m_name("") {}
	NoteTrack::NoteTrack(std::string name) : m_name(name) {}

	NoteTrack::~NoteTrack()
	{
		VecUtil::Clear(m_notelanes);
	}

	const std::string NoteTrack::GetName() const
	{
		return m_name;
	}

	void NoteTrack::SetName(std::string name)
	{
		m_name = name;
	}

	NoteTrackSource *NoteTrack::GetTrackNode() const
	{
		return m_trackNode;
	}

	void NoteTrack::SetTrackNode(NoteTrackSource *trackNode)
	{
		m_trackNode = trackNode;
	}

	const std::vector<NoteLane *> &NoteTrack::NoteLanes()
	{
		return m_notelanes;
	}

	NoteLane *NoteTrack::CreateNoteLane()
	{
		NoteLane *notelane = new NoteLane();
		m_notelanes.push_back(notelane);
		return notelane;
	}

	void NoteTrack::DeleteNoteLane(NoteLane *notelane)
	{
		VecUtil::Delete(m_notelanes, notelane);
	}

	// Ok, so reason has at least approximately per sample tempo automation...
	// Alright so the way this is going to work:
	//		For one, rewrite this process method.
	//		Instead of having this thing send the messages for one sample, give it
	//			a TrackTime timespan and just have it return a list of messages that
	//			will be sent.
	//			Now this doesn't need to worry about samples.
	//		It does need to be converted to samples, though, so how to do that?
	//			Same way it's already being done.
	//		So flow is:
	//			Request n samples.
	//			Calculate real time t = n/SR
	//			Calculate track time s(t) as integral of tempo function w.r.t. t
	//			Get TrackTime messages from NoteTracks and ControlTracks based on track time
	//			

	// Create TempoControl class
	//		-vector of TempoControlPoint
	//		-Calculate real time length of sample buffer
	//		-Given current TrackTime, find surrounding tempo automation control points
	//		-Given control points, find current real time, then find current beat from current real time + buffer time
	//		-Process tracks: given current track time and end of buffer track time, enqueue events/control changes
	//		-Send messages at sample offset:
	//			foreach message, convert track time to sample via tempo (GetTime: takes 2 points and beat, returns real time)
	//				convert real time to sample time via SR
	//			Issue messages

	void NoteTrack::Process(TrackTime startTime, TrackTime endTime, uint32_t sampleOffset)
	{
		for (auto noteLane : m_notelanes)
		{
			for (auto note : noteLane->Notes())
			{
				// Notes are sorted by start time, so if this note start time
				//  is past the current interval, we're done on this notelane.
				if (endTime < note.GetTime())
					break;

				// If the note starts on the current interval, send a NoteOnEvent
				if (note.GetTime() >= startTime)
				{
					// Send Note On message.
					EventMessage noteOn = EventMessage::CreateNoteOn(sampleOffset, note.GetNote());
					m_trackNode->SendMessage(noteOn);
					
				}

				// If the note releases on the current interval, send a NoteOffEvent
				if (note.GetReleaseTime() >= startTime && note.GetReleaseTime() <= endTime)
				{
					// Send Note Off message.
					EventMessage noteOff = EventMessage::CreateNoteOff(sampleOffset, note.GetNote());
					m_trackNode->SendMessage(noteOff);
				}
			}
		}
	}
}