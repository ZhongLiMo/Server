#include "basefunc.h"

namespace BASE_FUNC
{

	tm		get_cur_tm_time()
	{
		time_t cur_time;
		time(&cur_time);
		return *localtime(&cur_time);
	}





}