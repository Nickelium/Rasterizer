#include <ctime>

class Timer
{
public:
	Timer();
	void Count();

	float GetDelta();
	float GetFPS();
	float GetMS();
private:
	clock_t prev;
	clock_t current;

	//unit : [s]
	float delta;
};