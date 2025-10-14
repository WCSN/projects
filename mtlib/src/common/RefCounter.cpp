#include <mt/common/RefCounter.h>
#include <mt/thread/AutoMutex.h>

namespace mt {

	RefCounter::RefCounter(const unsigned int _count) : m_refCount(_count) {
	}

	void RefCounter::increment() {
		mt::AutoMutex cs(m_mutex);
		m_refCount++;
	}

	bool RefCounter::decrement() {
		mt::AutoMutex cs(m_mutex);
		m_refCount--;
		return (m_refCount == 0);
	}

	int RefCounter::current_count() {
		return m_refCount;
	}

	bool RefCounter::null() {
		return (m_refCount == 0);
	}
}
