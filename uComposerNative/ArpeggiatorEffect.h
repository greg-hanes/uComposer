#pragma once

#include "BaseEffect.h"
#include <vector>

namespace uComposer
{
	class ArpeggiatorEffect : public BaseEffect
	{
	public:
		using BaseEffect::BaseEffect;

		virtual void Initialize() override;
		virtual void Process(ProcessData data, float dt) override;

		float &Delay();
		std::vector<int> &NoteOffsets();


	private:
		EventInput *m_eventIn;
		EventOutput *m_eventOut;
		float m_delay;
		float m_duration;
		std::vector<int> m_noteOffsets;
	};

	inline BaseEffect *ArpeggiatorFactory()
	{
		return new ArpeggiatorEffect();
	}
}