#pragma once
#include "BiquadFilter.h"

namespace uComposer
{
	namespace Filters
	{

		class BiquadDouble
		{
		public:
			BiquadDouble();

			void Reset();

			BiquadMode GetMode() const;
			void SetMode(BiquadMode mode);

			double GetCutoff() const;
			void SetCutoff(double value);

			double GetResonance() const;
			void SetResonance(double value);

			double Process(double in);

		private:
			BiquadMode m_mode;
			double m_cutoff;
			double m_resonance;

			double m_b0;
			double m_b1;
			double m_b2;
			double m_b3;
			double m_b4;

			double m_q;
			double m_p;
			double m_f;
		};
	}
}