#ifndef COST_TIME_H
#define COST_TIME_H

#include <ctime>
#include "mylog.h"

#define COST_TIME_CONSTRUCTOR __LINE__, __FUNCTION__

class CostTime
{
public:
	CostTime(MyLog& mylog, const int& fun_line, const std::string& fun_name) :
		mylog(mylog), fun_name(fun_name), fun_line(fun_line), start(clock())
	{
	}
	~CostTime()
	{
		mylog.SaveLog(LOG_LEVEL_INFO, fun_line, fun_name.c_str(), "use time %ld ms.", clock() - start);
	}
private:
	int				fun_line;
	MyLog&			mylog;
	clock_t			start;
	std::string		fun_name;
};

#endif // !COST_TIME_H