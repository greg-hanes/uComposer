#include "BaseEffect.h"
#include "EffectGraph.h"
#include "VecUtil.h"
#include <assert.h>

namespace uComposer
{
	BaseInput::BaseInput(BaseEffect *owner) :
		m_owner(owner),
		m_name("")
	{

	}

	BaseInput::BaseInput(BaseEffect *owner, std::string name) :
		m_owner(owner),
		m_name(name)
	{

	}

	std::string BaseInput::Name() const
	{
		return m_name;
	}

	BaseEffect *BaseInput::Owner() const
	{
		return m_owner;
	}

	BaseOutput::BaseOutput(BaseEffect *owner) :
		m_owner(owner),
		m_name("")
	{

	}

	BaseOutput::BaseOutput(BaseEffect *owner, std::string name) :
		m_owner(owner),
		m_name(name)
	{

	}

	std::string BaseOutput::Name() const
	{
		return m_name;
	}

	BaseEffect *BaseOutput::Owner() const
	{
		return m_owner;
	}

	////////////////////////////////////////////////////
	// BaseEffect Definition
	////////////////////////////////////////////////////
#pragma region BaseEffect Implementation

	BaseEffect::BaseEffect()
	{
	}

	BaseEffect::~BaseEffect()
	{
		VecUtil::Clear(m_audioInputs);
		VecUtil::Clear(m_audioOutputs);
		VecUtil::Clear(m_controlInputs);
		VecUtil::Clear(m_controlOutputs);
		VecUtil::Clear(m_eventInputs);
		VecUtil::Clear(m_eventOutputs);
	}

	const EffectDescriptor *BaseEffect::Type() const
	{
		return m_nodeType;
	}

	std::string BaseEffect::Name() const
	{
		return m_name;
	}

	void BaseEffect::SetName(std::string name)
	{
		m_name = name;
	}

	void BaseEffect::FlushQueues(ProcessData data)
	{
		for (auto eventInput : m_eventInputs)
		{
			eventInput->Flush(data.nSamples);
		}

		for (auto controlInput : m_controlInputs)
		{
			controlInput->Flush(data.nSamples);
		}
	}

	uint32_t BaseEffect::SampleRate() const
	{
		return m_sampleRate;
	}

	void BaseEffect::SetSampleRate(uint32_t sampleRate)
	{
		m_sampleRate = sampleRate;
		OnSampleRateChanged();
	}

	void BaseEffect::RegisterComponent(SynthComponent *component)
	{
		component->OnRegister(this);
	}

	void BaseEffect::UnregisterComponent(SynthComponent *component)
	{
		component->OnUnregister(this);
	}

	AudioInput *BaseEffect::CreateAudioInput()
	{
		return CreateAudioInput("");
	}

	AudioInput *BaseEffect::CreateAudioInput(std::string name)
	{
		AudioInput *nodeInput = new AudioInput(this, name);
		m_audioInputs.push_back(nodeInput);
		m_inputAddedEvent(this, nodeInput);
		return nodeInput;
	}

	AudioInput *BaseEffect::InsertAudioInput(std::string name, int index)
	{
		AudioInput *nodeInput = new AudioInput(this, name);
		VecUtil::Insert(m_audioInputs, nodeInput, index);
		m_inputAddedEvent(this, nodeInput);
		return nodeInput;
	}

	void BaseEffect::RemoveAudioInput(int index)
	{
		assert(index >= 0 && index < m_audioInputs.size());

		BaseInput *toRemove = m_audioInputs.at(index);
		VecUtil::RemoveAt(m_audioInputs, index);

		m_inputRemovedEvent(this, toRemove);

		delete toRemove;
	}

	AudioOutput *BaseEffect::CreateAudioOutput()
	{
		return CreateAudioOutput("");
	}

	AudioOutput * BaseEffect::CreateAudioOutput(std::string name)
	{
		AudioOutput *nodeOutput = new AudioOutput(this, name);
		m_audioOutputs.push_back(nodeOutput);
		m_outputAddedEvent(this, nodeOutput);
		return nodeOutput;
	}

	AudioOutput * BaseEffect::InsertAudioOutput(std::string name, int index)
	{
		AudioOutput *nodeOutput = new AudioOutput(this, name);
		VecUtil::Insert(m_audioOutputs, nodeOutput, index);
		m_outputAddedEvent(this, nodeOutput);
		return nodeOutput;
	}

	void BaseEffect::RemoveAudioOutput(int index)
	{
		assert(index >= 0 && index < m_audioOutputs.size());

		AudioOutput *toRemove = m_audioOutputs.at(index);
		VecUtil::RemoveAt(m_audioOutputs, index);
		m_outputRemovedEvent(this, toRemove);
		delete toRemove;
	}


	ControlInput *BaseEffect::CreateControlInput()
	{
		return CreateControlInput("");
	}

	ControlInput *BaseEffect::CreateControlInput(std::string name)
	{
		ControlInput *controlInput = new ControlInput(this, name);
		m_controlInputs.push_back(controlInput);
		return controlInput;
	}

	void BaseEffect::RemoveControlInput(int index)
	{
		assert(index >= 0 && index < m_controlInputs.size());

		ControlInput *toRemove = m_controlInputs.at(index);
		VecUtil::RemoveAt(m_controlInputs, index);

		// TODO event?

		delete toRemove;
	}

	void BaseEffect::RemoveControlInput(ControlInput *input)
	{
		VecUtil::Delete(m_controlInputs, input);
	}

	ControlOutput * BaseEffect::CreateControlOutput()
	{
		return CreateControlOutput("");
	}

	ControlOutput * BaseEffect::CreateControlOutput(std::string name)
	{
		ControlOutput *controlOutput = new ControlOutput(this, name);
		m_controlOutputs.push_back(controlOutput);
		return controlOutput;
	}

	void BaseEffect::RemoveControlOutput(int index)
	{
		assert(index >= 0 && index < m_controlOutputs.size());

		ControlOutput *toRemove = m_controlOutputs.at(index);
		VecUtil::RemoveAt(m_controlOutputs, index);

		// TODO event?

		delete toRemove;
	}

	EventInput * BaseEffect::CreateEventInput()
	{
		return CreateEventInput("");
	}

	EventInput * BaseEffect::CreateEventInput(std::string name)
	{
		EventInput *eventInput = new EventInput(this, name);
		m_eventInputs.push_back(eventInput);
		return eventInput;
	}

	void BaseEffect::RemoveEventInput(int index)
	{
		assert(index >= 0 && index < m_eventInputs.size());

		EventInput *toRemove = m_eventInputs.at(index);
		VecUtil::RemoveAt(m_eventInputs, index);

		// TODO event?

		delete toRemove;
	}

	EventOutput *BaseEffect::CreateEventOutput()
	{
		return CreateEventOutput("");
	}

	EventOutput *BaseEffect::CreateEventOutput(std::string name)
	{
		EventOutput *eventOutput = new EventOutput(this, name);
		m_eventOutputs.push_back(eventOutput);
		return eventOutput;
	}

	void BaseEffect::RemoveEventOutput(int index)
	{
		assert(index >= 0 && index < m_eventOutputs.size());

		EventOutput *toRemove = m_eventOutputs.at(index);
		VecUtil::RemoveAt(m_eventOutputs, index);

		// TODO event?

		delete toRemove;
	}

	const std::vector<AudioInput *> &BaseEffect::AudioInputs() const
	{
		return m_audioInputs;
	}

	const std::vector<AudioOutput *> &BaseEffect::AudioOutputs() const
	{
		return m_audioOutputs;
	}

	const std::vector<ControlInput *> &BaseEffect::ControlInputs() const
	{
		return m_controlInputs;
	}

	const std::vector<ControlOutput *> &BaseEffect::ControlOutputs() const
	{
		return m_controlOutputs;
	}

	const std::vector<EventInput *>& BaseEffect::EventInputs() const
	{
		return m_eventInputs;
	}

	const std::vector<EventOutput *>& BaseEffect::EventOutputs() const
	{
		return m_eventOutputs;
	}

#pragma endregion
}