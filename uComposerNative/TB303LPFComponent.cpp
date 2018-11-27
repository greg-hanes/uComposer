#include "TB303LPFComponent.h"
#include "BaseEffect.h"

namespace uComposer
{
	ControlInput *TB303LPFComponent::GetCutoffControl()
	{
		return m_cutoffControl;
	}

	ControlInput *TB303LPFComponent::GetResonanceControl()
	{
		return m_resonanceControl;
	}

	float TB303LPFComponent::Process(uint32_t sampleOffset, float input)
	{
		m_cutoffControl->Update(sampleOffset);
		
		m_filter.SetCutoff(m_cutoffControl->GetValue());
		m_filter.SetResonance(m_resonanceControl->GetValue());
		return m_filter.Process(input);
	}

	void TB303LPFComponent::OnRegister(BaseEffect *effect)
	{
		m_cutoffControl = effect->CreateControlInput("Filter/Cutoff Frequency");
		m_resonanceControl = effect->CreateControlInput("Filter/Resonance");
	}

	void TB303LPFComponent::OnUnregister(BaseEffect *effect)
	{
		effect->RemoveControlInput(m_cutoffControl);
		effect->RemoveControlInput(m_resonanceControl);

		m_cutoffControl = nullptr;
		m_resonanceControl = nullptr;
	}

}