#ifndef __UTILS__SYSTEM__H
#define __UTILS__SYSTEM__H

#include <stdint.h>
#include <sys/types.h>

namespace mt {
	/**
	 * @class System mt/utils/System.h
	 * @brief System utilities. 
	 */

	class System {

	public:	
		/** 
		 * Sleep.
		 * 
		 * @param ms Timeout in milliseconds.
		 */
		static void sleep(uint32_t ms);
		
		/**
		 * Returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.
		 */
		static time_t currentTime();
		
		/// Return current thread's id.
		static int32_t getTid();
		
		/// Return current process's id.
		static 	int32_t getPid();
	};
}


#endif /* __UTILS__SYSTEM__H */
