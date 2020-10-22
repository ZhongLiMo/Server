#ifndef BASE_FUNC_H
#define BASE_FUNC_H

#include <ctime>
#include <string>

namespace BASE_FUNC
{
	tm		GetCurTmTime();
	int		GetCurMS();
	time_t	GetCurTimestamp();
	time_t	GetCurTimeMS();
	bool	CreateDirectory(const std::string& file_dir);


}

#endif // !BASE_FUNC_H