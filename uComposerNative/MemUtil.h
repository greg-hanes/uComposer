#pragma once

#include <algorithm>
#include <cstdint>

namespace uComposer
{
	namespace Memory
	{
		// Allocates a contiguous 2d array. The array is non-zeroed.
		inline float** Create2DArray(size_t nrows, size_t ncols)
		{
			float** ptr = new float*[nrows];  // allocate pointers
			float* pool = new float[nrows*ncols];  // allocate pool
			for (size_t i = 0; i < nrows; ++i, pool += ncols)
				ptr[i] = pool;
			return ptr;
		}

		// Deletes a contiguous 2d array. The given float** pointer *must* have been allocated via
		//	Create2DArray
		inline void Delete2DArray(float** arr)
		{
			delete[] arr[0];  // remove the pool
			delete[] arr;     // remove the pointers
		}

		// Zeroes the given array.
		inline void Zero(float *arr, uint32_t size)
		{
			std::fill(arr, arr + size, 0.0f);
		}

		// Creates a zeroed array.
		inline float *CreateArray(uint32_t size)
		{
			float *arr = new float[size];
			Zero(arr, size);
			return arr;
		}
	}
}