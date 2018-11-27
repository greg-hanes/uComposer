#include "BiquadFilter.h"

#include <iostream>

namespace uComposer
{
	namespace Filters
	{
		Biquad::Biquad() :
			m_mode(BiquadMode::Lowpass),
			m_cutoff(1000.0f / 44100.0f), // Cutoff is 0-1 which maps linearly to a 0-nyquist scale. 44100 isn't always right.
			m_resonance(0.35f)
		{
			Reset();
		}

		BiquadMode Biquad::GetMode() const
		{
			return m_mode;
		}

		void Biquad::SetMode(BiquadMode mode)
		{
			m_mode = mode;
			Reset();
		}

		float Biquad::GetCutoff() const
		{
			return m_cutoff;
		}

		void Biquad::SetCutoff(float value)
		{
			m_cutoff = value;

			m_q = 1 - m_cutoff;
			m_p = m_cutoff + 0.8f * m_cutoff * m_q;
			m_f = m_p + m_p - 1;
			m_q = m_resonance * (1 + 0.5f * m_q * (1 - m_q + 5.6f * m_q * m_q));
		}

		float Biquad::GetResonance() const
		{
			return m_resonance;
		}

		void Biquad::SetResonance(float value)
		{
			m_resonance = value;
			m_q = 1 - m_cutoff;
			m_p = m_cutoff + 0.8f * m_cutoff * m_q;
			m_f = m_p + m_p - 1;
			m_q = m_resonance * (1 + 0.5f * m_q * (1 - m_q + 5.6f * m_q * m_q));
		}

		float Biquad::Process(float input)
		{
			float t1, t2;
			float in = input;
			in -= m_q * m_b4;
			t1 = m_b1;
			m_b1 = (in + m_b0) * m_p - m_b1 * m_f;
			t2 = m_b2;
			m_b2 = (m_b1 + t1) * m_p - m_b2 * m_f;
			t1 = m_b3;
			m_b3 = (m_b2 + t2) * m_p - m_b3 * m_f;
			m_b4 = (m_b3 + t1) * m_p - m_b4 * m_f;
			m_b4 = m_b4 - m_b4 * m_b4 * m_b4 * 0.16666667f;
			m_b0 = in;

			switch (m_mode)
			{
			case BiquadMode::Lowpass:
				return m_b4;
			case BiquadMode::Bandpass:
				return (3.0f * (m_b3 - m_b4));
			case BiquadMode::Highpass:
				return (in - m_b4);
			}

			return 0;
		}

		void Biquad::Reset()
		{
			m_b0 = 0;
			m_b1 = 0;
			m_b2 = 0;
			m_b3 = 0;
			m_b4 = 0;

			
			m_q = 1 - m_cutoff;
			m_p = m_cutoff + 0.8f * m_cutoff * m_q;
			m_f = m_p + m_p - 1;
			m_q = m_resonance * (1 + 0.5f * m_q * (1 - m_q + 5.6f * m_q * m_q));
		}
	}
}