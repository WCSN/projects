#include <libxml/xpath.h>

#include <mt/common/XmlConfig.h>
#include <mt/common/Exception.h>

#include <string.h>

namespace mt {

	XmlConfig::XmlConfig( const std::string& file_name )
    : m_fileName(file_name), m_xmlContext(NULL), m_parsedDoc(NULL), m_loaded(false), m_lastChangeTime(0) {

		MT_LOGINIT(XmlConfig);
		m_errorBuf = new char[0x10000];
		m_errorBuf[0] = 0;
		init();
		load();
	}

	XmlConfig::XmlConfig( const char* file_content )
    : m_xmlContext(NULL), m_parsedDoc(NULL), m_loaded(false), m_lastChangeTime(0) {

		MT_LOGINIT(XmlConfig);
		m_errorBuf = new char[0x10000];
		m_errorBuf[0] = 0;
		init();
		parse(file_content);
	}

	XmlConfig::XmlConfig( const XmlConfig& xcfg, const std::string& xpath )
		: m_xmlContext(NULL), m_parsedDoc(NULL), m_loaded(false), m_lastChangeTime(0) {

		MT_LOGINIT(XmlConfig);
		m_lastChangeTime = xcfg.m_lastChangeTime;
		m_errorBuf = new char[0x10000];
		m_errorBuf[0] = 0;
		init();
		if (!xcfg.m_parsedDoc) {
			MT_EXCEPTION(mt::XmlException, "Failed to initialize XmlConfig: empty source document");
		}
		xmlXPathContextPtr ctx = xmlXPathNewContext(xcfg.m_parsedDoc);
		if (ctx == NULL) {
			MT_EXCEPTION(mt::XmlException, "Failed to initialize XPath context");
		}
		ctx->node = xcfg.m_parsedDoc->children;
		xmlXPathObjectPtr obj = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(), ctx);
		if (obj == NULL) {
			MT_EXCEPTION(mt::XmlException, "Failed to initialize XmlConfig: Failed to parse XPath " << xpath);
		}
		if (obj->nodesetval &&
				obj->nodesetval->nodeNr >= 1 &&
				obj->nodesetval->nodeTab[0]) {
			m_parsedDoc = xmlNewDoc((const xmlChar*)"1.0");
			// 1 - do a recursive copy (properties, namespaces and children when applicable)
			m_parsedDoc->children = xmlCopyNode(obj->nodesetval->nodeTab[0], 1);
			m_loaded = true;
		} else {
			MT_EXCEPTION(mt::XmlException, "Failed to initialize XmlConfig: no nodes found: xpath " << xpath);
		}
    xmlXPathFreeObject(obj);
    xmlXPathFreeContext(ctx);
	}

	XmlConfig::XmlConfig( const XmlConfig& cfg ) {
		MT_LOGINIT(XmlConfig);
		m_lastChangeTime = cfg.m_lastChangeTime;
		m_errorBuf = new char[0x10000];
		m_errorBuf[0] = 0;
		m_fileName = cfg.m_fileName;
		m_xmlContext = NULL;
		if (cfg.m_parsedDoc) {
			m_parsedDoc = xmlCopyDoc(cfg.m_parsedDoc, 1);
		} else {
			m_parsedDoc = NULL;
		}
		m_loaded = cfg.m_loaded;
	}

	XmlConfig::XmlConfig(const xmlNode* root, time_t last_change_time) : m_xmlContext(NULL), m_parsedDoc(NULL),
																																			 m_loaded(false), m_lastChangeTime(last_change_time){

		MT_LOGINIT(XmlConfig);
		m_errorBuf = new char[0x10000];
		m_errorBuf[0] = 0;
		init();
		m_parsedDoc = xmlNewDoc((const xmlChar*)"1.0");
		// 1 - do a recursive copy (properties, namespaces and children when applicable)
		m_parsedDoc->children = xmlCopyNode((xmlNode*)root, 1);
		m_loaded = true;
	}

	XmlConfig::~XmlConfig() {
		delete[] m_errorBuf;
		if (m_xmlContext) {
			xmlFreeParserCtxt(m_xmlContext);
		}
		if (m_parsedDoc) {
			xmlFreeDoc(m_parsedDoc);
		}
	}

	void XmlConfig::reload( const char* new_file_name ) {
		if( 0 != new_file_name ){
			m_fileName = new_file_name;
		}
		load();
	}

	void XmlConfig::init() {
		m_xmlContext = xmlNewParserCtxt();
		if (m_xmlContext == NULL) {
			MT_EXCEPTION(mt::XmlException, "Failed to initialize XML parser context");
		}
	}

	void XmlConfig::xmlErrorHandler(void *ctxt, const char* msg, ...) {
		MT_LOGINIT_STATIC(XmlConfig);
		XmlConfig* xcfg = (XmlConfig*)ctxt;
		char* buf = xcfg->m_errorBuf;
		va_list args;
		va_start(args, msg);
		vsprintf(&buf[strlen(buf)], msg, args);

		std::string sbuf(buf);
		std::string::size_type pos;
		if ((pos = sbuf.find(std::string("\n"))) != std::string::npos) {
			sbuf = sbuf.erase(pos, 1);
			MT_LOGWARNING(sbuf);
			buf[0] = 0;
		}
		va_end(args);
	}

	void XmlConfig::load(){
		xmlSetGenericErrorFunc(this, xmlErrorHandler);
		m_parsedDoc = xmlCtxtReadFile(m_xmlContext, m_fileName.c_str(),
																	NULL, XML_PARSE_RECOVER);
		if (m_parsedDoc == NULL) {
			MT_LOGERROR("Failed to parse XML from " << m_fileName);
			m_loaded = false;
			return;
		}
		setModificationTime();

		m_loaded = true;
		return;
	}

	void XmlConfig::parse(const char* content) {
		MT_LOGINFO("Getting configuration from a string");
		xmlSetGenericErrorFunc(this, xmlErrorHandler);
		m_parsedDoc = xmlCtxtReadDoc(m_xmlContext, (const xmlChar*)content,
																 "", NULL, XML_PARSE_RECOVER);
		if (m_parsedDoc == NULL) {
			MT_LOGERROR("Failed to parse XML from " << m_fileName);
			m_loaded = false;
			return;
		}
		MT_LOGINFO("Configuration successfully loaded");

		m_loaded = true;
		return;
	}

	std::string XmlConfig::get(const std::string& a, const std::string& b) const {
		std::string value;
		std::string xpath;
		if(a=="") xpath="@"+b;
		if(b=="") xpath=a;
		if(a!="" && b!="") xpath=a+"/@"+b;
		if(a=="" && b=="") {
			MT_EXCEPTION(mt::XmlException, "Failed to get attribute '" << b << "' from section '" << a << "'");
		}

		getValue(xpath, value);
		return value;
	}

	std::string XmlConfig::get(const std::string& b) const {
		return get("", b);
	}

	std::string XmlConfig::getMandatory(const std::string& b) const {
		std::string value = get("", b);
		if(value.empty())
			MT_EXCEPTION(mt::XmlExceptionNoMandatoryParamter, "No parameter '" << b << "'");
		return value;
	}

	void XmlConfig::put(const std::string& a, const std::string& b,  const std::string& c) {
		std::string xpath;

		if(a.empty())
			xpath="@"+b;

		if(b.empty())
			xpath = a;

		if(!a.empty() && !b.empty())
			xpath = a + "/@" + b;

		if(a.empty() && b.empty())
			MT_EXCEPTION(mt::XmlException, "Failed to set attribute '" << b << "' from section '" << a << "'");

		setValue(xpath, c);
	}

	void XmlConfig::put(const std::string& b, const std::string& c) {
		put("", b, c);
	}

	void XmlConfig::readFromFile(const std::string& filename) {
		m_fileName = filename;
		load();
	}

	int32_t XmlConfig::getValue( const char* xpath, std::string& value ) const {
		return getValue(std::string(xpath), value);
	}

	int32_t XmlConfig::getValue(const std::string& xpath, std::string& value ) const {
		if (!m_parsedDoc) {
			MT_EXCEPTION(mt::XmlException, "Failed to get value: empty document");
		}
		xmlXPathContextPtr ctx = xmlXPathNewContext(m_parsedDoc);
		if(!ctx){
			MT_EXCEPTION(mt::XmlException, "Failed to initialize XPath context");
		}
		ctx->node = m_parsedDoc->children;
		xmlXPathObjectPtr obj = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(), ctx);
		if(!obj) {
			MT_EXCEPTION(mt::XmlException, "Failed to parse XPath " << xpath);
		}
		if (obj->nodesetval &&
				obj->nodesetval->nodeNr >= 1 &&
				obj->nodesetval->nodeTab[0] &&
				obj->nodesetval->nodeTab[0]->type == XML_ATTRIBUTE_NODE &&
				obj->nodesetval->nodeTab[0]->children) {
			value = std::string((const char*)obj->nodesetval->nodeTab[0]->children->content);
		}

		xmlXPathFreeObject(obj);
		xmlXPathFreeContext(ctx);
		return 0;
	}

	int32_t XmlConfig::setValue( const std::string& xpath, const std::string& value ) {
		if (!m_parsedDoc) {
			MT_EXCEPTION(mt::XmlException, "Failed to get value: empty document");
		}
		xmlXPathContextPtr ctx = xmlXPathNewContext(m_parsedDoc);
		if (ctx == NULL) {
			MT_EXCEPTION(mt::XmlException, "Failed to initialize XPath context");
		}
		ctx->node = m_parsedDoc->children;
		xmlXPathObjectPtr obj = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(), ctx);
		if (obj == NULL) {
			MT_EXCEPTION(mt::XmlException, "Failed to parse XPath " << xpath);
		}
		if (obj->nodesetval &&
				obj->nodesetval->nodeNr >= 1 &&
				obj->nodesetval->nodeTab[0] &&
				obj->nodesetval->nodeTab[0]->type == XML_ATTRIBUTE_NODE &&
				obj->nodesetval->nodeTab[0]->children) {
			xmlChar* val = new xmlChar(value.length());
			strcpy((char*)val, value.c_str());
			obj->nodesetval->nodeTab[0]->children->content = val;
		} else {
			MT_LOGDEBUG("Failed to set string value");
		}
    xmlXPathFreeObject(obj);
    xmlXPathFreeContext(ctx);
		return 0;
	}

	ConfigList XmlConfig::getConfigs( const std::string xpath) const {
		ConfigList cl;

		if (!m_parsedDoc) {
			MT_EXCEPTION(mt::XmlException, "Failed to get configs: empty document");
		}
		xmlXPathContextPtr ctx = xmlXPathNewContext(m_parsedDoc);
		if (ctx == NULL) {
			MT_EXCEPTION(mt::XmlException, "Failed to initialize XPath context");
		}
		ctx->node = m_parsedDoc->children;
		xmlXPathObjectPtr obj = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(), ctx);
		if (obj == NULL) {
			MT_EXCEPTION(mt::XmlException, "Failed to parse XPath " << xpath);
		}
		if (obj->nodesetval) {
			for (int32_t i = 0; i < obj->nodesetval->nodeNr; i++) {
				cl.push_back(new XmlConfig(obj->nodesetval->nodeTab[i], m_lastChangeTime));
			}
		}
		xmlXPathFreeObject(obj);
		xmlXPathFreeContext(ctx);
		return cl;
	}

	XmlConfigPtr  XmlConfig::getConfig( const std::string xpath) const {
		ConfigList configs = getConfigs(xpath);
		return (configs.empty()) ? XmlConfigPtr() : configs.front();
	}

	XmlConfig& XmlConfig::operator=( const XmlConfig& cfg ) {
		*this = cfg;
		return *this;
	}

	std::string XmlConfig::getName() const {
		if (m_parsedDoc && m_parsedDoc->children) {
			return std::string((char*)m_parsedDoc->children->name);
		} else {
			MT_EXCEPTION(mt::XmlException, "Empty XML document.");
		}
		return std::string();
	}

	void XmlConfig::setModificationTime() {
		//Getting file's modification time
		struct stat st;
		stat(m_fileName.c_str(), &st);
		m_lastChangeTime = st.st_mtime;
	}
} //namespace mt
