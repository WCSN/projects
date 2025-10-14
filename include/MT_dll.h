#ifndef MT_DLL_H
#define MT_DLL_H

#include <MT_common.h>
#include <MT_string.h>

#ifdef WIN32
	using namespace std;
#endif

MT_EXTERNAL_SYMBOL void* MT_getdllfunc(const MT_string &dll, const MT_string &symbol);
MT_EXTERNAL_SYMBOL void* MT_getdllfunc(const MT_string &function);
#endif
