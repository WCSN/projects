#include <mt/thread/Thread.h>
#include <mt/common/Exception.h>

#include <pthread.h>
#include <string.h>

namespace mt {
pthread_key_t Thread::m_threadNameKey = 0;
const pthread_t Thread::m_mainThread = pthread_self();

Thread::Thread() :
	m_tid(0), m_stackSize(DEFAULT_STACK_SIZE) {

	MT_LOGINIT(Thread);
	if (m_threadNameKey == 0)
		pthread_key_create(&m_threadNameKey, &keyDestructor);

	sigemptyset(&m_sigSet);
	sigaddset(&m_sigSet, SIGFPE); //Float point exception
	sigaddset(&m_sigSet, SIGHUP);
	sigaddset(&m_sigSet, SIGPIPE);
	sigaddset(&m_sigSet, SIGCHLD);
	sigaddset(&m_sigSet, SIGALRM);
}

Thread::Thread(const std::string& name, uint32_t stackSize) :
	m_tid(0), m_stackSize(stackSize), m_name(name) {
	MT_LOGINIT(Thread);

	sigemptyset(&m_sigSet);
	sigaddset(&m_sigSet, SIGFPE); //Float point exception
	sigaddset(&m_sigSet, SIGHUP);
	sigaddset(&m_sigSet, SIGPIPE);
	sigaddset(&m_sigSet, SIGCHLD);
	sigaddset(&m_sigSet, SIGALRM);

	if (m_threadNameKey == 0)
		pthread_key_create(&m_threadNameKey, &keyDestructor);
}

Thread::~Thread() {
	if (m_tid)
		pthread_detach(m_tid);
	stop();
}

void Thread::start() {
	if (isRunning())
		throw mt::ThreadAlreadyRunningException();

	m_tid = createThread(&startable, m_stackSize, this);
	if (m_tid == 0) {
		switch (errno) {
		case EAGAIN:
			throw mt::ThreadTooManyThreadsException();
		case EINVAL:
			throw mt::ThreadInvalidArgumentException();
		case ENOMEM:
			throw mt::ThreadNoMemoryException();
		default:
			MT_EXCEPTION(mt::ThreadCantStartException, "Errno=" << errno)
			;
		}
	}
}

void Thread::stop() {
	if (isRunning())
		wait();
}

void Thread::wait() {
	if (isRunning()) {
		checkTid();
		pthread_join(m_tid, NULL);
	}
}

bool Thread::setPriority(int32_t priority) {
	checkTid();
	struct sched_param param;
	param.sched_priority = priority;
	return (pthread_setschedparam(m_tid, SCHED_OTHER, &param) != -1);
}

int32_t Thread::getPriority() {
	checkTid();
	struct sched_param param;
	int policy;
	pthread_getschedparam(m_tid, &policy, &param);
	return param.sched_priority;
}

std::string Thread::name() {
	if (pthread_self() == m_mainThread)// this is the main() thread
		return "main_thread";
	char* name = static_cast<char*> (pthread_getspecific(m_threadNameKey));
	if (name == 0 || strlen(name) == 0)
		return "Unknown thread";
	else
		return std::string(name);
}

void Thread::setName(const std::string& name) {
	char* buffer = new char[32];
	sprintf(buffer, "%.19s(%06d)", name.c_str(), (int32_t) pthread_self());
	pthread_setspecific(m_threadNameKey, buffer);
}

void* Thread::startable(void* thread) {
	MT_LOGINIT_STATIC(Thread);

	Thread* this_thread = (Thread*) thread;
	this_thread->atThreadStart();

	try {
		this_thread->run();
	} catch (const mt::Exception& me) {
		MT_LOGERROR("Stop thread " << (int32_t)this_thread->m_tid << " by error. Reason : " << me.displayText());
	} catch (...) {
		MT_LOGERROR("Stop thread " << (int32_t)this_thread->m_tid << " by unhandled exception.");
	}
	this_thread->atThreadEnd();
	return 0;
}

void Thread::atThreadStart() {
	pthread_setspecific(m_threadNameKey, m_name.c_str());
	pthread_sigmask(SIG_BLOCK, &m_sigSet, NULL);

	if (m_name.empty())
		setName("mt thread");
	else
		setName(m_name);
}

void Thread::atThreadEnd() {
	pthread_exit(0);
}

bool Thread::isRunning() {
	if (m_tid == 0)
		return false;
	struct sched_param param;
	int policy;
	return !(pthread_getschedparam(m_tid, &policy, &param) == ESRCH);
}

pthread_t Thread::createThread(void* (*threadFunc)(void*), uint32_t stack_size,
		void* params) {
	pthread_t ret_val;
	int32_t status = 0;
	if (stack_size > 0) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, stack_size);
		status = pthread_create(&ret_val, &attr, (void*(*)(void*)) threadFunc,
				params);
	} else
		status = pthread_create(&ret_val, 0, (void*(*)(void*)) threadFunc,
				params);

	if (status)
		return 0;
	return ret_val;
}

void Thread::checkTid() {
	if (m_tid == 0)
		m_tid = pthread_self();
}

void Thread::keyDestructor(void* key_value) {
	delete[] (char*) key_value;
}
}
