#pragma once
#include "MathUtil.h"
#include "OscillatorFunctions.h"

namespace uComposer
{
	namespace Oscillator
	{
		template<typename T>
		class Oscillator
		{
		public:
			inline float GetValue() const
			{
				return m_currentValue;
			}

			inline void Update(float frequency, float dt)
			{
				Math::StepPhase(m_phase, frequency, dt);
				m_currentValue = static_cast<T*>(this)->Sample(m_phase);
			}

			inline void Reset()
			{
				m_phase = 0.0f;
				m_currentValue = 0.0f;
			}

		private:
			float m_currentValue;
			float m_phase;
		};

		class Sine : public Oscillator<Sine>
		{
			friend class Oscillator<Sine>;
			inline float Sample(float phase)
			{
				return Math::FastSine(phase);
			}
		};

		class Square : public Oscillator<Square>
		{
			friend class Oscillator<Square>;
			float Sample(float time)
			{
				return Functions::SquareWave(time);
			}
		};

		class Triangle : public Oscillator<Triangle>
		{
			friend class Oscillator<Triangle>;
			float Sample(float time)
			{
				return Functions::TriangleWave(time);
			}
		};

		class Sawtooth : public Oscillator<Sawtooth>
		{
			friend class Oscillator<Sawtooth>;
			inline float Sample(float time)
			{
				return Functions::SawtoothWave(time);
			}
		};
	}
}