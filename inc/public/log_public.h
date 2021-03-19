/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core.log\inc\public\log_public.h
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


#ifndef _LOG_PUBLIC_H_
#define _LOG_PUBLIC_H_


#include "interface_log.h"

extern "C"
{
	LOG_API interface_logmgr*	load_module_log(void);
	LOG_API void				unload_module_log(interface_logmgr* p);
};

#endif //_LOG_PUBLIC_H_