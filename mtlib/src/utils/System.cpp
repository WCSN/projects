#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#include <mt/utils/System.h>

namespace mt {

	void System::sleep(uint32_t ms) {

		struct timespec tv;
		tv.tv_nsec = (ms % 1000) * 1000000;
		tv.tv_sec = ms / 1000;
		nanosleep(&tv, 0);
	}

	time_t System::currentTime() {
		return time(NULL);
	}

	int32_t System::getTid() {
		int32_t tid = (int32_t) pthread_self();
		return tid & 0xFFFF;
	}

	int32_t System::getPid() {
		return getpid();
	}
}
