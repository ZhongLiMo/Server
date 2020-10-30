#ifndef COST_TIME_H
#define COST_TIME_H

#include "mylog.h"
MyLog costtimelog("COSTTIME", "../log", 1024 * 1024 * 100, LOG_LEVEL_DEBUG, true);

#define COST_TIME_CONSTRUCTOR __LINE__, __FUNCTION__

class CostTime
{
public:
	CostTime(const int& fun_line, const std::string& fun_name) :
		fun_name(fun_name), fun_line(fun_line), start(clock())
	{
	}
	~CostTime()
	{
		costtimelog.SaveLog(LOG_LEVEL_INFO, fun_line, fun_name.c_str(), "use time %ld ms.", clock() - start);
	}
private:
	int				fun_line;
	clock_t			start;
	std::string		fun_name;
};

#endif // !COST_TIME_H