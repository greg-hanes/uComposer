#include <assert.h>
#include <iostream>
#include <vector>

#include "uComposer.h"
#include "MathUtil.h"
#include "Oscillator.h"
#include "VecUtil.h"
#include "AudioDeviceInterface.h"
#include "EffectList.h"
#include "PlaybackController.h"
#include "GraphPlayer.h"

#include "DemoProjectData.h"

using namespace uComposer;

PlaybackController *playbackController;

DLL_EXTERN float *GetBuffer(int size)
{
	return uComposer::Memory::CreateArray(size);
}

DLL_EXTERN void ReleaseBuffer(float *buffer)
{
	delete[] buffer;
}

DLL_EXTERN void Process(float *buffer, int nsamples, int channelCount)
{
	playbackController->Process(buffer);
}

DLL_EXTERN int GetTrackNodeCount()
{
	return (int)playbackController->GetGraphController()->GetEffectGraph()->TrackNodes().size();
}

DLL_EXTERN void SendMessage(int trackNodeIndex, EventType type, Note note)
{
	std::cout << "Received Message: " << trackNodeIndex << ", " << type << ", " << (int)note.GetKey() << ", " << note.GetTuning() << ", " << note.GetVelocity() << std::endl;
	const std::vector<NoteTrackSource *> &trackNodes = playbackController->GetGraphController()->GetEffectGraph()->TrackNodes();
	EventMessage message;
	message.mOffset = 0;
	message.note = note;
	message.type = type;
	trackNodes.at(trackNodeIndex)->SendMessage(message);
}

DLL_EXTERN void InitializeScratchpad(int sampleRate, SpeakerConfiguration speakerConfiguration, int bufferSize)
{
	std::cout << "Initializing - Sample Rate: " << sampleRate << ", Buffer Size: " << bufferSize << std::endl;
	std::cout << "For track playback, Play: Ctrl+Space, Stop: Ctrl+S, Pause: Ctrl+P" << std::endl;
	std::cout << "For keyboard input, white keys on Q and Z rows; black keys starting at 2 and S rows." << std::endl;
	playbackController = Demo::Initialize2(sampleRate, speakerConfiguration, bufferSize);
}

DLL_EXTERN void Play()
{
	std::cout << "Playing" << std::endl;
	playbackController->Play();
}

DLL_EXTERN void Pause()
{
	std::cout << "Paused" << std::endl;
	const std::vector<NoteTrackSource *> &trackNodes = playbackController->GetGraphController()->GetEffectGraph()->TrackNodes();
	EventMessage message;
	trackNodes.at(0)->SendMessage(EventMessage::CreateKillAllNotes(0));
	playbackController->Pause();
}

DLL_EXTERN void Stop()
{
	std::cout << "Stopped" << std::endl;
	const std::vector<NoteTrackSource *> &trackNodes = playbackController->GetGraphController()->GetEffectGraph()->TrackNodes();
	EventMessage message;
	trackNodes.at(0)->SendMessage(EventMessage::CreateKillAllNotes(0));
	playbackController->Stop();
}