#include "stdafx.h"
#include "log_logger_file.h"


__BEGIN_NAMESPACE


log_logger_file::log_logger_file(ELoggerType type, const ustring& name)
: m_type(type)
{
	init(name);
}

log_logger_file::~log_logger_file(void)
{
	m_logger.close();
}

ELoggerType log_logger_file::log_type(void)
{
	return m_type;
}

void log_logger_file::log(ELogType type, const char* log)
{
	app_unreferenced(type);
	m_logger << log << "\n";
	m_logger.flush();
}

void log_logger_file::init(const ustring& name)
{
	char exe_path[app_max_path] = { 0 };
	if (!app_get_exedir(NULL, exe_path, app_max_path))
		return;

	ustring cur_path = core_util::get_file_path(exe_path);
	ustring project_name = core_util::get_file_name(exe_path);

	ustring log_path = cur_path + "/logs/";
	if (-1 == app_file_exist(log_path.c_str(), 0))
	{
		if (!app_create_dir(log_path.c_str(), NULL))
		{
			return;
		}
	}

	log_path += project_name;
	if (-1 == app_file_exist(log_path.c_str(), 0))
	{
		if (!app_create_dir(log_path.c_str(), NULL))
		{
			return;
		}
	}

	log_path += "/";
	log_path += core_time::curtime_to_string();

	if (-1 == app_file_exist(log_path.c_str(), 0))
	{
		if (!app_create_dir(log_path.c_str(), NULL))
		{
			return;
		}
	}

	log_path += "/";
	log_path += name.c_str();

	m_logger.open(log_path.c_str());
}

__END_NAMESPACE
