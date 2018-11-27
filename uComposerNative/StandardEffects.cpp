#include "StandardEffects.h"

namespace uComposer
{
	namespace StandardEffects
	{
		const EffectDescriptor Track = EffectDescriptor("NoteTrack", "uComposer", 0, &TrackNodeFactory);
		const EffectDescriptor ControlTrack = EffectDescriptor("Control Track", "uComposer", 0, &ControlTrackSourceFactory);
		const EffectDescriptor AudioOutput = EffectDescriptor("Audio Output", "uComposer", 0, &OutputNodeFactory);
		const EffectDescriptor DelayLine = EffectDescriptor("Delay Line", "uComposer", 0, &DelayNodeFactory);
		const EffectDescriptor Mixer = EffectDescriptor("Mixer", "uComposer", 0, &MixerNodeFactory);
		const EffectDescriptor MonoSynth = EffectDescriptor("MonoSynth", "uComposer", 0, &MonoSynthFactory);
		const EffectDescriptor Sequencer = EffectDescriptor("Sequencer", "uComposer", 0, &SequencerNodeFactory);
		const EffectDescriptor DrumSynth = EffectDescriptor("DrumSynth", "uComposer", 0, &DrumSynthFactory);
		const EffectDescriptor TB303 = EffectDescriptor("TB-303", "uComposer", 0, &TB303SynthFactory);
		const EffectDescriptor ArpeggiatorEffect = EffectDescriptor("Arpeggiator", "uComposer", 0, &ArpeggiatorFactory);
		const EffectDescriptor TB303Filter = EffectDescriptor("TB303 Filter", "uComposer", 0, &TB303FilterFactory);
		const int EffectPluginCount = 10;

		DLL_EXTERN int GetEffectPluginCount()
		{
			return EffectPluginCount;
		}


		const EffectPackage *StandardEffects = new EffectPackage(
		{
			new EffectDescriptor[EffectPluginCount]
			{
				Track,
				ControlTrack,
				AudioOutput,
				DelayLine,
				Mixer,
				MonoSynth,
				Sequencer,
				DrumSynth,
				TB303,
				ArpeggiatorEffect
			}
		});
	}
}