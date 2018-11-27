#pragma once
#include "MathUtil.h"

namespace uComposer
{
	namespace Oscillator
	{
		namespace Functions
		{
			inline float SquareWave(float t)
			{
				return 2.0f * (t < 0.5f) - 1;
			}

			inline float BandlimitedSquareWave(float t)
			{
				static const float squareHarmonicPhases[5] = { 1,3,5,7,9 };
				static const float squareHarmonicAmplitudes[5] = { 1, 1 / 3.0f, 1 / 5.0f, 1 / 7.0f, 1 / 9.0f };

				float val = 0;
				for (int i = 0; i < 5; i++)
				{
					val += squareHarmonicAmplitudes[i] * Math::FastSine(t * squareHarmonicPhases[i]);
				}
				return val;
			}

			inline float SawtoothWave(float t)
			{
				return (2 * (1 - t) - 1);
			}

			inline float BandlimitedSawtooth(float t)
			{
				static const float sawHarmonicPhases[9] = { 1,2,3,4,5,6,7,8,9 };
				static const float sawHarmonicAmplitudes[9] = { 1, 1 / 2.0f, 1 / 3.0f, 1 / 4.0f, 1 / 5.0f, 1 / 6.0f, 1 / 7.0f, 1 / 8.0f, 1 / 9.0f };

				float val = 0;
				for (int i = 0; i < 9; i++)
				{
					val += sawHarmonicAmplitudes[i] * Math::FastSine(t * sawHarmonicPhases[i]);
				}
				return val;
			}

			inline float TriangleWave(float t)
			{
				return 2 * Math::Abs(2 * t - 1) - 1;
			}

			inline float BandLimitedTriangleWave(float t)
			{
				static const float triangleHarmonicPhases[5] = { 1, 3, 5, 7, 9 };
				static const float triangleHarmonicAmplitudes[5] = { 1, 1 / 9.0f, 1 / 25.0f, 1 / 49.0f, 1 / 81.0f };
				float val = 0;
				for (int i = 0; i < 5; i++)
				{
					val += triangleHarmonicAmplitudes[i] * Math::FastSine(t * triangleHarmonicPhases[i]);
				}
				return val;
			}
		}
	}
}