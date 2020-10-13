#include "mylog.h"

#include <iostream>
#include <chrono>
#include <time.h>
using namespace std;
using namespace std::chrono;

int main()
{
	MyLog molog("GameMain", "../log");
	molog.SaveLog(LOG_DEBUG, "%s��%s", "ning", "shi");
	//while (1)
	//{
	//	molog.SaveLog(LOG_ERROR, "%s��%s", "ning", "shi");
	//	molog.SaveLog(LOG_WARN, "%s��%s", "ning", "shi");
	//	molog.SaveLog(LOG_INFO, "%s��%s", "ning", "shi");
	//	molog.SaveLog(LOG_PROTO, "%s��%s", "ning", "shi");
	//	molog.SaveLog(LOG_DEBUG, "%s��%s", "ning", "shi");
	//}

	uint64_t timestamp(duration_cast<milliseconds>(chrono::system_clock::now().time_since_epoch()).count()); // ��ȡʱ��������룩

	uint64_t milli = timestamp + 8 * 60 * 60 * 1000; // תΪ����������ʱ��
	auto mTime = milliseconds(milli);
	auto tp = time_point<system_clock, milliseconds>(mTime);
	auto tt = system_clock::to_time_t(tp);
	tm now;
	gmtime_s(&now, &tt);
	printf("%4d��%02d��%02d�� %02d:%02d:%02d.%03d\n", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, int(timestamp % 1000));

	cin.get();

	return 0;
	return 0;
}