#pragma once

#include <vector>
#include "uComposer.h"
#include "EffectList.h"
#include "EffectPackage.h"

namespace uComposer
{
	namespace StandardEffects
	{
		extern const EffectDescriptor Track;
		extern const EffectDescriptor ControlTrack;
		extern const EffectDescriptor AudioOutput;
		extern const EffectDescriptor DelayLine;
		extern const EffectDescriptor Mixer;
		extern const EffectDescriptor MonoSynth;
		extern const EffectDescriptor Sequencer;
		extern const EffectDescriptor DrumSynth;
		extern const EffectDescriptor TB303;
		extern const EffectDescriptor ArpeggiatorEffect;
		extern const EffectDescriptor TB303Filter;

		extern const int EffectPluginCount;
		extern const EffectPackage *StandardEffects;
		
		DLL_EXTERN int GetEffectPluginCount();

	}
}