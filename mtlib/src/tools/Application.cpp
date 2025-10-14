#include <mt/tools/Application.h>

namespace mt {

	Application::Application(int argc, char* argv[]) : m_active(false),
																										 m_argc(argc),
																										 m_argv(argv){
	}

	Application::~Application() {
	}
	
	void Application::start() {
		if(int32_t ret = readArguments(m_argc, m_argv) != -1) {
			usage(ret);
			return;
		}
		initialize();
		m_active = true;
		while(m_active) {
			process();
		}
	}

}
