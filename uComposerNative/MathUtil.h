#pragma once
#include <math.h>

namespace uComposer
{
	namespace Math
	{
		const float kPI = 3.141592653589793238f;
		const float kAmpToDB = 8.685889638065036553f;
		const float kInvAmpToDB = 0.11512925464970f;

		template<typename T>
		inline T Min(T x, T y) { return x < y ? x : y; }

		template<typename T>
		inline T Max(T x, T y) { return x < y ? y : x; }

		template<typename T>
		inline T Bounded(T x, T min, T max) { return x < min ? min : (x > max ? max : x); }

		template<typename T>
		inline T Lerp(T start, T end, T t)
		{
			return (1 - t) * start + t * end;
		}

		template<typename T>
		inline T Sign(T x) { return x < 0 ? (T)-1 : (T)1; }

		template<typename T>
		inline T Abs(T x) { return x >= 0 ? x : -x; }

		template<typename T>
		inline T Floor(T x) { return (T)((int)x - (x < (int)x)); }

		template<>
		inline double Floor<double>(double x) { return (long)x - (x < (long)x); }

		template<typename T>
		inline T Frac(T x) { return x - Floor(x); }

		inline double DBToAmp(double dB)
		{
			return exp(kInvAmpToDB * dB);
		}

		inline double AmpToDB(double amp)
		{
			return kAmpToDB * log(Abs(amp));
		}

		// Change this to "dt" and multiply by inverseSampleRate if necessary.
		inline void StepPhase(float &phase, float frequency, float timeStep)
		{
			phase += frequency * timeStep;
			phase = Frac(phase);
		}

		inline float NoteNumberToFrequency(int noteNumber)
		{
			return 440.0f * (float)pow(2.0, (noteNumber - 69) / 12.0);
		}

		inline float VelocityNumberToDecimal(int velocityNumber)
		{
			return velocityNumber / 255.0f;
		}

		inline float FastExp(float x)
		{
			return (float)exp(x);
		}

		inline float FastSine(float t)
		{
			float f = Math::Abs(Math::Frac(t - 0.25f) - 0.5f);
			float h = Math::Abs(Math::Frac(t * 4) - 0.5f);

			return (-1.0f + f * f * (24.0f - 32.0f * f)) * (1.028519f - 0.0570379f * h);
		}

		inline float AllpassInterpolation(float input, float n, float &lastInput, float &lastOutput)
		{
			float output = n * input + lastInput - n * lastOutput;
			lastOutput = output;
			lastInput = input;
			return output;
		}
	}
}