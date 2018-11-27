#pragma once

#include "BaseEffect.h"

namespace uComposer
{
	class OutputNode : public BaseEffect
	{
	public:
		using BaseEffect::BaseEffect;
		void SetOutputChannels(int channels);
		void SetOutputBuffer(float *buffer);

		// Inherited via BaseEffect
		virtual void Initialize() override;
		virtual void Process(ProcessData data, float dt) override;

	private:
		AudioInput* m_inputs[6];
		int m_outputChannels;
		float *m_outputBuffer;
	};

	inline BaseEffect *OutputNodeFactory()
	{
		return new OutputNode();
	}
}