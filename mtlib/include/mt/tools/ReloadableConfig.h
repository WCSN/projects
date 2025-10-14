#ifndef __TOOLS__RELOADABLE_CONFIG__H
#define __TOOLS__RELOADABLE_CONFIG__H

#include <map>

#include <mt/tools/ConfigChecker.h>
#include <mt/common/XmlConfig.h>

namespace mt {
	typedef std::multimap<std::string, mt::XmlConfigPtr> Configs;

	/**
	 * @class ReloadableConfig mt/tools/ReloadableConfig.h
	 * @brief Realize reloadable configuration.
	 *
	 * Configuration file will be loaded and parsed by ReloadableConfig.
	 * To get element's configuration you can use getConfig() and getConfigs().
	 * You should check for file changes by call method needReinitConfiguration(). If will returned TRUE, you should reinitialize all your objects.
	 *
	 * <b>Example: </b>
	 * @code
	 * class Foo {
	 * public:
	 *  Foo() {
	 *    m_config.init("config.xml", 10);
	 *    initFoo();
	 *  }
	 *
 	 *  void initFoo() {
	 *    mt::XmlCondigPtr config = m_configs.getConfig("foo_element");
	 *    //... Initialize Foo with confuguartion.
	 *  }
	 *
	 *  void process() { //The method  periodically called.
	 *    if(m_config.needReinitConfiguration()) {
	 *       initFoo();
	 *    }
	 *    //... Process.
	 *  }
	 * private:
	 *   mt::ReloadableConfig m_config;
	 * };
	 * @endcode
	 */

	class ReloadableConfig {

	public:
		//@{
		ReloadableConfig();
		virtual ~ReloadableConfig();
		//@}
//@{
		/** 
		 * Initialize reloadable config.
		 * 
		 * @param file_name XML configuration file name.
		 * @param timer The interval between file last change time checking. Set to 5 sec by default.
		 */
		void init(const std::string& file_name, uint32_t timer = DEFAULT_TIMER_MS);
		/** 
		 * Get first element from config by name.
		 * 
		 * @param section_name The element's name.
		 * 
		 * @return Smart pointer to xml config. If no such element, will returned null-pointer.
		 */
		mt::XmlConfigPtr getConfig(const std::string& section_name);
		/** 
		 * Get all elements from config by name.
		 * 
		 * @param section_name The element's name.
		 * @param config_list Pointers to xml config will pushed in the list.
		 * 
		 * @return FALSE if no elements with such name. Otherwise, will returned TRUE.
		 */
		bool getConfigs(const std::string& section_name, mt::ConfigList* const config_list);
	
		/// Return TRUE if configuration was changed.
		bool needReinitConfiguration();
		//@}
	private:
		void loadConfig();
		void reload();

	private:
		enum {
			DEFAULT_TIMER_MS = 5000
		};
		mt::XmlConfigPtr m_commonXmlConfig;
		Configs m_configs;
		ConfigCheckerPtr m_configChecker;
	};

	typedef mt::SmartPtr<ReloadableConfig> ReloadableConfigPtr;

}

#endif /* __TOOLS__RELOADABLE_CONFIG__H */
