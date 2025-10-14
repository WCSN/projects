#ifndef STRING__H
#define STRING__H

#include <string>

namespace mt {
	/*!
	 * @class String mt/common/String.h
	 * @brief std::string wrapper. 
	 *
	 * Use this string if you have problem with operating memory fragmentation. This string allocate memory by 32 bytes blocks.
	 */
	class String {
	public:
//@{
		String();
		/**
		 * Content is initialized to a copy of the string formed by the null-terminated character sequence (C string) pointed by s. 
		 * The length of the caracter sequence is determined by the first occurrence of a null character (as determined by traits.length(s)).
		 */
		String(const char* str);

		/// Content is initialized to a copy of the string formed by the first n characters in the array of characters pointed by s.
		String(const char* str, size_t n);

		/// Content is initialized as a string formed by a repetition of character c, n times.
		String(size_t n, char ch);

		/// Content is initialized to a copy of a substring of str. The substring is the portion of str that begins at the character position pos and takes up to n characters (it takes less than n if the end of str is reached before).
		String(const String& str, size_t pos, size_t n = String::npos);
//@}
//@{
		/// Generates a null-terminated sequence of characters (c-string) with the same content as the string object and returns it as a pointer to an array of characters.
		const char* c_str() const {
			return m_data.c_str();
		}
	
		/// Returns a pointer to an array of characters with the same content as the string.
		const char* data() const {
			return m_data.data();
		}

		/// Returns a count of the number of characters in the string.
		size_t length() const {return m_data.length();}

		/// Returns a count of the number of characters in the string.
		size_t size() const {return m_data.size();}

		/// Returns true if the string size is 0, false otherwise.
		bool empty() const {return m_data.empty();}

		bool operator == (const char* str) const;
		bool operator == (const String& str) const;
		bool operator != (const String& str) const;

		/// Appends a copy of the argument to the string.
		String& operator+=(char ch);

		/// Appends a copy of the argument to the string.
		String& operator+=(const char* str);

		/// Appends a copy of the argument to the string.
		String& operator+=(const String& str);
	
		/** 
		 * Searches the string for the content specified in find_str and returns the position of the first occurrence in the string.
		 * @param pos Position of the first character in the string to be taken into consideration for possible matches. A value of 0 means that the entire string is considered.
		 * @return The position of the first occurrence in the string of the searched content. If the content is not found, the member value npos is returned.
		 **/
		size_t find(const char* find_str, size_t pos = 0) const;

		/** 
		 * Searches the string for the content specified in ch and returns the position of the first occurrence in the string.
		 * @param pos Position of the first character in the string to be taken into consideration for possible matches. A value of 0 means that the entire string is considered.
		 * @return The position of the first occurrence in the string of the searched content. If the content is not found, the member value npos is returned.
		 **/
		size_t find(char ch, size_t pos = 0) const;

		/** 
		 * Searches the string for the content specified in str and returns the position of the first occurrence in the string.
		 * @param pos Position of the first character in the string to be taken into consideration for possible matches. A value of 0 means that the entire string is considered.
		 * @return The position of the first occurrence in the string of the searched content. If the content is not found, the member value npos is returned.
		 **/
		size_t find(const String& str, size_t pos = 0) const;

		/**
		 * Returns a string object with its contents initialized to a substring of the current object.
		 * This substring is the character sequence that starts at character position pos and has a length of n characters.
		 */
		String substr(size_t pos = 0, size_t n = String::npos) const;

		/** 
		 * Replaces a section of the current string by some other content determined by the arguments passed.
		 * 
		 * @param pos Position within the string of the first character of the section to be replaced. Notice that the first position has a value of 0, not 1. If the position passed is past the end of the string, an out_of_range exception is thrown.
		 * @param n Length of the section to be replaced within the string. If this is longer than the actual length of the string, the function replaces the entire section between pos and the end of the string.
		 * @param str Array with a sequence of characters. 
		 * 
		 * @return *this
		 */
		String& replace(size_t pos, size_t n, const char* str);

		/** 
		 * Replaces a section of the current string by some other content determined by the arguments passed.
		 * 
		 * @param pos Position within the string of the first character of the section to be replaced. Notice that the first position has a value of 0, not 1. If the position passed is past the end of the string, an out_of_range exception is thrown.
		 * @param n Length of the section to be replaced within the string. If this is longer than the actual length of the string, the function replaces the entire section between pos and the end of the string.
		 * @param str Another object of class mt::String whose content is entirely or partially used as the replacement.
		 * 
		 * @return *this
		 */
		String& replace(size_t pos, size_t n, const String& str);

		/// Erases a sequence of n characters starting at position pos.
		String& erase(size_t pos, size_t n);

		/// The function deletes everything from position pos forwards.
		String& erase(size_t pos);

		/// The function deletes the entire string, like member clear.
		String& erase();

		/// Inserts a copy of the entire string object str at character position pos1.
		String& insert(size_t pos, const String& str);

		/// Inserts a string formed by a repetition of character ch, n times, at the character position pos.
		String& insert(size_t pos, size_t n, char ch);

		/// Sets as the new content a copy of the string formed by the first n characters of the array pointed by str.
		String& assign(const char* str, size_t n);
	
		/// Appends a copy of str.
		String& append(const String& str);

		/** Appends a copy of a substring of str. The substring is the portion of str that begins at the character position pos and takes up to n characters (it takes less than n if the end of string is reached before). 
		 * If the position passed is past the end of str, an std::out_of_range exception is thrown. 
		 **/
		String& append(const String& str, size_t pos, size_t n);

		/**
		 * Compares the content of this object to the content of a comparing string str.
		 * @return 0 if the compared characters sequences are equal, otherwise a number different from 0 is returned, with its sign indicating whether the object is considered greater than the comparing string passed as parameter (positive sign), or smaller (negative sign).
		 **/
		int32_t compare(const String& str) const;

		/// Returns a reference the character at position pos in the string.
		const char& operator[] ( size_t pos ) const;

		/// Returns a reference the character at position pos in the string.
		char& operator[] ( size_t pos );

		/// The string content is set to an empty string, erasing any previous content and thus leaving its size at 0 characters.
		void clear();
		
		/**
		 * Change content of the string to upper register.
		 * @return Count of changed characters.
		 **/
		size_t toUpper();

		friend String operator + (const String& a, const String& b);
		friend bool operator==(const char* a, const String& b);
		friend bool operator!=(const char* a, const String& b);
		friend std::ostream& operator<<(std::ostream& stream, const String& str);
		friend bool operator<(const String& a, const String& b);
//@}
	public:
		static const size_t npos;

	private:
		size_t getCapacity(size_t string_size);
		size_t createCapacity(size_t new_size);
		void checkCapacity(size_t appended_string_size);

	private:
		enum {
			DEFAULT_CAPACITY = 32
		};
		std::string m_data;
	};

	bool operator==(const char* a, const String& b);
	bool operator!=(const char* a, const String& b);
	String operator + (const String& a, const String& b);
	String operator + (const char* str, const String& mt_str);
	std::ostream& operator<<(std::ostream& stream, const String& str);
	bool operator<(const String& a, const String& b);

	inline String::String() {
		m_data.reserve(DEFAULT_CAPACITY);
	}

	inline String::String(const char* str) {
		m_data.reserve(getCapacity(strlen(str) + 1));
		m_data += str;
	}

	inline size_t String::getCapacity(size_t string_size) {
		return (string_size < DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : createCapacity(string_size);
	}

	inline size_t String::createCapacity(size_t new_size) {
		return (new_size / DEFAULT_CAPACITY + 1) * DEFAULT_CAPACITY;
	}

	inline String::String(const char* str, size_t n) {
		m_data.reserve(getCapacity(n + 1));
		m_data += std::string(str, n);
	}

	inline String::String(size_t n, char ch) {
		m_data.reserve(getCapacity(n + 1));
		m_data += std::string(n, ch);
	}

	inline String::String(const String& str, size_t pos, size_t n) {
		m_data.reserve(getCapacity(n + 1));
		m_data += std::string(str.m_data, pos, n);
	}

	inline bool String::operator==(const char* str) const {
		return (m_data == str);
	}

	inline bool String::operator == (const String& str) const {
		return (m_data == str.m_data);
	}

	inline bool String::operator != (const String& str) const {
		return (m_data != str.m_data);
	}

	inline String& String::operator+=(char ch) {
		checkCapacity(1);
		m_data += ch;
		return *this;
	}

	inline void String::checkCapacity(size_t appended_string_size) {
		size_t new_size = appended_string_size + m_data.length();
		if(new_size > m_data.capacity())
			m_data.reserve(createCapacity(new_size));
	}

	inline String& String::operator+=(const char* str) {
		checkCapacity(strlen(str));
		m_data += str;
		return *this;
	}

	inline String& String::operator+=(const String& str) {
		checkCapacity(str.length());
		m_data += str.m_data;
		return *this;
	}

	inline size_t String::find(const char* find_str, size_t pos) const {
		return m_data.find(find_str, pos);
	}

	inline size_t String::find(char ch, size_t pos) const {
		return m_data.find(ch, pos);
	}

	inline size_t String::find(const String& str, size_t pos) const{
		return m_data.find(str.m_data, pos);
	}

	inline String String::substr(size_t pos, size_t n) const{
	
		String ret;
		ret += m_data.substr(pos, n).c_str();
		return ret;
	}

	inline String& String::replace(size_t pos, size_t n, const char* str) {
	
		if(strlen(str) > n)
			checkCapacity(strlen(str) - n);

		m_data.replace(pos, n, str);
		return *this;
	}

	inline String& String::replace(size_t pos, size_t n, const String& str) {
		if(str.length() > n)
			checkCapacity(str.length() - n);

		m_data.replace(pos, n, str.m_data);
		return *this;
	}

	inline String& String::erase(size_t pos, size_t n) {
		m_data.erase(pos, n);
		return *this;
	}

	inline String& String::erase(size_t pos) {
		m_data.erase(pos);
		return *this;
	}

	inline String& String::erase() {
		m_data.erase();
		return *this;
	}

	inline String& String::insert(size_t pos, const String& str) {
		checkCapacity(str.length());
		m_data.insert(pos, str.m_data);
		return *this;
	}

	inline String& String::insert(size_t pos, size_t n, char ch) {
		checkCapacity(n);
		m_data.insert(pos, n, ch);
		return *this;
	}

	inline String& String::assign(const char* str, size_t n) {
		m_data.clear();
		checkCapacity(n);

		m_data.assign(str, n);
		return *this;
	}

	inline String& String::append(const String& str) {
		checkCapacity(str.length());
		m_data.append(str.m_data);
		return *this;
	}

	inline String& String::append(const String& str, size_t pos, size_t n) {
		checkCapacity(n);
		m_data.append(str.m_data, pos, n);
		return *this;
	}

	inline int32_t String::compare(const String& str) const {
		return m_data.compare(str.m_data);
	}

	inline const char& String::operator[] ( size_t pos ) const {
		return m_data.operator[](pos);
	}

	inline char& String::operator[] ( size_t pos ) {
		return m_data.operator[](pos);
	}

	inline void String::clear() {
		m_data.clear();
	}

	inline size_t String::toUpper() {
		size_t upper_count = 0;
		for (std::string::iterator i=m_data.begin(); i != m_data.end(); i++) {
			char current = *i;
			*i = (char)toupper(*i);
			if(*i != current)
				upper_count++;
		}
		return upper_count;
	}

}
#endif /* MT_STRING__H */

