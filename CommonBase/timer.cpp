#include "timer.h"

#include "basefunc.h"

static const int DEAFULT_TIMER_STAMP_MS = 1;

TimerManager::TimerManager() : next_time(0), timer_iter(listeners.end()), need_change_iter(true)
{

}
TimerManager::~TimerManager()
{

}
void TimerManager::RegisterTimer(Timer* timer)
{
	if (!timer) return;
	if (listeners.find(timer) == listeners.end()) 
		listeners.insert(timer);
}
void TimerManager::RemoveTimer(Timer* timer)
{
	if (!timer) return;
	std::set<Timer*>::iterator ite = listeners.find(timer);
	if (ite != listeners.end())
	{
		if (ite == timer_iter)
		{
			need_change_iter = false;
			timer_iter = listeners.erase(ite);
		}
		else
		{
			listeners.erase(ite);
		}
	}
}
void TimerManager::OnTimer()
{
	time_t cur_time = BASE_FUNC::GetCurTimeMS();
	if (next_time < cur_time)
	{
		next_time = cur_time + DEAFULT_TIMER_STAMP_MS;
		for (timer_iter = listeners.begin(); timer_iter != listeners.end(); )
		{
			if ((*timer_iter)->next_time < cur_time)
			{
				(*timer_iter)->task();
				(*timer_iter)->next_time = cur_time + (*timer_iter)->loop_time;
				if (!(*timer_iter)->loop)
				{
					RemoveTimer(*timer_iter);
				}
			}
			if (need_change_iter)
			{
				++timer_iter;
			}
			else
			{
				need_change_iter = true;
			}
		}
	}
}

Timer::~Timer()
{
	timer_manager.RemoveTimer(this);
}
void Timer::StartTimer(int ms, bool loop)
{
	timer_manager.RemoveTimer(this);
	this->loop = loop;
	this->loop_time = ms;
	next_time = BASE_FUNC::GetCurTimeMS() + loop_time;
	if (this->loop_time > 0)
	{
		timer_manager.RegisterTimer(this);
	}
}
void Timer::StopTimer()
{
	timer_manager.RemoveTimer(this);
}