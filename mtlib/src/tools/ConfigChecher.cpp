#include <mt/thread/AutoMutex.h>
#include <mt/utils/System.h>
#include <mt/tools/ConfigChecker.h>

namespace mt {

	ConfigChecker::ConfigChecker(const mt::XmlConfigPtr& config, 
															 time_t last_change_time, 
															 uint32_t timer) : m_config(config),
															 m_lastChangeTime(last_change_time),
															 m_configReloaded(false),
															 m_timer(timer) {
	}

	ConfigChecker::~ConfigChecker() {
	}

	bool ConfigChecker::configWasReloaded() {
		mt::AutoMutex amtx(m_mutex);
		bool need_reinit = m_configReloaded;
		m_configReloaded = false;
		return need_reinit;
	}

	void ConfigChecker::runLoop() {
		mt::System::sleep(m_timer);
		m_config->reload();
		if(m_config->lastChangeTime() != m_lastChangeTime) {
			m_configReloaded = true;
			m_lastChangeTime = m_config->lastChangeTime();
		}
	}

}
