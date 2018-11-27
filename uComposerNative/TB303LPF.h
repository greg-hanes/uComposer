#pragma once

#include "IEffect.h"

namespace uComposer
{
	namespace Filters
	{
		class TB303LPF
		{
		public:
			void Reset();
			float Process(float in);

			float GetResonance() const;
			float GetCutoff() const;

			void SetResonance(float resonance);
			void SetCutoff(float cutoff);

		private:
			float m_cutoff = 0;
			float m_resonance = 0;

			float il = 0;
			float uc = 0;
		};
	}
}