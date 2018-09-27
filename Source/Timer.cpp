#pragma once

#include "Timer.h"

Timer::Timer()
	:prev(0),
	current(0),
	delta(0)
{
}

void Timer::Count()
{
	current = clock();
	delta = float(current - prev) / CLOCKS_PER_SEC;
	prev = current;
}

float Timer::GetDelta()
{
	return delta;
}

float Timer::GetFPS()
{
	return 1.0f / (GetMS() / 1000);
}

float Timer::GetMS()
{
	return delta * 1000;
}