#pragma once
#include <cstdlib>
#include <cstdint>
#include "TB303LPF.h"
#include <iostream>
#include "Synthesis.h"
namespace uComposer
{
	class KarplusStrongString
	{
	public:
		KarplusStrongString()
		{
			m_noise = new float[kBufferSize];
			m_buffer = new float[kBufferSize];
		}

		float GetDecay() const
		{
			return m_decay;
		}

		void SetDecay(float decay)
		{
			m_decay = decay;
		}

		void Initialize(float sampleRate)
		{
			m_filter.SetMode(Filters::BiquadMode::Lowpass);
			m_filter.SetCutoff(15000.0f / sampleRate);
			m_filter.SetResonance(0.0f);

			for (int i = 0; i < kBufferSize; i++)
			{
				m_buffer[i] = 0;
				m_noise[i] = (m_filter.Process((((float)rand()) / RAND_MAX) - 0.5f));
			}
		}

		void Pluck(float amplitdue, float frequency, float sampleRate)
		{
			float d = sampleRate / frequency;
			m_readPointer = (m_writePointer - (int)d) & kWrapBuffer;
			float frac = d - (int)d;

			m_lastInput = 0.0f;
			m_lastOut = 0.0f;
			m_n = (1 - frac) / (1 + frac);

			
			for (int i = 0; i < kBufferSize; i++)
			{
				m_buffer[(m_readPointer + i) & kWrapBuffer] = m_noise[i];
			}

			for (int i = 0; i < d * 4; i++)
			{
				Update();
			}
		}

		// Instrumentation - which instruments are playing
		//		Cultural associations
		// Key/Mode
		// Density of musical material.
		// Consonance and dissonance.
		//		Relationship between pitches.
		// 

		float AllpassInterpolation(float input)
		{
			float output = m_n * input + m_lastInput - m_n * m_lastOut;
			m_lastOut = output;
			m_lastInput = input;
			return output;

			

			//OUT(n) = a * x(n) + x(n-1) - a * OUT(n-1)
			//input += -m_n * m_lastInput;
			//float output = m_lastInput + input * m_n;
			//m_lastInput = input;

			//m_lastOut = output;
			//return output;
		}

		float Update()
		{
			// Read sample from delay line.
			float input = m_buffer[m_readPointer];
			// Input is whole number sample delayed.
			input = Math::AllpassInterpolation(input, m_n, m_lastInput, m_lastOut);
			//input = AllpassInterpolation(input);

			// Filter delay line output.
			float filterOutput = m_filter.Process(input);

			// Put sample in delay line.
			m_buffer[m_writePointer] = m_decay * filterOutput + (1 - m_decay) * input;

			m_writePointer = (m_writePointer + 1) & kWrapBuffer;
			m_readPointer = (m_readPointer + 1) & kWrapBuffer;

			return filterOutput;
		}
		Filters::Biquad m_filter;

	private:
		static const int kBufferSize = 1 << 14;
		static const int kWrapBuffer = kBufferSize - 1;
		float *m_noise;
		float *m_buffer;

		int m_writePointer = 0;
		int m_readPointer = 0;

		float m_n;
		float m_lastOut;
		float m_lastInput;

		float m_decay = 1.0f;
	};
}