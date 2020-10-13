#include "basefunc.h"

#include <chrono>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

namespace BASE_FUNC
{

	tm GetCurTmTime()
	{
		time_t cur_time;
		time(&cur_time);
		return *localtime(&cur_time);
	}

	int GetCurTimeMS()
	{
		return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000);
	}

	bool CreateDirectory(const std::string& file_dir)
	{
#ifdef _WIN32
		int ret = _mkdir(file_dir.c_str());
#else
		nRet = mkdir(file_dir.c_str(), S_IRWXU);
#endif
		if (ret != 0 && errno != EEXIST) return false;
		return true;
	}



}