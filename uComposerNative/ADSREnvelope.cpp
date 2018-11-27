#include "ADSREnvelope.h"
#include <cmath>
#include <iostream>

namespace uComposer
{
	const float ADSR::kTargetRatioA = 0.3f;
	const float ADSR::kTargetRatioDR = 0.001f;

	namespace Envelope
	{
		//////////////////////////
		// ADSR Parameters Implementation

		const float ADSREnvelope::kTargetRatioA = 0.3f;
		const float ADSREnvelope::kTargetRatioDR = 0.001f;

		void ADSREnvelope::SetAttack(float timeMS, int sampleRate)
		{
			m_attackTime = timeMS;
			float sampleAttackTime = m_attackTime * sampleRate / 1000.0f;
			m_attackCoef = CalculateCoefficient(sampleAttackTime, kTargetRatioA);
			m_attackBase = (1 + kTargetRatioA) * (1 - m_attackCoef);
		}

		void ADSREnvelope::SetDecayTime(float timeMS, int sampleRate)
		{
			m_decayTime = timeMS;

			float sampleDecayTime = m_decayTime * sampleRate / 1000.0f;
			m_decayCoef = CalculateCoefficient(sampleDecayTime, kTargetRatioDR);
			m_decayBase = (m_sustainLevel - kTargetRatioDR) * (1.0f - m_decayCoef);
		}

		void ADSREnvelope::SetSustainLevel(float value)
		{
			m_sustainLevel = value;
			m_decayBase = (m_sustainLevel - kTargetRatioDR) * (1 - m_decayCoef);
		}

		void ADSREnvelope::SetReleaseTime(float timeMS, int sampleRate)
		{
			m_releaseTime = timeMS;
			float sampleReleaseTime = m_releaseTime * sampleRate / 1000.0f;
			m_releaseCoef = CalculateCoefficient(sampleReleaseTime, kTargetRatioDR);
			m_releaseBase = -kTargetRatioDR * (1 - m_releaseCoef);
		}

		float ADSREnvelope::CalculateCoefficient(float rate, float targetRatio)
		{
			return (float)Math::FastExp(
				(float)-std::log((1.0f + targetRatio) / targetRatio) / rate);
		}

		//////////////////////////
		// ADSR Envelope Implementation
		void ADSREnvelope::Trigger()
		{
			m_state = ADSREnvelopeMode::Attack;
		}

		void ADSREnvelope::Trigger(float velocity)
		{
			m_state = ADSREnvelopeMode::Attack;
		}

		void ADSREnvelope::Release()
		{
			if (m_state != ADSREnvelopeMode::Idle)
				m_state = ADSREnvelopeMode::Release;
		}

		ADSREnvelopeMode ADSREnvelope::GetState() const
		{
			return m_state;
		}

		float ADSREnvelope::GetEnvelopeValue() const
		{
			return m_currentValue;
		}

		void ADSREnvelope::Update()
		{
			switch (m_state)
			{
			case ADSREnvelopeMode::Attack:
				m_currentValue = m_attackBase + m_currentValue * m_attackCoef;
				if (m_currentValue >= 1)
				{
					m_currentValue = 1;
					m_state = ADSREnvelopeMode::Decay;
				}
				break;
			case ADSREnvelopeMode::Decay:
				m_currentValue = m_decayBase + m_currentValue * m_decayCoef;
				if (m_currentValue <= m_sustainLevel)
				{
					m_currentValue = m_sustainLevel;
					m_state = ADSREnvelopeMode::Sustain;
				}
				break;
			case ADSREnvelopeMode::Sustain:
				break;
			case ADSREnvelopeMode::Release:
				m_currentValue = m_releaseBase + m_currentValue * m_releaseCoef;
				if (m_currentValue <= 0)
				{
					m_currentValue = 0;
					m_state = ADSREnvelopeMode::Idle;
				}
				break;
			case ADSREnvelopeMode::Idle:
				break;
			}
		}
	}
}