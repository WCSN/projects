/*
 * Title       : StrUtils.cpp
 * Date        : 09.07.2010
 * Author      : Andrei Borodaenko
 * Description : ***
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sstream>
#include <iterator>
#include <algorithm>

#include <mt/utils/StrUtils.h>
#include <mt/common/Exception.h>

using std::string;
using std::vector;

namespace mt {

	void StrUtils::tokenize(const std::string& subject, const std::string& delimeters,
													std::vector<std::string>& out) {
    const char* delims = delimeters.c_str();
		std::string::size_type begin = subject.find_first_not_of(delims);
		std::string::size_type end = subject.find_first_of(delims, begin);
    while (true) {
			if (begin != string::npos) {
				if (end != string::npos) {
					out.push_back(subject.substr(begin, end - begin));
					begin = subject.find_first_not_of(delims, end);
					end = subject.find_first_of(delims, begin);
				} else {
					out.push_back(subject.substr(begin, subject.length() - begin));
					begin = subject.find_first_not_of(delims, end);
				}
			} else {
				break;
			}
    }
	}

	std::string StrUtils::detokenize(const std::vector<std::string>& in, const std::string& delimiters) {
		if(in.empty())
			return std::string();
		
		std::ostringstream os;
		std::copy(in.begin (), in.end() - 1, 
							std::ostream_iterator<std::string>(os, delimiters.c_str()));
		return os.str () + *(in.rbegin());
	}

	uint32_t StrUtils::stringToIp(const std::string& ip_str) {
		int32_t oct1, oct2, oct3, oct4;
		uint32_t ip = 0;
		if( 4!=sscanf( ip_str.c_str(), "%d.%d.%d.%d", &oct1, &oct2, &oct3, &oct4 ) ||
				oct1>255 || oct2>255 || oct3>255 || oct4>255 ||
				oct1<0   || oct2<0   || oct3<0   || oct4<0   )
		{
			MT_EXCEPTION(mt::Exception, "Failed to get IP address from string '" << ip_str << "'");
		}
		ip =  ((oct1 << 24)  & 0xFF000000) |
			 ((oct2 << 16) & 0x00FF0000) |
			 ((oct3 << 8)   & 0x0000FF00) |
			oct4;
		return ip;
	}

	std::string StrUtils::ipToString(uint32_t ip) {
		char buf[ 16 ];
		sprintf( buf,"%d.%d.%d.%d",
						 (ip & 0xFF000000) >> 24,
						 (ip & 0x00FF0000) >> 16,
						 (ip & 0x0000FF00) >> 8,
						 (ip & 0x000000FF));
		return buf;
	}

	std::string StrUtils::intToString(int32_t i) {
		char buf[16];
		sprintf(buf, "%d", i);
		return buf;
	}

	std::string StrUtils::binaryView(const uint8_t* data, uint32_t len) {
		std::string binStr("0x");
		char byteChars[10];
		for(uint32_t offset = 0; offset < len; offset++){
			sprintf(byteChars,"00%02X",(uint32_t)*(data + offset));
			binStr+= byteChars + strlen(byteChars)-2;
		}
		return binStr;
	}
}

