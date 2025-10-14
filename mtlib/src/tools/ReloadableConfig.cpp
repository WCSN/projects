#include <mt/tools/ReloadableConfig.h>

namespace mt {

	ReloadableConfig::ReloadableConfig() {
	}

	ReloadableConfig::~ReloadableConfig() {
		if(m_configChecker)
			m_configChecker->stop();
	}

	void ReloadableConfig::init(const std::string& file_name, uint32_t timer) {
		m_commonXmlConfig = new mt::XmlConfig(file_name);
		loadConfig();
		m_configChecker = new ConfigChecker(m_commonXmlConfig, m_commonXmlConfig->lastChangeTime(), timer);
		m_configChecker->start();
	}

	mt::XmlConfigPtr ReloadableConfig::getConfig(const std::string& section_name) {
		Configs::iterator it = m_configs.find(section_name);
		if(it == m_configs.end())
			return mt::XmlConfigPtr();
		return it->second;
	}

	bool ReloadableConfig::getConfigs(const std::string& section_name, mt::ConfigList* const config_list) {
		MT_EXCEPTION_ASSERT(config_list, "No config list.");
	
		std::pair<Configs::iterator, Configs::iterator> equal_range = m_configs.equal_range(section_name);
		for(Configs::iterator it = equal_range.first; it != equal_range.second; it++)
			config_list->push_back(it->second);
		return (!config_list->empty());
	}

	void ReloadableConfig::loadConfig() {
		mt::ConfigList sections = m_commonXmlConfig->getConfigs("*");
		for(mt::ConfigList::iterator it = sections.begin(); it != sections.end(); it++) {
			m_configs.insert(std::make_pair((*it)->getName(), *it));
		}
	}

	bool ReloadableConfig::needReinitConfiguration() {
		if(m_configChecker->configWasReloaded()) {
			reload();
			return true;
		}
		return false;
	}

	void ReloadableConfig::reload() {
		m_configs.clear();
		loadConfig();
	}

}
