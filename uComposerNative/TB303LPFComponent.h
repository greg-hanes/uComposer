#pragma once

#include "SynthComponent.h"
#include "TB303LPF.h"

namespace uComposer
{
	class TB303LPFComponent : public SynthComponent
	{
	public:
		float Process(uint32_t sampleOffset, float input);

		ControlInput *GetCutoffControl();
		ControlInput *GetResonanceControl();

		// Inherited via SynthComponent
		virtual void OnRegister(BaseEffect *) override;
		virtual void OnUnregister(BaseEffect *) override;

	private:
		Filters::TB303LPF m_filter;
		ControlInput *m_cutoffControl;
		ControlInput *m_resonanceControl;
	};
}