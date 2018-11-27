#pragma once
#include <assert.h>
#include <algorithm>

namespace VecUtil
{
	template<typename T>
	int IndexOf(std::vector<T> &vec, T item)
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (item == vec.at(i))
				return static_cast<int>(i);
		}
		return -1;
	}

	template<typename T>
	void Insert(std::vector<T> &vec, T item, int index)
	{
		assert(index >= 0 && index <= vec.size());
		vec.insert(vec.begin() + index, item);
	}

	template<typename T>
	void Swap(std::vector<T> &vec, int oldIndex, int newIndex)
	{
		assert(oldIndex >= 0 && newIndex < vec.size());

		T tmp = vec.at(oldIndex);
		vec.at(oldIndex) = vec.at(newIndex);
		vec.at(newIndex) = tmp;
	}

	template<typename T>
	void RemoveAt(std::vector<T> &vec, size_t index)
	{
		assert(index >= 0 && index < vec.size());
		vec.erase(vec.begin() + index);
	}

	template<typename T>
	bool Remove(std::vector<T> &vec, T &item)
	{
		auto iter = std::remove(vec.begin(), vec.end(), item);
		if (iter != vec.end())
		{
			vec.erase(iter);
			return true;
		}
		return false;
	}

	// Attempts to remove and delete the given item from the given vector.
	//	If the item is found, it is deleted and returns true.
	//	If the item is not found, no change is made to the vector and returns false.
	//	Can only be used on vectors with a pointer type, and must not be used on vectors of array types.
	template<typename T>
	bool Delete(std::vector<T> &vec, T item)
	{
		auto iter = std::remove(vec.begin(), vec.end(), item);
		if (iter != vec.end())
		{
			vec.erase(iter);
			delete item;
			return true;
		}
		return false;
	}

	// OBSOLETE
	template<typename T>
	bool Erase(std::vector<T> &vec, T item)
	{
		auto iter = std::remove(vec.begin(), vec.end(), item);
		if (iter != vec.end())
		{
			vec.erase(iter);
			delete item;
			return true;
		}
		return false;
	}

	// Clears the given vector and deletes each element.
	//
	// Notes:
	//		T *must* be a dynamically allocated pointer type.
	//		T *must not* be an array.
	template<typename T>
	void Clear(std::vector<T> &vec)
	{
		for (auto item : vec)
		{
			delete item;
		}
		vec.clear();
	}
}