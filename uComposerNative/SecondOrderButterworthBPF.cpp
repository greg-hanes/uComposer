#include "SecondOrderButterworthBPF.h"
#include <math.h>
#include "MathUtil.h"

namespace uComposer
{
	namespace Filters
	{
		void SecondOrderButterworthBPF::Reset()
		{
			xn1 = 0;
			xn2 = 0;
			yn1 = 0;
			yn2 = 0;
		}

		void SecondOrderButterworthBPF::UpdateCoefficients()
		{
			double C = 1.0 / tan(Math::kPI * m_freq * m_bw);
			double D = 2 * cos(2 * Math::kPI * m_freq);
			a0 = 1.0 / (1 + C);
			a1 = 0;
			a2 = -a0;
			b1 = -a0 * C * D;
			b2 = a0 * (C - 1);
		}

		void SecondOrderButterworthBPF::SetBandwidth(double bw)
		{
			// Assuming bw is already divided by sample rate.
			m_bw = bw;
			UpdateCoefficients();
		}

		void SecondOrderButterworthBPF::SetFrequency(double freq)
		{
			m_freq = freq;
			UpdateCoefficients();
		}

		double SecondOrderButterworthBPF::Process(double in)
		{
			xn2 = xn1;
			xn1 = in;

			double yn = a0 * in + a1 * xn1 + a2 * xn2 - b1 * yn1 - b2 * yn2;

			yn2 = yn1;
			yn1 = yn;

			return yn;
		}
	}
}