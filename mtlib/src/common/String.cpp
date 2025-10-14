#include "mt/common/String.h"

namespace mt {
	const size_t String::npos = std::string::npos;

	bool operator==(const char* a, const String& b) {
		return (a == b.m_data);
	}

	String operator + (const String& a, const String& b) {
		String ret;
		ret += a;
		ret += b;
		return ret;
	}

	String operator + (const char* str, const String& mt_str) {
		String ret;
		ret += str;
		ret += mt_str;
		return ret;
	}

	bool operator!=(const char* a, const String& b) {
		return (a != b.m_data);
	}

	std::ostream& operator <<(std::ostream& stream, const String& str) {
		stream << str.m_data;
		return stream;
	}

	bool operator < (const String& a, const String& b) {
		return (a.m_data < b.m_data);
	}

}
