#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <set>



class Timer;
class TimerManager
{
public:
	TimerManager();
	~TimerManager();
	void RegisterTimer(Timer* timer);
	void RemoveTimer(Timer* timer);
	void OnTimer();
private:
	std::set<Timer*> listeners;
	time_t next_time;
	bool need_change_iter;
	std::set<Timer*>::iterator timer_iter;
};

class Timer
{
public:
	template<typename callback, typename... arguments>
	Timer(TimerManager& timer_manager, callback&& fun, arguments&&... args) :
		task(std::bind(std::forward<callback>(fun), std::forward<arguments>(args)...)),
		next_time(0), timer_manager(timer_manager), loop(false), loop_time(0)
	{

	}
	~Timer();
	void StartTimer(int ms, bool loop = false);
	void StopTimer();
private:
	std::function<void()> task;
	bool loop;
	int loop_time;
	time_t next_time;
	TimerManager& timer_manager;
	friend class TimerManager;
};



#endif // !TIMER_H