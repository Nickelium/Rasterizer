#pragma once

#include "Timer.h"

Timer::Timer()
	:prev(0),
	current(0),
	delta(0),
	accumulated(0)
{
}

void Timer::Count()
{
	current = clock();
	delta = float(current - prev) / CLOCKS_PER_SEC;
	prev = current;
	accumulated += delta;
}

float Timer::GetDelta() const
{
	return delta;
}

float Timer::GetFPS() const
{
	return 1.0f / (GetMS() / 1000);
}

float Timer::GetMS() const
{
	return delta * 1000;
}

float Timer::GetTotal() const
{
	return accumulated;
}
