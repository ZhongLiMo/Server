#ifndef TIMER_H
#define TIMER_H

#include <set>
#include <functional>

class Timer;
class TimerManager
{
public:
	TimerManager();
	virtual ~TimerManager();
public:
	void OnTimer();
	void RemoveTimer(Timer* timer);
	void RegisterTimer(Timer* timer);
private:
	time_t						next_time;
	bool						change_iter;
	std::set<Timer*>			listeners;
	std::set<Timer*>::iterator	timer_iter;
};

class Timer
{
public:
	template<typename callback, typename... arguments>
	Timer(TimerManager& timer_manager, callback&& fun, arguments&&... args) :
		task(std::bind(std::forward<callback>(fun), std::forward<arguments>(args)...)),
		next_time(0), timer_manager(timer_manager), loop_time(0) {}
	virtual ~Timer();
	void StopTimer();
	void StartTimer(int delay_time, bool loop = false);
	void RestartTimer(int delay_time, bool loop = false);
private:
	int						loop_time;
	time_t					next_time;
	TimerManager&			timer_manager;
	std::function<void()>	task;
	friend class TimerManager;
};

#endif // !TIMER_H