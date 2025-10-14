/*
 * Title       : StrUtils.h
 * Date        : 09.07.2010
 * Author      : Andrei Borodaenko
 * Description :
 */

#ifndef STRUTILS_H
#define STRUTILS_H

#include <stdint.h>

#include <string>
#include <vector>

namespace mt {

/**
 * \class StrUtils
 *
 * \brief Miscellaneous functions for string processing
 */
class StrUtils {

public:
    /**
     * Separate string \a subject into tokens using each symbol
     * from \a delimeters as delimiter. Result tokens are
     * <b>appended</b> to \a out.
     */
    static void tokenize(const std::string& subject, const std::string& delimiters,
                         std::vector<std::string>& out);

		/**
		 * Split tokens to string and use @a delimiters as delimiter.
		 */
    static std::string detokenize(const std::vector<std::string>& in, const std::string& delimiters);

		/**
		 * Convert std::string to IP address as integer.
		 *
		 * @param ip_str IP address as string.
		 *
		 * @throws mt::Exception if ip_str isn't IP in string format.
		 * @return IP address as unsigned integer.
		 */
		static uint32_t stringToIp(const std::string& ip_str);

		/**
		 * Convert IP address as integer to std::string.
		 *
		 * @param ip IP address as integer.
		 *
		 * @return IP address as string.
		 */
		static std::string ipToString(uint32_t ip);

		/** 
		 * Convert integer to string.
		 * 
		 * @param i Integer for convert.
		 * 
		 * @return Integer as string.
		 */
		static std::string intToString(int32_t i);
		
		/** 
		 * Convert binary data to HEX string.
		 * 
		 * @param data Binary data.
		 * @param len Length of the binary data.
		 * 
		 * @return Binary as HEX string.
		 */
		static std::string binaryView(const uint8_t* data, uint32_t len);
};

} // namespace mt

#endif /* STRUTILS_H */
