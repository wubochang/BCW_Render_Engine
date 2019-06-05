#ifndef _TIMER_H_
#define _TIMER_H_

class Timer
{
public:
	static Timer& getInstance()
	{
		static Timer instance;

		return instance;
	}

	void Tick()
	{
	}

	// get delta time between current tick and previous tick
	float GetDeltaTime()
	{
	}

private:

	Timer() {}

	Timer(Timer const&) = delete;
	void operator=(Timer const&) = delete;
};

#endif // !_TIMER_H_
