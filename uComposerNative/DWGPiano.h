#pragma once

#include "Synthesis.h"
#include "FirstOrderLowpass.h"

namespace uComposer
{
	class DWGPiano
	{
	public:
		DWGPiano()
		{
			m_delayR = new float[kBufferSize];
			m_delayL = new float[kBufferSize];
		}

		void Initialize(float sampleRate)
		{
			for (int i = 0; i < kBufferSize; i++)
			{
				m_delayR[i] = 0;
				m_delayL[i] = 0;
			}

			m_filter.m_a0 = 0.5f;
			m_filter.m_a1 = 0.5f;
		}

		void Pluck(float amplitdue, float frequency, float sampleRate)
		{
			// length of delay line
			float d = 0.5f * sampleRate / frequency;
			float frac = d - (int)d;

			// sampling position for right moving wave delay line
			float dr = (1 - m_pickupPosition) * d;
			float fracR = dr - (int)dr;
			// sampling position for left moving wave delay line
			float dl = m_pickupPosition * d;
			float fracL = dl - (int)dl;

			// Setup endpoint samplers
			m_endDelaySamplerR.m_lastInput = 0;
			m_endDelaySamplerR.m_lastOutput = 0;
			m_endDelaySamplerR.m_readPointer = (m_writePointerR - (int)d) & kWrapBuffer;
			m_endDelaySamplerR.m_n = (1 - frac) / (1 + frac);

			m_endDelaySamplerL.m_lastInput = 0;
			m_endDelaySamplerL.m_lastOutput = 0;
			m_endDelaySamplerL.m_readPointer = (m_writePointerL - (int)d) & kWrapBuffer;
			m_endDelaySamplerL.m_n = (1 - frac) / (1 + frac);

			// Setup pickup samplers
			m_pickupDelaySamplerR.m_lastInput = 0;
			m_pickupDelaySamplerR.m_lastOutput = 0;
			m_pickupDelaySamplerR.m_readPointer = (m_writePointerR - (int)dr) & kWrapBuffer;
			m_pickupDelaySamplerR.m_n = (1 - fracR) / (1 + fracR);

			m_pickupDelaySamplerL.m_lastInput = 0;
			m_pickupDelaySamplerL.m_lastOutput = 0;
			m_pickupDelaySamplerL.m_readPointer = (m_writePointerL - (int)dl) & kWrapBuffer;
			m_pickupDelaySamplerL.m_n = (1 - fracL) / (1 + fracL);


			for (int i = 0; i < kBufferSize; i++)
			{
				m_delayL[i] = 0;
				m_delayR[i] = 0;
			}

			float pr = (1 - m_pluckPosition) * d;
			float pl = m_pluckPosition * d;



			m_delayR[(m_endDelaySamplerR.m_readPointer + (int)pr) & kWrapBuffer] = 1;
			m_delayL[(m_endDelaySamplerL.m_readPointer + (int)pl) & kWrapBuffer] = 1;

			Filters::FirstOrderLowpass m_filterL1;
			m_filterL1.m_a0 = 0.5f;
			m_filterL1.m_a1 = 0.5f;
			Filters::FirstOrderLowpass m_filterL2;
			m_filterL2.m_a0 = 0.3333f;
			m_filterL2.m_a1 = 0.3333f;
			m_filterL2.m_a2 = 0.3333f;
			Filters::FirstOrderLowpass m_filterL3;
			m_filterL3.m_a0 = 0.25f;
			m_filterL3.m_a1 = 0.25f;
			m_filterL3.m_a2 = 0.25f;
			m_filterL3.m_a3 = 0.25f;

			Filters::FirstOrderLowpass m_filterR1;
			m_filterR1.m_a0 = 0.5f;
			m_filterR1.m_a1 = 0.5f;
			Filters::FirstOrderLowpass m_filterR2;
			m_filterR2.m_a0 = 0.3333f;
			m_filterR2.m_a1 = 0.3333f;
			m_filterR2.m_a2 = 0.3333f;
			Filters::FirstOrderLowpass m_filterR3;
			m_filterR3.m_a0 = 0.25f;
			m_filterR3.m_a1 = 0.25f;
			m_filterR3.m_a2 = 0.25f;
			m_filterR3.m_a3 = 0.25f;

			for (int i = 0; i < (int)d; i++)
			{
				float valueR = 0;
				float valueL = 0;
				if (i > pr - m_hammerWidth * d && i < pr + m_hammerWidth * d)
				{
					valueR = 0.015f;
				}
				if (i > pl - m_hammerWidth * d && i < pl + m_hammerWidth * d)
				{
					valueL = 0.015f;
				}

				//float valueR = m_delayR[(m_endDelaySamplerR.m_readPointer + i) & kWrapBuffer];
				//float valueL = m_delayL[(m_endDelaySamplerL.m_readPointer + i) & kWrapBuffer];

				m_delayR[(m_endDelaySamplerR.m_readPointer + i) & kWrapBuffer] =
					m_filterR1.Process(valueR) +
					m_filterR2.Process(valueR) +
					m_filterR3.Process(valueR);

				m_delayL[(m_endDelaySamplerL.m_readPointer + i) & kWrapBuffer] =
					m_filterL1.Process(valueL) +
					m_filterL2.Process(valueL) +
					m_filterL3.Process(valueL);
			}

			//for (int i = 0; i < (int)d + 1; i++)
			//{
			//	float r = 0;
			//	float l = 0;
			//	if (i > pr - m_hammerWidth * d && i < pr + m_hammerWidth * d)
			//	{
			//		r = 0.15f;
			//	}
			//	if (i > pl - m_hammerWidth * d && i < pl + m_hammerWidth * d)
			//	{
			//		l = 0.15f;
			//	}

			//	m_delayR[(m_endDelaySamplerR.m_readPointer + i) & kWrapBuffer] = r;
			//	m_delayL[(m_endDelaySamplerL.m_readPointer + i) & kWrapBuffer] = l;
			//}

			m_filter.Reset();
			m_integratorState = 0.0f;
			m_integratorState2 = 0.0f;
		}


		float m_pickupPosition = 0.25f;
		float m_pluckPosition = 1.0f / 2.0f;
		float m_hammerWidth = 1.0f / 16.0f;

		float m_bridgeDecay = 1.0f;
		float m_nutDecay = 1.0f;

		float m_integratorState = 0.0f;
		float m_integratorDecay = 0.999f;

		float m_integratorState2 = 0.0f;
		float m_integratorDecay2 = 0.999f;

		void SetDecay(float value)
		{
			m_bridgeDecay = value;
		}

		float Update()
		{
			// Read sample from delay line at pickups
			float pickupR = SampleDelayLine(m_delayR, m_pickupDelaySamplerR);
			float pickupL = SampleDelayLine(m_delayL, m_pickupDelaySamplerL);

			float rEnd = SampleDelayLine(m_delayR, m_endDelaySamplerR);
			float lEnd = SampleDelayLine(m_delayL, m_endDelaySamplerL);

			m_delayL[m_writePointerL] = m_nutDecay * -rEnd;
			m_delayR[m_writePointerR] = m_bridgeDecay * m_filter.Process(-lEnd);

			float pickupOutput = 0.5f * (pickupR + pickupL);


			// Note: These are always the same, so there's not really any reason
			//	to have two separate variables.
			m_writePointerL = (m_writePointerL + 1) & kWrapBuffer;
			m_writePointerR = (m_writePointerR + 1) & kWrapBuffer;

			m_pickupDelaySamplerR.m_readPointer = (m_pickupDelaySamplerR.m_readPointer + 1) & kWrapBuffer;
			m_pickupDelaySamplerL.m_readPointer = (m_pickupDelaySamplerL.m_readPointer + 1) & kWrapBuffer;
			m_endDelaySamplerR.m_readPointer = (m_endDelaySamplerR.m_readPointer + 1) & kWrapBuffer;
			m_endDelaySamplerL.m_readPointer = (m_endDelaySamplerL.m_readPointer + 1) & kWrapBuffer;


			m_integratorState += pickupOutput;
			m_integratorState *= m_integratorDecay;

			m_integratorState2 += m_integratorState;
			m_integratorState2 *= m_integratorDecay2;
			return m_integratorState2;

			//pickupOutput = m_integratorState + pickupOutput;
			//m_integratorState = m_integratorDecay * pickupOutput;

			//return pickupOutput;



			//float input = m_buffer[m_readPointer];
			//// Input is whole number sample delayed.
			//input = Math::AllpassInterpolation(input, m_n, m_lastInput, m_lastOut);
			////input = AllpassInterpolation(input);

			//// Filter delay line output.
			//float filterOutput = m_filter.Process(input);

			//// Put sample in delay line.
			//m_buffer[m_writePointer] = m_decay * filterOutput + (1 - m_decay) * input;

			//m_writePointer = (m_writePointer + 1) & kWrapBuffer;
			//m_readPointer = (m_readPointer + 1) & kWrapBuffer;

			//return filterOutput;
		}

	private:
		static const int kBufferSize = 1 << 18;
		static const int kWrapBuffer = kBufferSize - 1;

		struct DelayLineSampler
		{
			int m_readPointer = 0;
			float m_n;
			float m_lastOutput;
			float m_lastInput;
		};

		float SampleDelayLine(float *delayLine, DelayLineSampler &sampler)
		{
			float input = delayLine[sampler.m_readPointer];
			return Math::AllpassInterpolation(input,
				sampler.m_n,
				sampler.m_lastInput,
				sampler.m_lastOutput);
		}

		Filters::FirstOrderLowpass m_filter;



		// Delay line right
		float *m_delayR;
		int m_writePointerR = 0;

		// Delay line left
		float *m_delayL;
		int m_writePointerL = 0;


		// Delay line sample points.
		DelayLineSampler m_endDelaySamplerR;
		DelayLineSampler m_pickupDelaySamplerR;
		DelayLineSampler m_endDelaySamplerL;
		DelayLineSampler m_pickupDelaySamplerL;
	};
}