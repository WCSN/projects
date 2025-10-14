#ifndef __MT_COMMON_LIB__MT_REF_COUNTER__H
#define __MT_COMMON_LIB__MT_REF_COUNTER__H

#include "MT_threads.h"


class MT_ref_counter
{
private:
	volatile int refCount;
	MT_mutex mutex;

public:
	MT_ref_counter(const unsigned int _count);
	~MT_ref_counter() {}

public:
	void increment();
	bool decrement(); //return TRUE if refCounter == 0 after decrement
	int current_count();
	bool null();
};

#endif //__MT_COMMON_LIB__MT_REF_COUNTER__H
