//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AzulCore.h"
#include "LeftRightPan.h"
#include "BasicsDemo.h"
#include "SndMan.h"

using namespace ThreadFramework;

// cool way to deal with time...
using namespace std::chrono_literals;

float setPan = 0.0f;


float Lerp(float A, float B, float t)
{
	float tmp;
	tmp = A + t * (B - A);
	return tmp;
}

void UpdatePan(float pan, float& left, float& right)
{
	left = 0.5f - pan * 0.5f;
	right = 0.5f + pan * 0.5f;

}


void LeftRightPan_Main()
{
	SimpleBanner b;

	Debug::out("- LEFT TO RIGHT PANNING - \n");

	float pan(0.0f);
	float left(0.0f);
	float right(0.0f);


	Time TimeTotal = Time(Duration::TIME_ONE_SECOND) * 2;
	Time TimeElapsed(Duration::TIME_ZERO);

	float tElapsed = (float)Time::quotient(TimeElapsed, Time(Duration::TIME_ONE_MILLISECOND));
	float tTotal = (float)Time::quotient(TimeTotal, Time(Duration::TIME_ONE_MILLISECOND));
	float ratio = 0.0f;

	Timer tA;

	Handle::Status status1;
	Snd* pSndA = SndMan::Add(Snd::ID::Bassoon);
	status1 = pSndA->Vol(0.7f);
	status1 = pSndA->Play();

	while (TimeElapsed <= TimeTotal)
	{
		tA.tic();

		tElapsed = (float)Time::quotient(TimeElapsed, Time(Duration::TIME_ONE_MILLISECOND));
		ratio = tElapsed / tTotal;

		// moves from -1...0...1
		pan = Lerp(-1.0f, 1.0f, ratio);

		UpdatePan(pan, left, right);

		Debug::out("l: %0.2f  r: %0.2f\n", left, right);

		std::this_thread::sleep_for(1ms);

		// subtract time passed
		TimeElapsed += tA.toc();

		status1 = pSndA->Pan(pan);
	}
}

void RightLeftPan_Main()
{
	SimpleBanner b;

	Debug::out("- RIGHT TO LEFT PANNING - \n");

	float pan(0.0f);
	float left(0.0f);
	float right(0.0f);


	Time TimeTotal = Time(Duration::TIME_ONE_SECOND) * 2;
	Time TimeElapsed(Duration::TIME_ZERO);

	float tElapsed = (float)Time::quotient(TimeElapsed, Time(Duration::TIME_ONE_MILLISECOND));
	float tTotal = (float)Time::quotient(TimeTotal, Time(Duration::TIME_ONE_MILLISECOND));
	float ratio = 0.0f;

	Timer tA;

	Handle::Status status1;
	Snd* pSndA = SndMan::Add(Snd::ID::Bassoon);
	status1 = pSndA->Vol(0.7f);
	status1 = pSndA->Play();

	while (TimeElapsed <= TimeTotal)
	{
		tA.tic();

		tElapsed = (float)Time::quotient(TimeElapsed, Time(Duration::TIME_ONE_MILLISECOND));
		ratio = tElapsed / tTotal;

		// moves from -1...0...1
		pan = Lerp(1.0f, -1.0f, ratio);

		UpdatePan(pan, left, right);

		Debug::out("l: %0.2f  r: %0.2f\n", left, right);

		std::this_thread::sleep_for(1ms);

		// subtract time passed
		TimeElapsed += tA.toc();

		status1 = pSndA->Pan(pan);
	}
}



// End of File 