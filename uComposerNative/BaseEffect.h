#pragma once

#include "IEffect.h"
#include <cstdint>

namespace uComposer
{
	class BaseEffect
	{
		friend class EffectGraph;
		friend class EffectDescriptor;
	public:
		BaseEffect();
		virtual ~BaseEffect();

		virtual const EffectDescriptor *Type() const;
		std::string Name() const;
		void SetName(std::string name);


		const std::vector<AudioInput *> &AudioInputs() const;
		const std::vector<ControlInput *> &ControlInputs() const;
		const std::vector<EventInput *> &EventInputs() const;

		const std::vector<AudioOutput *> &AudioOutputs() const;
		const std::vector<ControlOutput *> &ControlOutputs() const;
		const std::vector<EventOutput *> &EventOutputs() const;


		virtual void Initialize() {}
		virtual void SetupProcessing() {}
		virtual void Process(ProcessData data, float dt) = 0;

		void FlushQueues(ProcessData data);

		uint32_t SampleRate() const;
		void SetSampleRate(uint32_t sampleRate);

	
		void RegisterComponent(SynthComponent *component);
		void UnregisterComponent(SynthComponent *component);

		AudioInput *CreateAudioInput();
		AudioInput *CreateAudioInput(std::string name);
		AudioInput *InsertAudioInput(std::string name, int index);
		void RemoveAudioInput(int index);

		AudioOutput *CreateAudioOutput();
		AudioOutput *CreateAudioOutput(std::string name);
		AudioOutput *InsertAudioOutput(std::string name, int index);
		void RemoveAudioOutput(int index);

		// No insert?
		ControlInput *CreateControlInput();
		ControlInput *CreateControlInput(std::string name);
		void RemoveControlInput(int index);
		void RemoveControlInput(ControlInput *input);

		ControlOutput *CreateControlOutput();
		ControlOutput *CreateControlOutput(std::string name);
		void RemoveControlOutput(int index);

		EventInput *CreateEventInput();
		EventInput *CreateEventInput(std::string name);
		void RemoveEventInput(int index);

		EventOutput *CreateEventOutput();
		EventOutput *CreateEventOutput(std::string name);
		void RemoveEventOutput(int index);

	protected:
		virtual void OnInputConnected(BaseInput *input) {}
		virtual void OnInputDisconnected(BaseInput *input) {}
		virtual void OnOutputConnected(BaseOutput *output) {}
		virtual void OnOutputDisconnected(BaseOutput *output) {}

		virtual void OnSampleRateChanged() {};

	private:
		std::string m_name;
		const EffectDescriptor *m_nodeType;

		uint32_t m_sampleRate;

		Event<BaseEffect *, BaseInput *> m_inputAddedEvent;
		Event<BaseEffect *, BaseInput *> m_inputRemovedEvent;

		Event<BaseEffect *, BaseOutput *> m_outputAddedEvent;
		Event<BaseEffect *, BaseOutput *> m_outputRemovedEvent;

		std::vector<AudioInput *> m_audioInputs;
		std::vector<AudioOutput *> m_audioOutputs;

		std::vector<ControlInput *> m_controlInputs;
		std::vector<ControlOutput *> m_controlOutputs;

		std::vector<EventInput *> m_eventInputs;
		std::vector<EventOutput *> m_eventOutputs;
	};
}