#pragma once

#include "BaseEffect.h"
#include "TB303LPF.h"
#include "Synthesis.h"

namespace uComposer
{
	class TB303FilterEffect : public BaseEffect
	{
	public:
		virtual void Initialize() override;
		virtual void SetupProcessing() override;
		virtual void Process(ProcessData data, float dt) override;
	private:
		AudioInput *m_input;
		AudioOutput *m_output;
		Filters::TB303LPF m_filter;
		Oscillator::Sine m_osc;

		// Inherited via BaseEffect
	};

	inline BaseEffect *TB303FilterFactory()
	{
		return new TB303FilterEffect();
	}
}