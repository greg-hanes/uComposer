#pragma once

#include <cstdint>
#include <cmath>
#include "TimeSignature.h"

namespace uComposer
{
	// Ok... now what... back to parameter automation.

	class Tempo
	{
	public:
		Tempo();
		double GetBPM() const;
		void SetBPM(double bpm);

		const TimeSignature GetTimeSignature() const;
		void SetTimeSignature(Count count, Quantum quantum);

	private:
		double m_bpm;
		double m_bps;
		TimeSignature m_timeSignature;
	};
}