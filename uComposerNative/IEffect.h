#pragma once

#include <string>
#include <vector>

#include "Event.h"
#include "MathUtil.h"
#include "MessageQueue.h"
#include "EventMessage.h"
#include "ControlMessage.h"
#include "TrackTime.h"
#include "SynthComponent.h"

namespace uComposer
{
	typedef MessageQueue<EventMessage> EventQueue;
	typedef EventQueue *EventQueuePtr;

	typedef MessageQueue<ControlMessage> ControlQueue;
	typedef ControlQueue *ControlQueuePtr;

	struct ProcessData
	{
		uint32_t nSamples;
		TrackTime trackTime;
	};

	class BaseEffect;

	class BaseInput
	{
	public:
		BaseInput(BaseEffect *owner);
		BaseInput(BaseEffect *owner, std::string name);

		std::string Name() const;
		BaseEffect *Owner() const;
	private:
		BaseEffect *m_owner;
		std::string m_name;
	};

	class BaseOutput
	{
	public:
		BaseOutput(BaseEffect *owner);
		BaseOutput(BaseEffect *owner, std::string name);

		std::string Name() const;
		BaseEffect *Owner() const;

	private:
		BaseEffect *m_owner;
		std::string m_name;
	};

	class AudioOutput : public BaseOutput
	{
		friend class GraphPlayer;
		using BaseOutput::BaseOutput;

	public:
		float *const Buffer()
		{
			return m_buffer;
		}

	private:
		float *m_buffer;

	};

	class AudioInput : public BaseInput
	{
		friend class GraphPlayer;
		using BaseInput::BaseInput;

	public:
		float const * const Buffer()
		{
			return m_buffer;
		}
	private:
		float *m_buffer;
	};

	class ControlOutput : public BaseOutput
	{
		friend class GraphPlayer;
		using BaseOutput::BaseOutput;
	public:
		ControlQueue *ControlChanges()
		{
			return m_controlChanges;
		}
	private:
		ControlQueue *m_controlChanges;
	};

	class ControlInput : public BaseInput
	{
		friend class GraphPlayer;
		using BaseInput::BaseInput;
	public:
		ControlQueue *ControlChanges()
		{
			return m_controlChanges;
		}

		bool Update(uint32_t sampleOffset)
		{
			while (!m_controlChanges->Empty())
			{
				ControlMessage *msg = m_controlChanges->Peek();
				if (msg->mOffset > sampleOffset)
				{
					m_nextMessage = *msg;
					float t = m_timeSinceLastMessage / (float)(m_nextMessage.GetOffset() - m_lastMessage.GetOffset());
					t = Math::Bounded(t, 0.0f, 1.0f);
					m_currentValue = Math::Lerp(m_lastMessage.m_value, m_nextMessage.m_value, t);
					return true;
				}

				m_lastMessage = *msg;
				m_timeSinceLastMessage = 0;

				m_controlChanges->Remove();
			}
			return false;
		}

		void Flush(uint32_t nSamples)
		{
			m_controlChanges->Flush<true>(nSamples);
		}

		float GetValue() const
		{
			return m_currentValue;
		}

		void SetValue(float value)
		{
			m_currentValue = value;
		}
	private:
		ControlMessage m_lastMessage;
		ControlMessage m_nextMessage;
		uint32_t m_timeSinceLastMessage;
		float m_currentValue;

		ControlQueue *m_controlChanges;
	};

	class EventOutput : public BaseOutput
	{
		friend class GraphPlayer;
		using BaseOutput::BaseOutput;
	public:
		EventQueue *Events()
		{
			return m_events;
		}
	private:
		EventQueue *m_events;
	};

	class EventInput : public BaseInput
	{
		friend class GraphPlayer;
		using BaseInput::BaseInput;
	public:
		EventQueue *Events()
		{
			return m_events;
		}

		void Flush(uint32_t nSamples)
		{
			m_events->Flush<true>(nSamples);
		}
	private:
		EventQueue *m_events;
	};

	class EffectDescriptor;

	// TODO finish IEffect interface declaration.
	//	Play around with different BaseEffect impls or helper functions for processing
	//		events, control changes.
	class IEffect
	{
		friend class EffectGraph;
		friend class EffectDescriptor;

	public:
		virtual const EffectDescriptor *Type() const = 0;
		//std::string Name() const;
		//void SetName(std::string name);

		//virtual void Initialize() = 0;
		//virtual void SetupProcessing() = 0;
		//virtual void Process(ProcessData data, float dt) = 0;


		//const std::vector<BaseInput *> &AudioInputs() const;
		//const std::vector<BaseInput *> &ControlInputs() const;
		//const std::vector<BaseInput *> &EventInputs() const;

		//const std::vector<BaseOutput *> &AudioOutputs() const;
		//const std::vector<BaseOutput *> &ControlOutputs() const;
		//const std::vector<BaseOutput *> &EventOutputs() const;

		
		
	};
}