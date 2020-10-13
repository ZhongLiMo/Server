#ifndef MYLOG_H
#define MYLOG_H

#include <string>

enum LOG_LEVEL
{
	LOG_LEVEL_FATAL = 0,
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_WARN	= 2,
	LOG_LEVEL_INFO	= 3,
	LOG_LEVEL_PROTO = 4,
	LOG_LEVEL_DEBUG = 5
};

class MyLog
{
public:
	void Start(std::string log_file, std::string file_dir, long max_size = 1024 * 1024 * 100, LOG_LEVEL log_level = LOG_LEVEL_DEBUG);
	void SaveLog(LOG_LEVEL log_level, char *format, ...);
	static MyLog* Instance()
	{
		static MyLog mylog;
		return &mylog;
	}
private:
	MyLog();
	~MyLog();
	MyLog(const MyLog&) = delete;
	void check_log_file();
	bool check_log_date();
	std::string get_file_name();
	void output(char *buf, int cnt);
private:
	struct tm			m_cur_day;
	long				m_max_size;
	FILE*				m_log_file;
	std::string			m_file_name;
	std::string			m_file_dir;
	LOG_LEVEL			m_log_level;
};

extern char g_format[10240];

#define log_panic(format, ...)\
{\
	snprintf(g_format, sizeof(g_format),"line[%d] func[%s] %s\n", __LINE__, __FUNCTION__, format); \
	MyLog::Instance()->SaveLog(LOG_LEVEL_FATAL, g_format, ##__VA_ARGS__); \
}
#define log_error(format, ...)\
{\
	snprintf(g_format, sizeof(g_format),"line[%d] func[%s] %s\n", __LINE__, __FUNCTION__, format); \
	MyLog::Instance()->SaveLog(LOG_LEVEL_ERROR, g_format, ##__VA_ARGS__); \
}
#define log_warn(format, ...)\
{\
	snprintf(g_format, sizeof(g_format),"line[%d] func[%s] %s\n", __LINE__, __FUNCTION__, format); \
	MyLog::Instance()->SaveLog(LOG_LEVEL_WARN, g_format, ##__VA_ARGS__); \
}
#define log_info(format, ...)\
{\
	snprintf(g_format, sizeof(g_format),"line[%d] func[%s] %s\n", __LINE__, __FUNCTION__, format); \
	MyLog::Instance()->SaveLog(LOG_LEVEL_INFO, g_format, ##__VA_ARGS__); \
}
#define log_proto(format, ...)\
{\
	snprintf(g_format, sizeof(g_format),"line[%d] func[%s] %s\n", __LINE__, __FUNCTION__, format); \
	MyLog::Instance()->SaveLog(LOG_LEVEL_PROTO, g_format, ##__VA_ARGS__); \
}
#define log_debug(format, ...)\
{\
	snprintf(g_format, sizeof(g_format),"line[%d] func[%s] %s\n", __LINE__, __FUNCTION__, format); \
	MyLog::Instance()->SaveLog(LOG_LEVEL_DEBUG, g_format, ##__VA_ARGS__); \
}

#endif // !MYLOG_H