#pragma once

#include <cstdint>
#include <iostream>
#include "MemUtil.h"
#include "Event.h"

namespace uComposer
{
	enum SpeakerConfiguration
	{
		Mono,
		Stereo,
		Quad,
		Surround,
		Surround5point1,
		Surround7point1,
		ConfigurationCount
	};

	extern const int kChannelCount[6];
	extern const char *kSpeakerConfigurationNames[6];

	struct AudioChannel
	{
		float *data;
	};

	class AudioDeviceInterface
	{
	public:
		Event<int> SampleRateChanged;
		Event<int> BufferSizeChanged;
		Event<SpeakerConfiguration> SpeakerConfigurationChanged;

		inline AudioDeviceInterface(int sampleRate, int bufferSize, SpeakerConfiguration speakerConfiguration) :
			m_speakerConfiguration(speakerConfiguration),
			m_sampleRate(sampleRate),
			m_sampleDeltaTime(1.0 / sampleRate),
			m_bufferSize(bufferSize)
		{
			//std::cout << "Creating AudioInterface:" << std::endl;
			//std::cout << "\tSpeaker Configuration: " << kSpeakerConfigurationNames[speakerConfiguration] << std::endl;
			//std::cout << "\tOutput Channel Count: " << kChannelCount[speakerConfiguration] << std::endl;
			m_outputChannelCount = kChannelCount[speakerConfiguration];
		}

		inline void SetSpeakerConfiguration(SpeakerConfiguration speakerConfig)
		{
			m_speakerConfiguration = speakerConfig;
			SpeakerConfigurationChanged(speakerConfig);
		}

		inline int GetSampleRate() const
		{
			return m_sampleRate;
		}

		inline double GetSampleDeltaTime() const
		{
			return m_sampleDeltaTime;
		}

		inline void SetSampleRate(int sampleRate)
		{
			m_sampleRate = sampleRate;
			m_sampleDeltaTime = 1.0 / sampleRate;

			SampleRateChanged(sampleRate);
		}

		inline int GetBufferSize() const
		{
			return m_bufferSize;
		}

		inline void SetBufferSize(int bufferSize)
		{
			m_bufferSize = bufferSize;
			BufferSizeChanged(bufferSize);
		}

		inline int GetInputChannelCount() const
		{
			return m_inputChannelCount;
		}

		inline int GetOutputChannelCount() const
		{
			return m_outputChannelCount;
		}

	private:
		SpeakerConfiguration m_speakerConfiguration;
		int m_sampleRate;
		double m_sampleDeltaTime;
		int m_bufferSize;

		int m_inputChannelCount;
		int m_outputChannelCount;
	};
}