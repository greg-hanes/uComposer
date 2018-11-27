#include "Tempo.h"

namespace uComposer
{
	Tempo::Tempo()
	{
		SetBPM(120);
		m_timeSignature.count = 4;
		m_timeSignature.quantum = Quantum::Four;
	}

	double Tempo::GetBPM() const
	{
		return m_bpm;
	}

	void Tempo::SetBPM(double bpm)
	{
		m_bpm = bpm;
		m_bps = m_bpm / 60.0f;
	}

	const TimeSignature Tempo::GetTimeSignature() const
	{
		return m_timeSignature;
	}

	void Tempo::SetTimeSignature(Count count, Quantum quantum)
	{
		m_timeSignature.count = count;
		m_timeSignature.quantum = quantum;
	}
}