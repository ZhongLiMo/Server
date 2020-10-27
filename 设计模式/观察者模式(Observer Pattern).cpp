#include <set>
#include <iostream>
#include <vector>
#include <list>

class Timer;
class TimerManager
{
public:
	void OnTimer()
	{
		for (auto ite : listeners)
		{
			ite->OnTimer();
		}
	}
private:
	void RemoveTimer(Timer* timer)
	{
		std::set<Timer*>::iterator ite = listeners.find(timer);
		if (ite != listeners.end())
		{
			listeners.erase(ite);
		}
	}
	void RegisterTimer(Timer* timer)
	{
		if (listeners.find(timer) == listeners.end())
		{
			listeners.insert(timer);
		}
	}
private:
	std::set<Timer*>			listeners;
};
class Timer
{
public:
	void OnTimer() { /*do something*/ };
};