#include <mt/common/Regex.h>
#include <mt/common/Exception.h>

namespace mt {

	Regex::Regex(const std::string& regex_mask) {
		if(regcomp(&m_regex, regex_mask.c_str(), REG_EXTENDED))
			MT_EXCEPTION(mt::Exception, "An error occurred at creation regexp for mask. Invalid value of regular expression '" << regex_mask << "'.");
	}

	Regex::~Regex() {
		regfree(&m_regex);
	}

	bool Regex::match(const std::string& value) {
		return !regexec(&m_regex, value.c_str(), 0, 0, 0);
	}
}
