#include "stdafx.h"
#include "log_logger_console.h"


__BEGIN_NAMESPACE

log_logger_console::log_logger_console(ELoggerType type)
{

}

log_logger_console::~log_logger_console(void)
{

}

ELoggerType log_logger_console::log_type(void)
{
	return ELoggerType::ELT_COSOLE;
}

#if defined(WIN32) || defined(_WIN32)
std::ostream& color_default(std::ostream& os)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return os;
}
std::ostream& color_debug(std::ostream& os)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return os;
}
std::ostream& color_error(std::ostream& os)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		FOREGROUND_RED | FOREGROUND_INTENSITY);
	return os;
}
std::ostream& color_perf(std::ostream& os)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	return os;
}
#else
std::ostream& color_default(std::ostream& os)
{
	return os << "\033[37m";
}
std::ostream& color_debug(std::ostream& os)
{
	return os << "\033[1m\033[36m";
}
std::ostream& color_error(std::ostream& os)
{
	return os << "\033[31m";
}
std::ostream& color_perf(std::ostream& os)
{
	return os << "\033[32m";
}
#endif

void log_logger_console::log(ELogType type, const char* log)
{
	switch (type)
	{
	case ELT_DEBUG:
		std::cout << color_debug << log << color_default << std::endl;
		break;
	case ELT_INFO:
		std::cout << color_default << log << color_default << std::endl;
		break;
	case ELT_ERROR:
		std::cout << color_error << log << color_default << std::endl;
		break;
	case ELT_PERF:
		std::cout << color_perf << log << color_default << std::endl;
		break;
	}
}

__END_NAMESPACE
