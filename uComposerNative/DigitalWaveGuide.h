#pragma once

#include "Synthesis.h"
#include "BiquadDouble.h"
#include "FirstOrderLowpass.h"

namespace uComposer
{
	class DigitalWaveGuide
	{
	public:
		DigitalWaveGuide()
		{
			m_delayR = new float[kBufferSize];
			m_delayL = new float[kBufferSize];
		}

		void SetDecay(float decay)
		{
			m_bridgeDecay = decay;
		}

		void Initialize(float sampleRate)
		{
			m_filterNut.SetMode(Filters::BiquadMode::Lowpass);
			m_filterNut.SetCutoff(16000.0f / sampleRate);
			m_filterNut.SetResonance(0.0f);

			m_filterBridge.SetMode(Filters::BiquadMode::Lowpass);
			m_filterBridge.SetCutoff(17000.0f / sampleRate);
			m_filterBridge.SetResonance(0.0f);


			for (int i = 0; i < kBufferSize; i++)
			{
				m_delayR[i] = 0;
				m_delayL[i] = 0;
			}
		}

		void Pluck(float amplitdue, float frequency, float sampleRate)
		{
			m_filter.Reset();

			// length of delay line
			float d = 0.5f * sampleRate / frequency;
			//std::cout << "D: " << d << std::endl;
			float frac = d - (int)d;
			//std::cout << "Frac: " << frac << std::endl;

			// sampling position for right moving wave delay line
			float dr = (1 - m_pickupPosition) * d;
			//std::cout << "DR: " << dr << std::endl;
			float fracR = dr - (int)dr;
			// sampling position for left moving wave delay line
			float dl = m_pickupPosition * d;
			//std::cout << "DL: " << dl << std::endl;
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

			Filters::FirstOrderLowpass m_filter1;
			//Filters::FirstOrderLowpass m_filter1b;
			Filters::FirstOrderLowpass m_filter2;
			//Filters::FirstOrderLowpass m_filter2b;

			for (int i = 0; i < (int)d + 1; i++)
			{
				float r;
				if (i < pr)
					r = i / pr;
				else
					r = 1 - (i - pr) / (d - pr);

				float l;
				if (i < pl)
					l = -i / pl;
				else
					l = -(1 - (i - pl) / (d - pl));

				m_delayR[(m_endDelaySamplerR.m_readPointer + i) & kWrapBuffer] = (0.5f * r);
				m_delayL[(m_endDelaySamplerL.m_readPointer + i) & kWrapBuffer] = (0.5f * l);
				
			}


			/*m_readPointerA = (m_writePointerA - (int)d) & kWrapBuffer;
			m_readPointerB = (m_writePointerB - (int)d) & kWrapBuffer;*/

			//float d = sampleRate / frequency;
			//m_readPointer = (m_writePointer - (int)d) & kWrapBuffer;
			//float frac = d - (int)d;

			//m_lastInput = 0.0f;
			//m_lastOut = 0.0f;
			//m_n = (1 - frac) / (1 + frac);


			//for (int i = 0; i < kBufferSize; i++)
			//{
			//	m_buffer[(m_readPointer + i) & kWrapBuffer] = m_noise[i];
			//}

			//for (int i = 0; i < d * 4; i++)
			//{
			//	Update();
			//}
		}


		float m_pickupPosition = 0.2f;
		float m_pluckPosition = 1.0f / 4.0f;

		float m_bridgeDecay = 1.0f;
		float m_nutDecay = 1.0f;

		float Update()
		{
			// Read sample from delay line at pickups
			float pickupR = SampleDelayLine(m_delayR, m_pickupDelaySamplerR);
			float pickupL = SampleDelayLine(m_delayL, m_pickupDelaySamplerL);

			float rEnd = SampleDelayLine(m_delayR, m_endDelaySamplerR);
			float lEnd = SampleDelayLine(m_delayL, m_endDelaySamplerL);

			m_delayL[m_writePointerL] = m_nutDecay * -rEnd;
			m_delayR[m_writePointerR] = m_bridgeDecay * m_filter.Process(-lEnd);
			//m_delayR[m_writePointerR] = (float)m_filterBridge.Process(m_bridgeDecay * -lEnd);

			float pickupOutput = pickupR + pickupL;


			// Note: These are always the same, so there's not really any reason
			//	to have two separate variables.
			m_writePointerL = (m_writePointerL + 1) & kWrapBuffer;
			m_writePointerR = (m_writePointerR + 1) & kWrapBuffer;

			m_pickupDelaySamplerR.m_readPointer = (m_pickupDelaySamplerR.m_readPointer + 1) & kWrapBuffer;
			m_pickupDelaySamplerL.m_readPointer = (m_pickupDelaySamplerL.m_readPointer + 1) & kWrapBuffer;
			m_endDelaySamplerR.m_readPointer = (m_endDelaySamplerR.m_readPointer + 1) & kWrapBuffer;
			m_endDelaySamplerL.m_readPointer = (m_endDelaySamplerL.m_readPointer + 1) & kWrapBuffer;

			return pickupOutput;



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

		Filters::BiquadDouble m_filterNut;
		Filters::BiquadDouble m_filterBridge;
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