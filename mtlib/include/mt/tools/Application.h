#ifndef __TOOLS__APPLICATION__H
#define __TOOLS__APPLICATION__H

#include <stdint.h>

namespace mt {
	/**
	 * @class Application mt/tools/Application.h
	 * @brief Abstract base class for mt applications.
	 */

	class Application {

	public:
		/// Put command's line arguments to constructor.
		Application(int argc, char* argv[]);
		virtual ~Application();
		
		/// Start application.
		void start();
		
		/// Stop application.
		void stop();

	protected:
		/// Initialize application after read all command line's arguments.
		virtual void initialize() = 0;
		
		/// This method will called in inplicit loop.
		virtual void process() = 0;

		/**
		 * This method will be called if command line's arguments are incorrect. 
		 * @param opt number of incorrect parameter.
		*/
		virtual void usage(uint32_t opt) = 0;
		
		/// Read command line's arguments.
		virtual int32_t readArguments(int argc, char* argv[]) = 0;

	private:
		bool m_active;
		int m_argc;
		char** m_argv;
	};

	inline void Application::stop() {
		m_active = false;
	}
}

#endif /* __TOOLS__APPLICATION__H */
