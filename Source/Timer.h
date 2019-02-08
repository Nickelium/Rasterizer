#pragma once

#include <ctime>

/*
 * Class Timer
 * 
 * - Class tracking time related metrics
 * - ms, fps, total time etc...
 */
class Timer
{
public:
	Timer();
	void Count();

	float GetDelta() const;
	float GetFPS() const;
	float GetMS() const;

	float GetTotal() const;
private:
	clock_t prev;
	clock_t current;

	//unit : [s]
	float delta;
	float accumulated;
};