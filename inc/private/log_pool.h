/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core.log\inc\private\log_pool.h
| AUTHOR     : https://supercline.com/
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-25      SuperCLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _LOG_POOL_H_
#define _LOG_POOL_H_


__BEGIN_NAMESPACE


class log_pool
{
public:
	struct log_buffer
	{
		ELogType type;
		char data[1024];
		
		void init(ELogType tp)
		{
			type = tp;
			app_memset(data, 0, app_array_len(data));
		}

		std::size_t get_size(void)
		{
			return 1024;
		}
	};

public:
	log_pool(void);
	~log_pool(void);

	log_buffer* get(void);
	void cycle(log_buffer* p);

private:
	core_pool<log_buffer> m_pool;
};


__END_NAMESPACE


#endif //_LOG_POOL_H_