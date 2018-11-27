#include "ADSREnvelopeComponent.h"
#include "BaseEffect.h"

namespace uComposer
{
	ControlInput * ADSREnvelopeComponent::GetAttackControl() const
	{
		return m_attack;
	}

	ControlInput * ADSREnvelopeComponent::GetDecayControl() const
	{
		return m_decay;
	}

	ControlInput * ADSREnvelopeComponent::GetSustainControl() const
	{
		return m_sustain;
	}

	ControlInput * ADSREnvelopeComponent::GetReleaseControl() const
	{
		return m_release;
	}

	void ADSREnvelopeComponent::OnRegister(BaseEffect *effect)
	{
		m_attack = effect->CreateControlInput("ADSR Envelope/Attack");
		m_decay = effect->CreateControlInput("ADSR Envelope/Decay");
		m_sustain = effect->CreateControlInput("ADSR Envelope/Sustain");
		m_release = effect->CreateControlInput("ADSR Envelope/Release");
	}

	void ADSREnvelopeComponent::OnUnregister(BaseEffect *effect)
	{
		effect->RemoveControlInput(m_attack);
		effect->RemoveControlInput(m_decay);
		effect->RemoveControlInput(m_sustain);
		effect->RemoveControlInput(m_release);

		m_attack = nullptr;
		m_decay = nullptr;
		m_sustain = nullptr;
		m_release = nullptr;
	}
}