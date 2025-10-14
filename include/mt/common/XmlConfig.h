#ifndef __COMMON__XML_CONFIG__H
#define __COMMON__XML_CONFIG__H

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string>
#include <vector>
#include <stdint.h>

#include <mt/common/SmartPtr.h>
#include <mt/logger/Logable.h>

namespace mt {
	/*!
	 * \class XmlConfig mt/common/XmlConfig.h
	 * \brief XML files parser.
	 */

	class XmlConfig;
	typedef mt::SmartPtr<XmlConfig> XmlConfigPtr;
	typedef std::vector<XmlConfigPtr> ConfigList;

	class XmlConfig : public Logable {

	public:
		//@{
		/**
		 * Load XML from file.
		 * @param file_name File to load.
		 */
		XmlConfig( const std::string& file_name );

		/**
		 * Load XML's element from XML config.
		 * @param xcfg XML config.
		 * @param xpath The element's name.
		 *
		 */
		XmlConfig( const XmlConfig& xcfg, const std::string& xpath );

		/**
		 * Load XML from string.
		 * @param file_content XML string.
		 */
		XmlConfig( const char* file_content );

		/// Copy constructor.
		XmlConfig( const XmlConfig& cfg );
		virtual ~XmlConfig();
		//@}

		//@{
		/**
		 * Load all element's from the XML config.
		 *
		 * @param xpath The element's path.
		 *
		 * @return List of element's.
		 */
		ConfigList getConfigs( const std::string xpath) const;

		/**
		 * Load first element from the XML config.
		 * 
		 * @param xpath The element's path.
		 * 
		 * @return Pointer to element. NULL pointer will be returned if no such element.
		 */
		XmlConfigPtr getConfig(const std::string xpath) const;	

		/** 
		 * Name of the element.
		 * @return Name of the config's root element .
		 */

		std::string getName() const;

		/**
		 * Get value of the parameter from the element.
		 *
		 * @param a The element's name.
		 * @param b The parameter's name.
		 *
		 * @throws mt::XmlException if the document is empty.
		 * @throws mt::XmlException if error occurred at initializing the element context.
		 * @throws mt::XmlException if error occurred at parsing the element.
		 * @return Value of the parameter.
		 */
		std::string get(const std::string& a, const std::string& b) const ;

		/**
		 * Get value of the parameter from the XML config's root element.
		 *
		 * @param b The parameter's name.
		 *
		 * @throws mt::XmlException if the document is empty.
		 * @throws mt::XmlException if error occurred at initializing the element context.
		 * @throws mt::XmlException if error occurred at parsing the element.
		 * @return Value of the parameter.
		 */
		std::string get(const std::string& b) const;

		/**
		 * Get value of the parameter from the XML config's root element. If no such parameter or the parameter's value is empty than throws exception.
		 *
		 * @param b The parameter's name.
		 *
		 * @throws mt::XmlException if the document is empty.
		 * @throws mt::XmlException if error occurred at initializing the element context.
		 * @throws mt::XmlException if error occurred at parsing the element.
		 * @throws mt::XmlExceptionNoMandatoryParamter if no such parameter.
		 * @return Value of the parameter.
		 */
		std::string getMandatory(const std::string& b) const;

		/**
		 * Set value of the parameter from the element.
		 *
		 * @param a The element's name.
		 * @param b The parameter's name.
		 * @param c Putted value of the parameter.
		 * @throws mt::XmlException if the document is empty.
		 * @throws mt::XmlException if error occurred at initializing the element context.
		 * @throws mt::XmlException if error occurred at parsing the element.
		 *
		 */
		void put(const std::string& a, const std::string& b, const std::string& c);

		/**
		 * Set value of the parameter from the XML config's root element.
		 *
		 * @param b The parameter's name.
		 * @param c Putted value of the parameter.
		 * @throws mt::XmlException if the document is empty.
		 * @throws mt::XmlException if error occurred at initializing the element context.
		 * @throws mt::XmlException if error occurred at parsing the element.
		 *
		 */
		void put(const std::string& b, const std::string& c);

		/**
		 * Load XML config from file.
		 *
		 * @param filename File's name.
		 */
		void readFromFile(const std::string& filename);

		/**
		 * Reload XML config. If new_file_name not 0 than XML config will loaded from file 'new_file_name'
		 *
		 * @param new_file_name File's name.
		 */
		void reload( const char* new_file_name = 0);

		/**
		 * Get configuration file's last changing time.
		 *
		 * @return Time in seconds since the Epoch.
		 */
		time_t lastChangeTime() const;
		XmlConfig& operator=(const XmlConfig& cfg);
		//@}
protected:
		/**
		 * Get parameter's value from XML.
		 *
		 * @param xpath The element's path.
		 * @param value The parameter's value was set to it.
		 *
		 * @throws mt::XmlException if the document is empty.
		 * @throws mt::XmlException if error occurred at initializing the element context.
		 * @throws mt::XmlException if error occurred at parsing the element.
		 * @return 0 if success.
		 */
		int32_t getValue( const char* xpath, std::string& value ) const;

		/**
		 * Get parameter's value from XML.
		 *
		 * @param xpath The element's name.
		 * @param value The parameter's value was set to it.
		 *
		 * @throws mt::XmlException if the document is empty.
		 * @throws mt::XmlException if error occurred at initializing the element context.
		 * @throws mt::XmlException if error occurred at parsing the element.
		 * @return 0 if success.
		 */
		int32_t getValue( const std::string& xpath, std::string& value ) const;

		/**
		 * Get parameter's value from the element.
		 *
		 * @param value The parameter's value was set to it.
		 *
		 * @return 0 if success.
		 */
		int32_t getValue( std::string& value ) const;

		/**
		 *
		 *
		 * @param xpath The element's path.
		 * @param value Putted value if the parameter.
		 *
		 * @return 0 if success.
		 */
		int32_t setValue( const std::string& xpath, const std::string& value );

	private:
		void load();
		void init();
		void parse( const char* content );
		void setModificationTime();

		static void xmlErrorHandler(void *ctxt, const char* msg, ...);
		XmlConfig(const xmlNode* root, time_t last_change_time = 0);

	private:
		std::string m_fileName;
		xmlParserCtxtPtr m_xmlContext;
		xmlDocPtr m_parsedDoc;

		bool m_loaded;
		char* m_errorBuf;
		time_t m_lastChangeTime;
	};

	inline time_t XmlConfig::lastChangeTime() const {
		return m_lastChangeTime;
	}
} //namespace mt
#endif /* __COMMON__XML_CONFIG__H */

/*  LocalWords:  Seted
 */
