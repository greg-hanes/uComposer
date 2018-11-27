#include "Block.h"
#include "NoteTrack.h"
#include "VecUtil.h"

namespace uComposer
{
	Block::~Block()
	{
		VecUtil::Clear(m_tracks);
		VecUtil::Clear(m_transitions);
	}

	bool Block::Loop() const
	{
		return m_loops;
	}

	void Block::SetLoops(bool loops)
	{
		m_loops = loops;
	}

	bool Block::OverridesTempo() const
	{
		return m_overridesTempo && m_tempoControlTrack.ControlPoints().size() != 0;
	}

	void Block::SetOverridesTempo(bool overridesTempo)
	{
		m_overridesTempo = overridesTempo;
	}

	TempoControlTrack & Block::GetTempoControlTrack()
	{
		return m_tempoControlTrack;
	}

	NoteTrack *Block::CreateNoteTrack(std::string name)
	{
		NoteTrack *track = new NoteTrack(name);
		m_tracks.push_back(track);
		return track;
	}

	void Block::RemoveNoteTrack(NoteTrack *track)
	{
		VecUtil::Delete(m_tracks, track);
	}

	const std::vector<NoteTrack *> &Block::NoteTracks() const
	{
		return m_tracks;
	}

	ControlTrack *Block::CreateParameterTrack(std::string name)
	{
		ControlTrack *track = new ControlTrack(name);
		m_parameterTracks.push_back(track);
		return track;
	}

	void Block::RemoveParameterTrack(ControlTrack *track)
	{
		VecUtil::Delete(m_parameterTracks, track);
	}

	const std::vector<ControlTrack *> &Block::ParameterTracks() const
	{
		return m_parameterTracks;
	}

	TrackTime Block::GetLength()
	{
		return m_length;
	}

	void Block::SetLength(TrackTime time)
	{
		m_length = time;
	}

	const std::vector<BlockTransition *> &Block::Transitions()
	{
		return m_transitions;
	}

	void Block::AddTransition(BlockTransition *transition)
	{
		m_transitions.push_back(transition);
	}

	void Block::RemoveTransition(BlockTransition *transition)
	{
		VecUtil::Delete(m_transitions, transition);
	}
}