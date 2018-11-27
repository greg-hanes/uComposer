#pragma once

#include <string>
#include "BaseEffect.h"

namespace uComposer
{
	typedef BaseEffect*(*EffectConstructor)();

	class EffectDescriptor
	{
	public:
		EffectDescriptor(const char *effectName, const char *effectAuthor, int effectVersion, EffectConstructor constructor) :
			m_effectName(effectName),
			m_effectAuthor(effectAuthor),
			m_effectVersion(effectVersion),
			m_effectConstructor(constructor)
		{

		}

		const char *EffectName() const
		{
			return m_effectName;
		}

		const char *EffectAuthor() const
		{
			return m_effectAuthor;
		}

		int EffectVersion() const
		{
			return m_effectVersion;
		}

		BaseEffect *CreateInstance() const
		{
			BaseEffect *node = m_effectConstructor();
			node->m_nodeType = this;
			return node;
		}

		template<typename T>
		T *CreateInstance() const
		{
			return static_cast<T*>(CreateInstance());
		}

		friend bool operator==(const EffectDescriptor& lhs, const EffectDescriptor &rhs)
		{
			return lhs.m_effectConstructor == rhs.m_effectConstructor;
		}

		friend bool operator!=(const EffectDescriptor& lhs, const EffectDescriptor &rhs)
		{
			return !(lhs == rhs);
		}
	private:
		const char *m_effectName;
		const char *m_effectAuthor;
		int m_effectVersion;
		EffectConstructor m_effectConstructor;
	};
}