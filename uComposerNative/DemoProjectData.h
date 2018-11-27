#pragma once

#include <iostream>
#include "EffectGraph.h"
#include "NoteTrack.h"
#include "StandardEffects.h"
#include "PlaybackController.h"

using namespace uComposer;

namespace Demo
{
	void InitGraph(EffectGraph *graph, int sampleRate)
	{
		NoteTrackSource *seqNode1 = StandardEffects::Track.CreateInstance<NoteTrackSource>();
		ArpeggiatorEffect *arp = StandardEffects::ArpeggiatorEffect.CreateInstance<ArpeggiatorEffect>();
		BaseEffect *monoSynth0 = StandardEffects::MonoSynth.CreateInstance();
		DelayEffect *delay0 = StandardEffects::DelayLine.CreateInstance<DelayEffect>();
		DelayEffect *delay1 = StandardEffects::DelayLine.CreateInstance<DelayEffect>();
		ControlTrackSource *controlTrack = StandardEffects::ControlTrack.CreateInstance<ControlTrackSource>();
		TB303FilterEffect *filterEffect = StandardEffects::TB303Filter.CreateInstance<TB303FilterEffect>();
		TB303Synth *tb303Effect = StandardEffects::TB303.CreateInstance<TB303Synth>();

		graph->AddNode(delay1);
		graph->AddNode(monoSynth0);
		graph->AddNode(tb303Effect);
		graph->AddNode(seqNode1);
		graph->AddNode(arp);
		graph->AddNode(filterEffect);
		graph->AddNode(delay0);
		graph->AddNode(controlTrack);

		delay0->SetDelayAmount((int)(sampleRate * 0.05f));
		delay0->SetFeedback(0.95f);
		delay0->SetDryMixAmount(0.5f);
		delay0->SetWetMixAmount(0.35f);

		delay1->SetDelayAmount((int)(sampleRate * 0.07f));
		delay1->SetFeedback(0.95f);
		delay1->SetDryMixAmount(0.5f);
		delay1->SetWetMixAmount(0.35f);
		
		arp->Delay() = 0.05f;
		//arp->NoteOffsets().push_back(2);
		//arp->NoteOffsets().push_back(3);
		//arp->NoteOffsets().push_back(5);
		//arp->NoteOffsets().push_back(2);

		//graph->CreateConnection(seqNode1->EventOutputs()[0], arp->EventInputs()[0]);
		//graph->CreateConnection(arp->EventOutputs()[0], monoSynth0->EventInputs()[0]);

		graph->CreateConnection(seqNode1->EventOutputs()[0], tb303Effect->EventInputs()[0]);

		// Use delay:

		//graph->CreateConnection(delay0->AudioOutputs()[0], graph->GetOutputNode()->AudioInputs()[0]);
		//graph->CreateConnection(delay1->AudioOutputs()[0], graph->GetOutputNode()->AudioInputs()[1]);

		graph->CreateConnection(tb303Effect->AudioOutputs()[0], graph->GetOutputNode()->AudioInputs()[0]);
		graph->CreateConnection(tb303Effect->AudioOutputs()[0], graph->GetOutputNode()->AudioInputs()[1]);

		//graph->CreateConnection(monoSynth0->AudioOutputs()[0], filterEffect->AudioInputs()[0]);

		//graph->CreateConnection(filterEffect->AudioOutputs()[0], delay0->AudioInputs()[0]);
		//graph->CreateConnection(filterEffect->AudioOutputs()[0], delay1->AudioInputs()[0]);


		//graph->CreateConnection(controlTrack->ControlOutputs()[0], monoSynth0->ControlInputs()[0]);
		// Bypass delay:
		//graph->CreateConnection(monoSynth0->AudioOutputs()[0], graph->GetOutputNode()->AudioInputs()[0]);
		//graph->CreateConnection(monoSynth0->AudioOutputs()[0], graph->GetOutputNode()->AudioInputs()[1]);
	}

	PlaybackController *Initialize2(int sampleRate, SpeakerConfiguration speakerConfiguration, int bufferSize)
	{
		AudioDeviceInterface *device = new AudioDeviceInterface(sampleRate, bufferSize, speakerConfiguration);

		ProjectData *pd = new ProjectData();

		double bpm = 180;
		pd->GetTempo()->SetBPM(bpm);
		InitGraph(pd->GetEffectGraph(), sampleRate);


		Block *b1;
		{
			// Block one initialization
			
			// Create a new block, set it to loop.
			b1 = pd->GetBlocks()->CreateBlock();
			b1->SetLoops(true);

			// Set the length of the block in beats.
			b1->SetLength(TrackTime(12, 0));

			// Create a new track on the block.
			NoteTrack *t1 = b1->CreateNoteTrack("NoteTrack 1");
			b1->GetTempoControlTrack().AddControlPoint(TempoControlPoint(80, TrackTime(0, 0)));
			b1->GetTempoControlTrack().AddControlPoint(TempoControlPoint(140, TrackTime(8, 0)));
			b1->GetTempoControlTrack().AddControlPoint(TempoControlPoint(80, TrackTime(12, 0)));
			b1->SetOverridesTempo(true);

			// Send the track notes to the effect graph's track node.
			t1->SetTrackNode(pd->GetEffectGraph()->TrackNodes()[0]);

			// Create a new note lane and add notes.
			NoteLane *t1NoteLane = t1->CreateNoteLane();

			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(12, 0), Note(Keys::C3_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(12, 0), Note(Keys::C4_Sharp, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(1, 0), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(1, 0), TrackTime(1, 0), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(2, 0), TrackTime(1, 0), Note(Keys::E5, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(3, 0), TrackTime(1, 0), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(4, 0), TrackTime(1, 0), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(5, 0), TrackTime(1, 0), Note(Keys::E5, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(6, 0), TrackTime(1, 0), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(7, 0), TrackTime(1, 0), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(8, 0), TrackTime(1, 0), Note(Keys::E5, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(9, 0), TrackTime(1, 0), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(10, 0), TrackTime(1, 0), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(11, 0), TrackTime(1, 0), Note(Keys::E5, 1.0f)));


			//ControlTrack *c1 = b1->CreateParameterTrack("Gain");
			//c1->SetTrackNode(pd->GetEffectGraph()->ParameterTrackNodes()[0]);
			//AutomationLane &lane = c1->GetAutomationLane();

			//lane.Insert(AutomationKey(0.0f, TrackTime(0, 0)));
			//lane.Insert(AutomationKey(1.0f, TrackTime(12, 0)));
		}


		Block *b2;
		{
			// Block 2 initializtion

			// Create new block and set it to loop and set the length.
			b2 = pd->GetBlocks()->CreateBlock();
			b2->SetLoops(true);
			b2->SetLength(TrackTime(12, 0));

			NoteTrack *t1 = b2->CreateNoteTrack("NoteTrack 1");
			b2->GetTempoControlTrack().AddControlPoint(TempoControlPoint(120, TrackTime(0, 0)));
			b2->GetTempoControlTrack().AddControlPoint(TempoControlPoint(160, TrackTime(8, 0)));
			b2->GetTempoControlTrack().AddControlPoint(TempoControlPoint(100, TrackTime(12, 0)));
			b2->SetOverridesTempo(true);

			t1->SetTrackNode(pd->GetEffectGraph()->TrackNodes()[0]);

			
			NoteLane *t1NoteLane = t1->CreateNoteLane();

			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(12, 0), Note(Keys::B2, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(12, 0), Note(Keys::B3, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(1, 0), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(1, 0), TrackTime(1, 0), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(2, 0), TrackTime(1, 0), Note(Keys::E5, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(3, 0), TrackTime(1, 0), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(4, 0), TrackTime(1, 0), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(5, 0), TrackTime(1, 0), Note(Keys::E5, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(6, 0), TrackTime(1, 0), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(7, 0), TrackTime(1, 0), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(8, 0), TrackTime(1, 0), Note(Keys::E5, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(9, 0), TrackTime(1, 0), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(10, 0), TrackTime(1, 0), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(11, 0), TrackTime(1, 0), Note(Keys::E5, 1.0f)));
		}

		Block *b3;
		{
			b3 = pd->GetBlocks()->CreateBlock();
			b3->SetLoops(true);
			b3->SetLength(TrackTime(12, 0));
			b3->GetTempoControlTrack().AddControlPoint(TempoControlPoint(120, TrackTime(0, 0)));
			b3->GetTempoControlTrack().AddControlPoint(TempoControlPoint(160, TrackTime(8, 0)));
			b3->GetTempoControlTrack().AddControlPoint(TempoControlPoint(80, TrackTime(12, 0)));
			b3->SetOverridesTempo(true);

			NoteTrack *t1 = b3->CreateNoteTrack("NoteTrack 1");
			t1->SetTrackNode(pd->GetEffectGraph()->TrackNodes()[0]);

			NoteLane *t1NoteLane = t1->CreateNoteLane();

			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(6, 0), Note(Keys::A2, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(6, 0), Note(Keys::A3, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(6, 0), TrackTime(6, 0), Note(Keys::F2_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(6, 0), TrackTime(6, 0), Note(Keys::F3_Sharp, 1.0f)));


			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(0, 2), Note(Keys::A4, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(1, 0), TrackTime(0, 2), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(2, 0), TrackTime(0, 2), Note(Keys::E5, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(3, 0), TrackTime(0, 2), Note(Keys::A4, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(4, 0), TrackTime(0, 2), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(5, 0), TrackTime(0, 2), Note(Keys::E5, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(6, 0), TrackTime(0, 2), Note(Keys::A4, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(7, 0), TrackTime(0, 2), Note(Keys::D5, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(8, 0), TrackTime(0, 2), Note(Keys::F5_Sharp, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(9, 0), TrackTime(0, 2), Note(Keys::A4, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(10, 0), TrackTime(0, 2), Note(Keys::D5, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(11, 0), TrackTime(0, 2), Note(Keys::F5_Sharp, 1.0f)));
		}

		Block *b4;
		{
			b4 = pd->GetBlocks()->CreateBlock();
			b4->SetLoops(true);
			b4->SetLength(TrackTime(12, 0));
			b4->GetTempoControlTrack().AddControlPoint(TempoControlPoint(120, TrackTime(0, 0)));
			b4->GetTempoControlTrack().AddControlPoint(TempoControlPoint(160, TrackTime(8, 0)));
			b4->GetTempoControlTrack().AddControlPoint(TempoControlPoint(80, TrackTime(12, 0)));
			b4->SetOverridesTempo(true);

			NoteTrack *t1 = b4->CreateNoteTrack("NoteTrack 1");
			t1->SetTrackNode(pd->GetEffectGraph()->TrackNodes()[0]);

			NoteLane *t1NoteLane = t1->CreateNoteLane();

			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(6, 0), Note(Keys::G2_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(6, 0), Note(Keys::G3_Sharp, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(6, 0), TrackTime(6, 0), Note(Keys::G2_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(6, 0), TrackTime(6, 0), Note(Keys::G3_Sharp, 1.0f)));


			t1NoteLane->Insert(TrackNote(TrackTime(0, 0), TrackTime(0, 2), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(1, 0), TrackTime(0, 2), Note(Keys::C5, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(2, 0), TrackTime(0, 2), Note(Keys::F5_Sharp, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(3, 0), TrackTime(0, 2), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(4, 0), TrackTime(0, 2), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(5, 0), TrackTime(0, 2), Note(Keys::E5, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(6, 0), TrackTime(0, 2), Note(Keys::G4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(7, 0), TrackTime(0, 2), Note(Keys::C5_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(8, 0), TrackTime(0, 2), Note(Keys::D5_Sharp, 1.0f)));

			t1NoteLane->Insert(TrackNote(TrackTime(9, 0), TrackTime(0, 2), Note(Keys::F4_Sharp, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(10, 0), TrackTime(0, 2), Note(Keys::C5, 1.0f)));
			t1NoteLane->Insert(TrackNote(TrackTime(11, 0), TrackTime(0, 2), Note(Keys::D5_Sharp, 1.0f)));
		}

		PlaybackController *pbc = new PlaybackController(device, pd);

		BlockTransition *defaultToB1 = new BlockTransition(b1);
		defaultToB1->SetPlayCount(1);
		defaultToB1->Set(true);
		pd->GetBlocks()->GetDefaultBlock()->AddTransition(defaultToB1);

		BlockTransition *b1ToB2 = new BlockTransition(b2);
		b1ToB2->SetPlayCount(1);
		b1ToB2->Set(true);
		b1->AddTransition(b1ToB2);

		BlockTransition *b2ToB3 = new BlockTransition(b3);
		b2ToB3->SetPlayCount(1);
		b2ToB3->Set(true);
		b2->AddTransition(b2ToB3);

		BlockTransition *b3ToB4 = new BlockTransition(b4);
		b3ToB4->SetPlayCount(1);
		b3ToB4->Set(true);
		b3->AddTransition(b3ToB4);

		BlockTransition *b4ToB1 = new BlockTransition(b1);
		b4ToB1->SetPlayCount(1);
		b4ToB1->Set(true);
		b4->AddTransition(b4ToB1);

		

		return pbc;
	}
}