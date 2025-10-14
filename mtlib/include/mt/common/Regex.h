#ifndef __COMMON__REGEX__H
#define __COMMON__REGEX__H

#include <regex.h>
#include <string>
#include <mt/common/SmartPtr.h>

namespace mt {
	/*!
	 *  \class Regex
	 *  \brief Shell class for regexp.
	 */
	class Regex {

	public:
		//@{
		/** 
		 * @param regex_mask Regexp mask.
		 */
		Regex(const std::string& regex_mask);
		virtual ~Regex();
		//@}
		//@{
		/** 
		 * Match value.
		 * 
		 * @param value The value for match.
		 * 
		 * @return Return TRUE if value matched. Otherwise, will return FALSE.
		 */
		bool match(const std::string& value);
		//@}

	private:
		regex_t m_regex;
	};

	typedef SmartPtr<Regex> RegexPtr;
}

#endif /* __COMMON__REGEX__H */
