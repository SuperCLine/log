#include "stdafx.h"
#include "log_logger_console.h"


__BEGIN_NAMESPACE


enum EColorCode {
	FG_RED = 31,
	FG_GREEN = 32,
	FG_BLUE = 34,
	FG_DEFAULT = 39,
	BG_RED = 41,
	BG_GREEN = 42,
	BG_BLUE = 44,
	BG_DEFAULT = 49
};

class Modifier 
{
	EColorCode code;
public:
	Modifier(EColorCode c) : code(c) 
	{}

	friend std::ostream& operator<<(std::ostream& os, const Modifier& mod) 
	{
		return os << "\033[" << mod.code << "m";
	}
};


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

void log_logger_console::log(ELogType type, const char* log)
{
	static Modifier red(FG_RED);
	static Modifier green(FG_GREEN);
	static Modifier def(FG_DEFAULT);

	switch (type)
	{
	case ELT_INFO:
		std::cout << green << log << def << std::endl;
		break;
	case ELT_ERROR:
		std::cout << red << log << def << std::endl;
		break;
	case ELT_PERF:
	case ELT_DEBUG:
		std::cout << log << std::endl;
		break;
	}
}

__END_NAMESPACE
