#ifndef __THREAD__INFINITE_THREAD__H
#define __THREAD__INFINITE_THREAD__H

#include <mt/thread/Thread.h>
#include <mt/thread/AutoMutex.h>

namespace mt {
	/**
	 * @class InfiniteThread mt/thread/InfiniteThread.h
	 * @brief The class create thread and start code in infinite loop in this thread.
	 * <b>Declaration:</b>\n
	 * @code
	 * class InOtherThread : public mt::InfiniteThread {
	 *   void runLoop() {
	 *     //This code will work in infinite loop in other thread.
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

	class InfiniteThread : public mt::Thread {

	public:
		//@{
		/** 
		 * @param timeout The pause in milliseconds between executing @a runLoop(). @a timeout set to 0 by default.
		 */
		InfiniteThread(uint32_t timeout = 0);

		/** 
		 * @param name Name of the thread.
		 * @param stackSize Stack size of the thread.
		 * @param timeout The pause in milliseconds between executing @b runLoop(). @a timeout set to 0 by default.
		 */
		InfiniteThread(const std::string& name, uint32_t stackSize = DEFAULT_STACK_SIZE, uint32_t timeout = 0);
		virtual ~InfiniteThread();
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
		//@}
	protected:
		/** 
		 * The method will called in infinite loop created thread. Client MUST define this method. 
		 */
		virtual void runLoop() = 0;

	private:	
		void run();
		bool active();

	private:
		mt::Mutex m_activeMutex;
		bool m_active;
		uint32_t m_timeout;
	};

}
#endif /* __THREAD__INFINITE_THREAD__H */
