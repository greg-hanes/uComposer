#pragma once

#include "SynthComponent.h"
#include "IEffect.h"

namespace uComposer
{
	class ADSREnvelopeComponent : public SynthComponent
	{
	public:
		ControlInput *GetAttackControl() const;
		ControlInput *GetDecayControl() const;
		ControlInput *GetSustainControl() const;
		ControlInput *GetReleaseControl() const;

		// Inherited via SynthComponent
		virtual void OnRegister(BaseEffect *) override;
		virtual void OnUnregister(BaseEffect *) override;

	private:
		ControlInput *m_attack;
		ControlInput *m_decay;
		ControlInput *m_sustain;
		ControlInput *m_release;
	};
}