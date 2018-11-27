#pragma once

#include "BaseEffect.h"
#include "Tempo.h"
#include <vector>
#include <assert.h>


namespace uComposer
{
	class SequencerEffect : public BaseEffect
	{
	public:
		using BaseEffect::BaseEffect;

		// Inherited via BaseEffect
		virtual void Initialize() override;
		virtual void Process(ProcessData data, float dt) override;

		std::vector<EventMessage> &GetSequencerMessages();
		int GetSequenceLength() const;
		void SetSequenceLength(int);
	private:
		EventOutput *m_eventOut;

		int m_sequencerLength;
		int m_sampleOffset = 0;
		int m_currentIndex = 0;
		std::vector<EventMessage> m_eventMessages;
	};

	inline BaseEffect *SequencerNodeFactory()
	{
		return new SequencerEffect();
	}
}