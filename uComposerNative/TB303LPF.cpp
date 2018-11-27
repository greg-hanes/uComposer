#include "TB303LPF.h"
#include <iostream>

namespace uComposer
{
	namespace Filters
	{
		void TB303LPF::Reset()
		{
			il = 0;
			uc = 0;
		}

		float TB303LPF::GetResonance() const
		{
			return m_resonance;
		}

		float TB303LPF::GetCutoff() const
		{
			return m_cutoff;
		}

		void TB303LPF::SetResonance(float resonance)
		{
			m_resonance = resonance;
		}

		void TB303LPF::SetCutoff(float cutoff)
		{
			m_cutoff = cutoff;
		}

		// Back this by a simple function?
		float TB303LPF::Process(float in)
		{
			il += m_cutoff * (in - uc - m_resonance * il);
			uc += il * m_cutoff;
			return uc;
		}
	}
}