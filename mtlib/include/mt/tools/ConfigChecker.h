#ifndef __TOOLS__CONFIG_CHECKER__H
#define __TOOLS__CONFIG_CHECKER__H

#include <mt/thread/InfiniteThread.h>
#include <mt/common/XmlConfig.h>
#include <mt/thread/Mutex.h>
#include <mt/common/SmartPtr.h>

namespace mt {

	/**
	 * @class ConfigChecker mt/tools/ConfigChecker.h
	 * @brief File change time checker.
	 */

	class ConfigChecker : public mt::InfiniteThread {

	public:
		ConfigChecker(const mt::XmlConfigPtr& config, time_t last_change_time, uint32_t timer);
		virtual ~ConfigChecker();

		bool configWasReloaded();

	protected:
		void runLoop();

	private:
		mt::XmlConfigPtr m_config;
		time_t m_lastChangeTime;
		bool m_configReloaded;
		mt::Mutex m_mutex;
		const uint32_t m_timer;
	};

	typedef mt::SmartPtr<ConfigChecker> ConfigCheckerPtr;

}
#endif /* __TOOLS__CONFIG_CHECKER__H */
