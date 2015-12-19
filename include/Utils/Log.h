#ifndef LOG_H
#define LOG_H

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL logDEBUG4
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <Windows.h>
#endif

#include <sstream>
#include <string>
#include <cstdio>

inline std::string nowTime();

enum TLogLevel
{
	logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4
};

template <typename T>
class BaseLog
{
public:
	BaseLog();
	virtual ~BaseLog();
	std::ostringstream& get(TLogLevel level, const char* func, int line);
public:
	static TLogLevel& reportingLevel();
	static std::string toString(TLogLevel level);
	static TLogLevel fromString(const std::string& level);
protected:
	std::ostringstream os;
private:
	BaseLog(const BaseLog&);
	BaseLog& operator =(const BaseLog&);
};

template <typename T>
BaseLog<T>::BaseLog()
{
}

template <typename T>
std::ostringstream& BaseLog<T>::get(TLogLevel level, const char* func, int line)
{
	os << "- " << nowTime();
	os << " " << toString(level);
	os << " ";
	os << func << "@";
	os << line << ": ";
	os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
	return os;
}

template <typename T>
BaseLog<T>::~BaseLog()
{
	os << std::endl;
	T::output(os.str());
}

template <typename T>
TLogLevel& BaseLog<T>::reportingLevel()
{
	static TLogLevel reportingLevel = logDEBUG4;
	return reportingLevel;
}

template <typename T>
std::string BaseLog<T>::toString(TLogLevel level)
{
	static const char* const buffer[] = { "ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4" };
	{
		return buffer[level];
	}
}

template <typename T>
TLogLevel BaseLog<T>::fromString(const std::string& level)
{
	if (level == "DEBUG4")
		return logDEBUG4;
	if (level == "DEBUG3")
		return logDEBUG3;
	if (level == "DEBUG2")
		return logDEBUG2;
	if (level == "DEBUG1")
		return logDEBUG1;
	if (level == "DEBUG")
		return logDEBUG;
	if (level == "INFO")
		return logINFO;
	if (level == "WARNING")
		return logWARNING;
	if (level == "ERROR")
		return logERROR;
	BaseLog<T>().get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
	return logINFO;
}


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

inline std::string nowTime()
{
	const int MAX_LEN = 200;
	char buffer[MAX_LEN];
	if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0,
		"HH':'mm':'ss", buffer, MAX_LEN) == 0)
		return "Error in nowTime()";

	char result[100] = { 0 };
	static DWORD first = GetTickCount();
	sprintf_s(result, "%s.%03ld", buffer, (long)(GetTickCount() - first) % 1000);
	return result;
}

#else

#include <sys/time.h>

inline std::string nowTime()
{
	char buffer[11];
	time_t t;
	time(&t);
	tm r = { 0 };
	strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
	struct timeval tv;
	gettimeofday(&tv, 0);
	char result[100] = { 0 };
	std::sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000);
	return result;
}

#endif //WIN32

class Output2FILE
{
public:
	static FILE*& stream();
	static void output(const std::string& msg);
};

inline FILE*& Output2FILE::stream()
{
	static FILE* pStream = stderr;
	return pStream;
}

inline void Output2FILE::output(const std::string& msg)
{
	FILE* pStream = stream();
	if (!pStream)
		return;
	fprintf(pStream, "%s", msg.c_str());
	fflush(pStream);
}

class Output2All
{
public:
	static void output(const std::string& msg);
};

inline void Output2All::output(const std::string& msg)
{
	if (Output2FILE::stream())
	{
		Output2FILE::output(msg);
	}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	std::wostringstream os;
	os << msg.c_str();
	OutputDebugStringW(os.str().c_str());
#endif
}

class Log : public BaseLog<Output2All> {};

#if defined(WIN32)
#define __func__ __FUNCTION__
#endif

#define LOG(level) \
    if (level > FILELOG_MAX_LEVEL) ;\
    else if (level > Log::reportingLevel()) ; \
		else Log().get(level, __func__, __LINE__)

#define LOG_TRACE LOG(logDEBUG1)
#define LOG_DEBUG LOG(logDEBUG)
#define LOG_INFO LOG(logINFO)
#define LOG_WARN LOG(logWARNING)
#define LOG_ERROR LOG(logERROR)

#endif //LOG_H
