#ifndef __THREAD__THREAD__H
#define __THREAD__THREAD__H

#include <stdint.h>
#include <string>
#include <signal.h>

#include <mt/logger/Logable.h>

namespace mt {
	/**
	 * @class Thread
	 * @brief The class create thread and start code in this thread.
	 *
	 * <b>Declaration:</b>\n
	 * @code
	 * class InOtherThread : public mt::Thread {
	 *   void run() {
	 *     //This code will work on other thread.
	 *   }
	 * };
	 * @endcode
	 *
	 * <b>Usage:</b> \n
	 * @code
	 * InOtherThread thread;
	 * thread.start(); //Create and start thread.
	 * thread.stop(); //Stop thread.
	 * @endcode
	 */

	class Thread : virtual public Logable {



	public:
		//@{
		Thread();
		/**
		 * @param name Name of the thread.
		 * @param stackSize Stack size of the thread.
		 */
		Thread(const std::string& name, uint32_t stackSize = DEFAULT_STACK_SIZE);
		virtual ~Thread();
		//@}

		//@{
		/**
		 * Create and start thread.
		 * @throws mt::ThreadAlreadyRunningException if the thread already started.
		 * @throws mt::ThreadTooManyThreadsException if threads limit was reached.
		 * @throws mt::ThreadInvalidArgumentException if some of given arguments are invalid.
		 * @throws mt::ThreadNoMemoryException if no memory.
		 * @throws mt::ThreadCantStartException if other errors was occurred.
		 *
		 */
		virtual void start();

		/// Stop thread.
		virtual void stop();

		/// Wait while thread finished.
		void wait();

		/**
		 * Set the thread's priority.
		 *
		 * @param priority Priority of the thread.
		 */
		bool setPriority(int32_t priority);

		/// Get the thread's priority.
		int32_t getPriority();

		/// Get name of the current thread.
		static std::string name();

		/**
		 * Set name of the current thread.
		 *
		 * @param name The thread's name.
		 */
		static void setName(const std::string& name);

		/**
		 * The method will called in created thread. Client MUST define this method.
		 */
		virtual void run() = 0;

		void setSigSet(const sigset_t &sigSet) {
			m_sigSet = sigSet;
		}

		//@}


	protected:
		enum ConstsValue {
			DEFAULT_STACK_SIZE = 0
		};

	private:
		static void* startable(void* thread);
		void atThreadStart();
		void atThreadEnd();
		bool isRunning();
		pthread_t createThread(void* (*threadFunc)(void*), uint32_t stack_size, void* params);
		void checkTid();

	private:
		pthread_t m_tid;
		const int32_t m_stackSize;
		std::string m_name;
		sigset_t m_sigSet;

		static pthread_key_t m_threadNameKey;
		static const pthread_t m_mainThread;
		static 	void keyDestructor(void* key_value);

	};
}

#endif /* __THREAD__THREAD__H */
