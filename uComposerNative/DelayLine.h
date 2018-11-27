#pragma once
#include <cstdint>
namespace uComposer
{
	class DelayLine
	{
	public:
		void SetDelay(float seconds, float sampleRate)
		{
			float samples = sampleRate * seconds;

			
		}
	private:
		uint32_t m_bufferSize;
		float *m_buffer;
	};
}