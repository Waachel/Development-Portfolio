//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AzulCore.h"
#include "VolumeChange.h"
#include "BasicsDemo.h"
#include "SndMan.h"

using namespace ThreadFramework;

// cool way to deal with time...
using namespace std::chrono_literals;

float setVolumeChange = 0.0f;


float ChangeVolume(float A, float B, float t)
{
	float tmp;
	tmp = A + t * (B - A);
	return tmp;
}

void UpdateVolume(float volume)
{
	setVolumeChange = volume;
}


void VolumeUp_Main()
{
	SimpleBanner b;

	Debug::out("- VOLUME UP - \n");

	float volume(0.0f);

	Time TimeTotal = Time(Duration::TIME_ONE_SECOND) * 2;
	Time TimeElapsed(Duration::TIME_ZERO);

	float tElapsed = (float)Time::quotient(TimeElapsed, Time(Duration::TIME_ONE_MILLISECOND));
	float tTotal = (float)Time::quotient(TimeTotal, Time(Duration::TIME_ONE_MILLISECOND));
	float ratio = 0.0f;

	Timer tA;

	Handle::Status status1;
	Snd* pSndA = SndMan::Add(Snd::ID::Oboe2_mono);
	status1 = pSndA->Vol(0.0f);
	status1 = pSndA->Play();

	while (TimeElapsed <= TimeTotal)
	{
		tA.tic();

		tElapsed = (float)Time::quotient(TimeElapsed, Time(Duration::TIME_ONE_MILLISECOND));
		ratio = tElapsed / tTotal;

		// moves from -1...0...1
		volume = ChangeVolume(0.0f, 1.0f, ratio);

		UpdateVolume(volume);

		Debug::out("Volume: %0.2f \n", volume);

		std::this_thread::sleep_for(1ms);

		// subtract time passed
		TimeElapsed += tA.toc();

		status1 = pSndA->Vol(volume);
	}
}

void VolumeDown_Main()
{
	SimpleBanner b;

	Debug::out("- VOLUME DOWN - \n");

	float volume(0.0f);

	Time TimeTotal = Time(Duration::TIME_ONE_SECOND) * 2;
	Time TimeElapsed(Duration::TIME_ZERO);

	float tElapsed = (float)Time::quotient(TimeElapsed, Time(Duration::TIME_ONE_MILLISECOND));
	float tTotal = (float)Time::quotient(TimeTotal, Time(Duration::TIME_ONE_MILLISECOND));
	float ratio = 0.0f;

	Timer tA;

	Handle::Status status1;
	Snd* pSndA = SndMan::Add(Snd::ID::Oboe2_mono);
	status1 = pSndA->Vol(0.0f);
	status1 = pSndA->Play();

	while (TimeElapsed <= TimeTotal)
	{
		tA.tic();

		tElapsed = (float)Time::quotient(TimeElapsed, Time(Duration::TIME_ONE_MILLISECOND));
		ratio = tElapsed / tTotal;

		// moves from -1...0...1
		volume = ChangeVolume(1.0f, 0.0f, ratio);

		UpdateVolume(volume);

		Debug::out("Volume: %0.2f \n", volume);

		std::this_thread::sleep_for(1ms);

		// subtract time passed
		TimeElapsed += tA.toc();

		status1 = pSndA->Vol(volume);
	}
}



// End of File 