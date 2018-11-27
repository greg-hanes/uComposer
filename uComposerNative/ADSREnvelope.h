#pragma once
#include "MathUtil.h"
#include <cmath>
namespace uComposer
{
	class ADSR
	{
	public:
		enum ADSREnvelopeMode : int
		{
			Idle,
			Attack,
			Decay,
			Sustain,
			Release
		};

		static const float kTargetRatioA;
		static const float kTargetRatioDR;

		class State
		{
			friend class ADSR;
		public:
			void Trigger()
			{
				m_state = ADSREnvelopeMode::Attack;
			}

			void Release()
			{
				if (m_state != ADSREnvelopeMode::Idle)
					m_state = ADSREnvelopeMode::Release;
			}

			ADSREnvelopeMode GetState() const
			{
				return m_state;
			}

			float GetEnvelopeValue() const
			{
				return m_currentValue;
			}
		private:
			ADSREnvelopeMode m_state = ADSREnvelopeMode::Idle;
			float m_currentValue = 0;
		};

		class Configuration
		{
			friend class ADSR;
		public:
			float GetSampleRate()
			{
				return m_sampleRate;
			}

			float GetAttack() const
			{
				return m_attackTime;
			}

			float GetDecay() const
			{
				return m_decayTime;
			}

			float GetSustainLevel() const
			{
				return m_sustainLevel;
			}

			float GetRelease() const
			{
				return m_releaseTime;
			}

			void SetSampleRate(float sampleRate)
			{
				m_sampleRate = sampleRate;
				SetAttack(m_attackTime);
			}

			void SetAttack(float timeMS)
			{
				m_attackTime = timeMS;
				float sampleAttackTime = m_attackTime * m_sampleRate / 1000.0f;
				m_attackCoef = CalculateCoefficient(sampleAttackTime, kTargetRatioA);
				m_attackBase = (1 + kTargetRatioA) * (1 - m_attackCoef);
			}

			void SetDecayTime(float timeMS)
			{
				m_decayTime = timeMS;

				float sampleDecayTime = m_decayTime * m_sampleRate / 1000.0f;
				m_decayCoef = CalculateCoefficient(sampleDecayTime, kTargetRatioDR);
				m_decayBase = (m_sustainLevel - kTargetRatioDR) * (1.0f - m_decayCoef);
			}

			void SetSustainLevel(float value)
			{
				m_sustainLevel = value;
				m_decayBase = (m_sustainLevel - kTargetRatioDR) * (1 - m_decayCoef);
			}

			void SetReleaseTime(float timeMS)
			{
				m_releaseTime = timeMS;
				float sampleReleaseTime = m_releaseTime * m_sampleRate / 1000.0f;
				m_releaseCoef = CalculateCoefficient(sampleReleaseTime, kTargetRatioDR);
				m_releaseBase = -kTargetRatioDR * (1 - m_releaseCoef);
			}

		private:
			static float CalculateCoefficient(float rate, float targetRatio)
			{
				return (float)Math::FastExp(
					(float)-std::log((1.0f + targetRatio) / targetRatio) / rate);
			}

			float m_sampleRate;

			float m_attackTime;
			float m_decayTime;
			float m_sustainLevel;
			float m_releaseTime;

			
			float m_attackBase;
			float m_attackCoef;
			float m_decayBase;
			float m_decayCoef;

			float m_releaseBase;
			float m_releaseCoef;
		};

		static void Update(Configuration &config, State &state)
		{
			switch (state.m_state)
			{
			case ADSREnvelopeMode::Attack:
				state.m_currentValue = config.m_attackBase + state.m_currentValue * config.m_attackCoef;
				if (state.m_currentValue >= 1)
				{
					state.m_currentValue = 1;
					state.m_state = ADSREnvelopeMode::Decay;
				}
				break;
			case ADSREnvelopeMode::Decay:
				state.m_currentValue = config.m_decayBase + state.m_currentValue * config.m_decayCoef;
				if (state.m_currentValue <= config.m_sustainLevel)
				{
					state.m_currentValue = config.m_sustainLevel;
					state.m_state = ADSREnvelopeMode::Sustain;
				}
				break;
			case ADSREnvelopeMode::Sustain:
				break;
			case ADSREnvelopeMode::Release:
				state.m_currentValue = config.m_releaseBase + state.m_currentValue * config.m_releaseCoef;
				if (state.m_currentValue <= 0)
				{
					state.m_currentValue = 0;
					state.m_state = ADSREnvelopeMode::Idle;
				}
				break;
			case ADSREnvelopeMode::Idle:
				break;
			}
		}
	};

	namespace Envelope
	{
		enum ADSREnvelopeMode : int
		{
			Idle,
			Attack,
			Decay,
			Sustain,
			Release
		};

		class ADSREnvelope
		{
		public:
			void Trigger();
			void Trigger(float velocity);
			void Release();

			ADSREnvelopeMode GetState() const;
			float GetEnvelopeValue() const;
			void Update();

			void SetAttack(float timeMS, int sampleRate);
			void SetDecayTime(float timeMS, int sampleRate);
			void SetSustainLevel(float value);
			void SetReleaseTime(float timeMS, int sampleRate);
			

		private:
			float CalculateCoefficient(float rate, float targetRatio);

			ADSREnvelopeMode m_state = ADSREnvelopeMode::Idle;

			float m_currentValue = 0;
			bool m_trigger = false;
			bool m_gated = false;

			float m_attackTime;
			float m_decayTime;
			float m_sustainLevel;
			float m_releaseTime;


			float m_attackBase;
			float m_attackCoef;
			float m_decayBase;
			float m_decayCoef;

			float m_releaseBase;
			float m_releaseCoef;

			static const float kTargetRatioA;
			static const float kTargetRatioDR;
		};
	}
}