#include "stdafx.h"
#include "log_pool.h"


__BEGIN_NAMESPACE

log_pool::log_pool(void)
{

}

log_pool::~log_pool(void)
{

}

log_pool::log_buffer* log_pool::get(void)
{
	return m_pool.get();
}

void log_pool::cycle(log_buffer* p)
{
	m_pool.cycle(p);
}

__END_NAMESPACE
