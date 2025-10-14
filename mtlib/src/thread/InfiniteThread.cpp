#include <mt/thread/InfiniteThread.h>
#include <mt/utils/System.h>

namespace mt {

	InfiniteThread::InfiniteThread(uint32_t timeout) : m_active(false),
																										 m_timeout(timeout) {
	}

	InfiniteThread::InfiniteThread(const std::string& name, uint32_t stackSize, uint32_t timeout) : Thread(name, stackSize),
																																																	m_active(false),
																																																	m_timeout(timeout){
	}

	InfiniteThread::~InfiniteThread() {
		stop();

	}

	void InfiniteThread::start() {
		{
			AutoMutex amtx(m_activeMutex);
			m_active = true;
		}
		Thread::start();
	}

	void InfiniteThread::stop() {
		{

			AutoMutex amtx(m_activeMutex);
			m_active = false;
		}
		Thread::stop();
	}

	bool InfiniteThread::active() {
		AutoMutex amtx(m_activeMutex);
		return m_active;
	}

	void InfiniteThread::run() {
		while(active()) {
			runLoop();
			if(m_timeout > 0)
				mt::System::sleep(m_timeout);
		}
	}

}
