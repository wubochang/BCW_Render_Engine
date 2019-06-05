#ifndef _TIMER_H_
#define _TIMER_H_

#include "GLWindow.h"
#include "Misc.h"

class Timer
{
	MAKE_SINGLETON(Timer)

public:

	void Tick()
	{
		static double oldTime = glfwGetTime();
		double newTime = glfwGetTime();
		m_deltaTime = newTime - oldTime;
		oldTime = newTime;
	}

	// get delta time between current tick and previous tick
	double GetDeltaTime()
	{
		return m_deltaTime;
	}

private:

	double m_deltaTime;

};

#endif // !_TIMER_H_
