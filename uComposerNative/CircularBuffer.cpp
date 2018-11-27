#include "CircularBuffer.h"
#include "MemUtil.h"
#include "MathUtil.h"

CircularBuffer::CircularBuffer() :
	m_size(4096),
	m_startIndex(0),
	m_endIndex(4095),
	m_bufferData(uComposer::Memory::CreateArray(m_size))
{
}

void CircularBuffer::SetSize(int size)
{
	if (size > m_size)
	{
		// Expand array
		float *tmp = m_bufferData;

		// Create new array
		m_bufferData = uComposer::Memory::CreateArray(size);

		// Copy old contents into it.
		memcpy(tmp, m_bufferData, m_size);

		// Delete old array
		delete[] tmp;
	}

	m_size = size;
	// If size decreased, make sure new size is within bounds.
	m_startIndex = m_startIndex % m_size;

	// Make sure endIndex correctly wraps.
	m_endIndex = m_startIndex + m_size;
	m_endIndex = m_endIndex % m_size;
}

void CircularBuffer::Enqueue(float value)
{
	m_bufferData[m_endIndex] = value;
	m_endIndex++;
	m_endIndex = m_endIndex % m_size;
}

float CircularBuffer::Dequeue()
{
	float value = m_bufferData[m_startIndex];

	m_startIndex++;
	m_startIndex = m_startIndex % m_size;

	return value;
}