#pragma once

#include <cstdint>
#include <cmath>

namespace uComposer
{
	typedef uint8_t Count;

	enum Quantum : uint8_t
	{
		One = 1,
		Two = 2,
		Four = 4,
		Eight = 8,
		Sixteen = 16,
		ThirtyTwo = 32,
	};

	struct TimeSignature
	{
		Count count;
		Quantum quantum;
	};
}