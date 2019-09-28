#include "stdafx.h"
#include "log_mgr.h"

LOG_API interface_logmgr* load_module_log(void)
{
	return new SuperCLine::log_mgr();
}

LOG_API void unload_module_log(interface_logmgr* p)
{
	app_safe_delete(p);
}

__BEGIN_NAMESPACE

log_mgr::log_mgr(void)
: m_property(0)
, m_pool(0)
, m_work_queue(0)
, m_work_channel(0)
{

}

log_mgr::~log_mgr(void)
{
	if (m_work_queue)
	{
		m_work_queue->remove_request_handler(m_work_channel, this);
		m_work_queue->shutdown();
	}

	app_safe_delete(m_work_queue);
	app_safe_delete(m_property);

	{
		ulocker scoped_locker(m_log_buf_list_mutex);
		for (log_buf_list_type::iterator itr = m_log_buf_list.begin(); itr != m_log_buf_list.end(); ++itr)
		{
			m_pool->cycle(*itr);
		}
		m_log_buf_list.clear();
	}
	app_safe_delete(m_pool);

	for (log_map_type::iterator itr = m_log_map.begin(); itr != m_log_map.end(); ++itr)
	{
		app_safe_delete(itr->second);
	}
	m_log_map.clear();
}

bool log_mgr::init(const char* config)
{
	m_type_string_map.insert(umap<ELogType, ustring>::value_type(ELT_DEBUG, "DEBUG"));
	m_type_string_map.insert(umap<ELogType, ustring>::value_type(ELT_INFO, "INFO"));
	m_type_string_map.insert(umap<ELogType, ustring>::value_type(ELT_ERROR, "ERROR"));
	m_type_string_map.insert(umap<ELogType, ustring>::value_type(ELT_PERF, "PERF"));

	m_property = new log_property();
	m_property->deserialize(config);

	const ulist<log_property::logger_type>& logger_list = m_property->get_logger_list();
	for (auto itr = logger_list.begin(); itr != logger_list.end(); ++itr)
	{
		interface_logger* logger = NULL;
		ELoggerType type = (*itr).type;
		switch (type)
		{
		case ELoggerType::ELT_COSOLE:
			logger = new log_logger_console(type);
			break;
		case ELoggerType::ELT_FILE_DEBUG:
			logger = new log_logger_file(type, (*itr).name);
			break;
		case ELoggerType::ELT_FILE_INFO:
			logger = new log_logger_file(type, (*itr).name);
			break;
		case ELoggerType::ELT_FILE_ERROR:
			logger = new log_logger_file(type, (*itr).name);
			break;
		case ELoggerType::ELT_FILE_PERF:
			logger = new log_logger_file(type, (*itr).name);
			break;
		}
		m_log_map.insert(log_map_type::value_type(type, logger));
	}

	if (m_property->is_asyn())
	{
		m_pool = new log_pool();
		m_work_queue = new core_default_workqueue("log_worker");
		m_work_queue->set_response_processing_timelimit(10);

		m_work_channel = m_work_queue->get_channel("log_channel");
		m_work_queue->add_request_handler(m_work_channel, this);

		m_work_queue->startup(1);
	}

	return true;
}

void log_mgr::log(ELogType type, const char* tag, const char* log)
{
	if (filter(type, tag))
		return;

	const char* type_str = m_type_string_map[type].c_str();
	if (m_property->is_asyn())
	{
		log_pool::log_buffer* buf = m_pool->get();
		buf->init(type);
		app_snprintf(buf->data, buf->get_size(), "[%s] [%s] [%s] %s", core_time::curtime_to_string(ETF_COLON), type_str, tag, log);

		m_work_queue->add_request(m_work_channel, 0, buf);

		ulocker scoped_locker(m_log_buf_list_mutex);
		m_log_buf_list.push_back(buf);
	}
	else
	{
		char buf[512] = { 0 };
		app_snprintf(buf, 512, "[%s] [%s] [%s] %s", core_time::curtime_to_string(ETF_COLON), type_str, tag, log);

		do_log(type, buf);
	}
}

void log_mgr::do_log(ELogType type, const char* log)
{
	if (m_log_map[ELT_COSOLE])
	{
		m_log_map[ELT_COSOLE]->log(type, log);
	}

	interface_logger* logger = NULL;
	switch (type)
	{
	case ELT_DEBUG:
		logger = m_log_map[ELT_FILE_DEBUG];
		break;
	case ELT_INFO:
		logger = m_log_map[ELT_FILE_INFO];
		break;
	case ELT_ERROR:
		logger = m_log_map[ELT_FILE_ERROR];
		break;
	case ELT_PERF:
		logger = m_log_map[ELT_FILE_PERF];
		break;
	}
	if (logger)
	{
		logger->log(type, log);
	}
}

void log_mgr::logf(ELogType type, const char* tag, const char* format, ...)
{
	if (filter(type, tag))
		return;

	char buf[512] = { 0 };
	va_list vlist;
	va_start(vlist, format);
	app_vsnprintf(buf, app_array_len(buf), format, vlist);
	va_end(vlist);

	log(type, tag, buf);
}

bool log_mgr::can_handle_request(const core_workqueue::request* req, const core_workqueue* srcQ)
{
	return true;
}

core_workqueue::response* log_mgr::handle_request(const core_workqueue::request* req, const core_workqueue* srcQ)
{
	log_pool::log_buffer* buf = (log_pool::log_buffer*)(req->get_data());

	do_log(buf->type, buf->data);

	{
		ulocker scoped_locker(m_log_buf_list_mutex);
		m_log_buf_list.remove(buf);
	}

	m_pool->cycle(buf);

	return NULL;
}

bool log_mgr::filter(ELogType type, const char* tag)
{
	umap<ustring, bool>& filter_map = m_property->get_filter_map();
	umap<ustring, bool>& tag_map = m_property->get_tag_map();

	bool check_filter = filter_map[m_type_string_map[type]];

	return check_filter || !tag_map[tag];
}

__END_NAMESPACE
