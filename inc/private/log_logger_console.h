/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \log\inc\private\log_logger_console.h
| AUTHOR     : CLine
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-25      CLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _LOG_LOGGER_CONSOLE_H_
#define _LOG_LOGGER_CONSOLE_H_


__BEGIN_NAMESPACE


class log_logger_console : public core_interface, public interface_logger
{
public:
	explicit log_logger_console(ELoggerType type);
	virtual ~log_logger_console(void);

	virtual ELoggerType log_type(void);
	virtual void log(ELogType type, const char* log);
};


__END_NAMESPACE


#endif //_LOG_LOGGER_CONSOLE_H_