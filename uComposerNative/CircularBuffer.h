#pragma once

class CircularBuffer
{
public:
	CircularBuffer();

	void SetSize(int size);
	void Enqueue(float value);
	float Dequeue();

private:
	int m_startIndex;
	int m_endIndex;
	int m_size;
	float *m_bufferData;
};