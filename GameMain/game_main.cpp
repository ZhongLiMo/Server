#include "mylog.h"

int main()
{
	MyLog::Instance()->Start("GameMain","../log");
	while (1)
	{
		log_debug("ning");
		log_info("jie");
		log_proto("biao");
		log_error("chi");
		log_warn("shi");
	}
	return 0;
}