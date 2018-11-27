#include "BiquadDouble.h"

#include <iostream>

namespace uComposer
{
	namespace Filters
	{
		BiquadDouble::BiquadDouble() :
			m_mode(BiquadMode::Lowpass),
			m_cutoff(1000.0 / 44100.0), // Cutoff is 0-1 which maps linearly to a 0-nyquist scale. 44100 isn't always right.
			m_resonance(0.35)
		{
			Reset();
		}

		BiquadMode BiquadDouble::GetMode() const
		{
			return m_mode;
		}

		void BiquadDouble::SetMode(BiquadMode mode)
		{
			m_mode = mode;
			Reset();
		}

		double BiquadDouble::GetCutoff() const
		{
			return m_cutoff;
		}

		void BiquadDouble::SetCutoff(double value)
		{
			m_cutoff = value;

			m_q = 1 - m_cutoff;
			m_p = m_cutoff + 0.8f * m_cutoff * m_q;
			m_f = m_p + m_p - 1;
			m_q = m_resonance * (1 + 0.5f * m_q * (1 - m_q + 5.6f * m_q * m_q));
		}

		double BiquadDouble::GetResonance() const
		{
			return m_resonance;
		}

		void BiquadDouble::SetResonance(double value)
		{
			m_resonance = value;
			m_q = 1 - m_cutoff;
			m_p = m_cutoff + 0.8f * m_cutoff * m_q;
			m_f = m_p + m_p - 1;
			m_q = m_resonance * (1 + 0.5f * m_q * (1 - m_q + 5.6f * m_q * m_q));
		}

		double BiquadDouble::Process(double input)
		{
			double t1, t2;
			double in = input;
			in -= m_q * m_b4;
			t1 = m_b1;
			m_b1 = (in + m_b0) * m_p - m_b1 * m_f;
			t2 = m_b2;
			m_b2 = (m_b1 + t1) * m_p - m_b2 * m_f;
			t1 = m_b3;
			m_b3 = (m_b2 + t2) * m_p - m_b3 * m_f;
			m_b4 = (m_b3 + t1) * m_p - m_b4 * m_f;
			m_b4 = m_b4 - m_b4 * m_b4 * m_b4 * 0.16666667;
			m_b0 = in;

			switch (m_mode)
			{
			case BiquadMode::Lowpass:
				return m_b4;
			case BiquadMode::Bandpass:
				return (3.0 * (m_b3 - m_b4));
			case BiquadMode::Highpass:
				return (in - m_b4);
			}

			return 0;
		}

		void BiquadDouble::Reset()
		{
			m_b0 = 0;
			m_b1 = 0;
			m_b2 = 0;
			m_b3 = 0;
			m_b4 = 0;


			m_q = 1 - m_cutoff;
			m_p = m_cutoff + 0.8 * m_cutoff * m_q;
			m_f = m_p + m_p - 1;
			m_q = m_resonance * (1 + 0.5 * m_q * (1 - m_q + 5.6 * m_q * m_q));
		}
	}
}