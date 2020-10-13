#include "mylog.h"

#include <ctime>
#include <stdarg.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif


char g_format[10240];
static char g_format_buf[1024000];
static const std::string log_level_str[6] = { "fatal", "error", "warn " , "info " ,"proto", "debug" };

MyLog::MyLog() : m_log_file(NULL), m_log_level(LOG_LEVEL_DEBUG), m_max_size(1024 * 1024 * 100)
{
	time_t timep;
	time(&timep);
	m_cur_day = *localtime(&timep);
}
MyLog::~MyLog()
{
	if (m_log_file)
	{
		fflush(m_log_file);
		fclose(m_log_file);
		m_log_file = NULL;
	}
}
void MyLog::Start(std::string file_name, std::string file_dir, long max_size, LOG_LEVEL log_level)
{
	int ret = 0;
#ifdef _WIN32
	ret = _mkdir(file_dir.c_str());
#else
	nRet = mkdir(file_dir.c_str(), S_IRWXU);
#endif
	if (ret != 0 && errno != EEXIST) exit(1);
	m_file_name = file_name;
	m_file_dir = file_dir;
	m_log_file = fopen(get_file_name().c_str(), "w+");
	if (m_log_file == NULL) exit(1);
	m_log_level = log_level;
	m_max_size = max_size < m_max_size ? max_size : m_max_size;
}
void MyLog::SaveLog(LOG_LEVEL log_level, char *format, ...)
{
	time_t timep;
	struct tm* tmp;
	time(&timep);
	tmp = localtime(&timep);
	int len = snprintf(g_format_buf, sizeof(g_format_buf), "[%s][%04d-%02d-%02d %02d:%02d:%02d] ",
		log_level_str[log_level].c_str(),
		tmp->tm_year + 1900,
		tmp->tm_mon + 1,
		tmp->tm_mday,
		tmp->tm_hour,
		tmp->tm_min,
		tmp->tm_sec);
	va_list argptr;
	va_start(argptr, format);
	int cnt = vsprintf(g_format_buf + len, format, argptr);
	va_end(argptr);
	cnt += len;
	output(g_format_buf, cnt);
	if (LOG_LEVEL_FATAL == log_level) exit(1);
}
void MyLog::output(char *buf, int cnt)
{
#ifdef MYDEBUG_ON
	printf("%s", buf);
#endif
	fputs(buf, m_log_file);
	fflush(m_log_file);
	check_log_file();
}
bool MyLog::check_log_date()
{
	time_t timep;
	time(&timep);
	tm* cut_time = localtime(&timep);
	if (cut_time->tm_mday == m_cur_day.tm_mday)
		return true;
	return false;
}
void MyLog::check_log_file()
{
	if (m_log_file && ftell(m_log_file) < m_max_size && check_log_date()) return;
	if (m_log_file) fclose(m_log_file);
	m_log_file = fopen(get_file_name().c_str(), "w+");
}
std::string MyLog::get_file_name()
{
	time_t timep;
	time(&timep);
	tm* cut_time = localtime(&timep);
	char file_name[512];
	snprintf(file_name, sizeof(file_name), "%s/%s-%04d%02d%02d-%02d%02d%02d.log",
		m_file_dir.c_str(), m_file_name.c_str(), cut_time->tm_year + 1900, cut_time->tm_mon + 1,
		cut_time->tm_mday, cut_time->tm_hour, cut_time->tm_min, cut_time->tm_sec);
	return file_name;
}