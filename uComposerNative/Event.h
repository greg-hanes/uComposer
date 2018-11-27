#pragma once
#include "FastDelegate.h"
#include <vector>
#include <algorithm>

template<class... T>
class Event
{
public:
	typedef void(*Callback)(T... arg);

	void Invoke(T... args)
	{
		for (unsigned int i = 0; i < m_things.size(); i++)
		{
			m_things.at(i)(args...);
		}
	}

	void Register(Callback callback)
	{
		operator+=(callback);
	}

	void Unregister(Callback callback)
	{
		operator-=(callback);
	}

	template<typename TTarget>
	void Register(TTarget *target, void (TTarget::*fptr)(T... args))
	{
		Thing t = Thing();
		t.bind(target, fptr);
		m_things.push_back(t);
	}

	template<typename TTarget>
	void Unregister(TTarget *target, void (TTarget::*fptr)(T... args))
	{
		Thing t = Thing();
		t.bind(target, fptr);
		m_things.erase(std::remove(m_things.begin(), m_things.end(), t));
	}

	void operator()(T... args)
	{
		Invoke(args...);
	}

	void operator+=(const Callback& rhs)
	{
		m_things.push_back(rhs);
	}

	void operator-=(const Callback &rhs)
	{
		m_things.erase(std::remove(m_things.begin(), m_things.end(), rhs));
	}

private:
	typedef fastdelegate::FastDelegateN<T...> Thing;
	std::vector<Thing> m_things;
};

