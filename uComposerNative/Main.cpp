#include <iostream>
#include <assert.h>
#include "BaseEffect.h"
#include "FlattenGraphVisitor.h"
#include "EffectList.h"

#include "PlaybackController.h"
#include "TempoControlTrack.h"

using namespace uComposer;

void PrintInputs(BaseEffect *node)
{
	int index = 0;
	std::cout << "Inputs" << std::endl;
	for (auto i : node->AudioInputs())
	{
		std::cout << index << ") " << i->Name() << std::endl;
		index++;
	}
}

void PrintOutputs(BaseEffect *node)
{
	int index = 0;
	std::cout << "Outputs" << std::endl;
	for (auto i : node->AudioOutputs())
	{
		std::cout << index << ") " << i->Name() << std::endl;
		index++;
	}
}

void TestMixerNodeAddChannels()
{
	MixerEffect *t = new MixerEffect();
	t->Initialize();

	while (true)
	{
		std::string in;
		std::cout << "Op? c+ c- i+ i-" << std::endl;

		std::cin >> in;

		if (in == "c+")
			t->AddChannel();
		if (in == "c-")
			t->RemoveChannel();
		if (in == "i+")
			t->AddInputBus();
		if (in == "i-")
			t->RemoveInputBus();

		PrintInputs(t);
		PrintOutputs(t);
	}

	PrintInputs(t);
	PrintOutputs(t);

	t->RemoveChannel();
	PrintInputs(t);
	PrintOutputs(t);

	t->RemoveInputBus();

	PrintInputs(t);
	PrintOutputs(t);


	t->AddChannel();

	PrintInputs(t);
	PrintOutputs(t);

	t->AddInputBus();

	PrintInputs(t);
	PrintOutputs(t);


	t->RemoveChannel();

	PrintInputs(t);
	PrintOutputs(t);


	t->RemoveInputBus();

	PrintInputs(t);
	PrintOutputs(t);
}


void TestTempoControl()
{
	TempoControlTrack tempoControl;

	TempoControlPoint p0 = TempoControlPoint(120, TrackTime(0, 0));
	TempoControlPoint p1 = TempoControlPoint(140, TrackTime(1, 0));
	tempoControl.AddControlPoint(p0);
	tempoControl.AddControlPoint(p1);

	assert(tempoControl.ControlPoints().at(0) == p0);
	assert(tempoControl.ControlPoints().at(1) == p1);

	TempoControlPoint p2 = TempoControlPoint(160, TrackTime(0, 2));
	tempoControl.AddControlPoint(p2);

	assert(tempoControl.ControlPoints().at(0) == p0);
	assert(tempoControl.ControlPoints().at(1) == p2);
	assert(tempoControl.ControlPoints().at(2) == p1);


	TempoControlPoint p0a = TempoControlPoint(130, TrackTime(0, 0));
	tempoControl.AddControlPoint(p0a);
	assert(tempoControl.ControlPoints().size() == 3);
	assert(tempoControl.ControlPoints().at(0) == p0a);

	tempoControl.RemoveControlPoint(p0a);
	assert(tempoControl.ControlPoints().size() == 2);
	assert(tempoControl.ControlPoints().at(0) == p2);

	std::cout << "TempoControl test passed" << std::endl;
}

void main()
{
	TestTempoControl();
	//TestGraph();
	//TestMixerNodeAddChannels();
	

	std::cout << "Done!" << std::endl;
	int l;
	std::cin >> l;
	return;
}