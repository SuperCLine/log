/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \log\inc\private\log_property.h
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


#ifndef _LOG_PROPERTY_H_
#define _LOG_PROPERTY_H_


__BEGIN_NAMESPACE


class log_property :public core_property
{
public:
	typedef struct t_logger_type
	{
		ELoggerType type;
		ustring name;
	} logger_type;

public:
	log_property(void);
	~log_property(void);

	virtual ustring serialize(void);
	virtual bool deserialize(const ustring& name);

	bool is_asyn(void);
	const ulist<logger_type>& get_logger_list(void);
	umap<ustring, bool>& get_filter_map(void);
	umap<ustring, bool>& get_tag_map(void);

private:
	struct log_property_impl* m_impl;
};


__END_NAMESPACE


#endif //_LOG_PROPERTY_H_