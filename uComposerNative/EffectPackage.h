#pragma once

#include "EffectDescriptor.h"

namespace uComposer
{
	// An EffectPackage is a set of EffectDescriptors.
	class EffectPackage
	{
	public:
		EffectPackage(const EffectDescriptor *effectDescriptors) : m_effectDescriptors(effectDescriptors)
		{

		}

		int EffectCount() const
		{
			return m_effectCount;
		}

		EffectDescriptor GetEffectDescriptor(int index) const
		{
			return m_effectDescriptors[index];
		}
	private:
		int m_effectCount;
		const EffectDescriptor *m_effectDescriptors;
	};
}