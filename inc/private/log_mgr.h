/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \log\inc\private\log_mgr.h
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


#ifndef _LOG_MGR_H_
#define _LOG_MGR_H_


__BEGIN_NAMESPACE


class log_mgr : public core_interface
	, public interface_logmgr
	, public core_workqueue::request_handler
{
public:
	log_mgr(void);
	virtual ~log_mgr(void);

	virtual bool init(const char* config);
	virtual void log(ELogType type, const char* tag, const char* log);
	virtual void logf(ELogType type, const char* tag, const char* format, ...);

private:
	virtual core_workqueue::response* handle_request(const core_workqueue::request* req, const core_workqueue* srcQ);

	bool filter(ELogType type, const char* tag);
	void do_log(ELogType type, const char* log);

private:
	typedef umap<ELoggerType, interface_logger*>	log_map_type;

	log_map_type		m_log_map;
	log_property*		m_property;

	log_pool*			m_pool;
	core_default_workqueue* m_work_queue;
	uint16 m_work_channel;

	umap<ELogType, ustring> m_type_string_map;
};


__END_NAMESPACE


#endif //_LOG_MGR_H_