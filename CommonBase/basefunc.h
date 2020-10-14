#ifndef BASE_FUNC_H
#define BASE_FUNC_H

#include <ctime>
#include <string>

namespace BASE_FUNC
{
	tm		GetCurTmTime();
	int		GetCurTimeMS();
	bool	CreateDirectory(const std::string& file_dir);



}

#endif // !BASE_FUNC_H