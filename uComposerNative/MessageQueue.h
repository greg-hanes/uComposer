#pragma once

#include <stdlib.h>
#include <cstring>

template<class T>
class MessageQueue
{
public:
	MessageQueue(int size = 4096) : mBuf(NULL), mSize(0), mGrow(Granulize(size)), mFront(0), mBack(0) { Expand(); }
	~MessageQueue() { free(mBuf); }

	// Adds a MIDI message add the back of the queue. If the queue is full,
	// it will automatically expand itself.
	void Add(T pMsg)
	{
		if (mBack >= mSize)
		{
			if (mFront > 0)
				Compact();
			else if (!Expand()) return;
		}

#ifndef DONT_SORT_IMIDIQUEUE
		// Insert the MIDI message at the right offset.
		if (mBack > mFront && pMsg.GetOffset() < mBuf[mBack - 1].GetOffset())
		{
			int i = mBack - 2;
			while (i >= mFront && pMsg.GetOffset() < mBuf[i].GetOffset()) --i;
			i++;
			memmove(&mBuf[i + 1], &mBuf[i], (mBack - i) * sizeof(T));
			mBuf[i] = pMsg;
		}
		else
#endif
			mBuf[mBack] = pMsg;
		++mBack;
	}

	// Removes a MIDI message from the front of the queue (but does *not*
	// free up its space until Compact() is called).
	inline void Remove() { ++mFront; }

	// Returns true if the queue is empty.
	inline bool Empty() const { return mFront == mBack; }

	// Returns the number of MIDI messages in the queue.
	inline int ToDo() const { return mBack - mFront; }

	// Returns the number of MIDI messages for which memory has already been
	// allocated.
	inline int GetSize() const { return mSize; }

	// Returns the "next" MIDI message (all the way in the front of the
	// queue), but does *not* remove it from the queue.
	inline T* Peek() const { return &mBuf[mFront]; }

	// Moves back MIDI messages all the way to the front of the queue, thus
	// freeing up space at the back, and updates the sample offset of the
	// remaining MIDI messages by substracting nFrames.
	
	template<bool adjustOffsets>
	inline void Flush(int nFrames)
	{
		// Move everything all the way to the front.
		if (mFront > 0) Compact();

		// Update the sample offset.
		for (int i = 0; i < mBack; ++i) mBuf[i].mOffset -= nFrames;
	}

	template<>
	inline void Flush<true>(int nFrames)
	{
		if (mFront > 0) Compact();

		// Update the sample offset.
		for (int i = 0; i < mBack; ++i) mBuf[i].mOffset -= nFrames;
	}

	template<>
	inline void Flush<false>(int nFrames)
	{
		if (mFront > 0) Compact();
	}

	// Clears the queue.
	inline void Clear() { mFront = mBack = 0; }

	// Resizes (grows or shrinks) the queue, returns the new size.
	int Resize(int size)
	{
		if (mFront > 0) Compact();
		mGrow = size = Granulize(size);
		// Don't shrink below the number of currently queued MIDI messages.
		if (size < mBack) size = Granulize(mBack);
		if (size == mSize) return mSize;

		void* buf = realloc(mBuf, size * sizeof(T));
		if (!buf) return mSize;

		mBuf = (T*)buf;
		mSize = size;
		return size;
	}

protected:
	// Automatically expands the queue.
	bool Expand()
	{
		if (!mGrow) return false;
		int size = (mSize / mGrow + 1) * mGrow;

		void* buf = realloc(mBuf, size * sizeof(T));
		if (!buf) return false;

		mBuf = (T*)buf;
		mSize = size;
		return true;
	}

	// Moves everything all the way to the front.
	inline void Compact()
	{
		mBack -= mFront;
		if (mBack > 0) memmove(&mBuf[0], &mBuf[mFront], mBack * sizeof(T));
		mFront = 0;
	}

	// Rounds the MIDI queue size up to the next 4 kB memory page size.
	inline int Granulize(int size) const
	{
		int bytes = size * sizeof(T);
		int rest = bytes % 4096;
		if (rest) size = (bytes - rest + 4096) / sizeof(T);
		return size;
	}

	T* mBuf;

	int mSize, mGrow;
	int mFront, mBack;
};
