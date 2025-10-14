#ifndef MT_STRING__H
#define MT_STRING__H

#include <cstring>
#include <string.h>
#include <algorithm>
#include <string>

const size_t DEFAULT_CAPACITY = 64;

class MT_string {
 public:
	MT_string();
	MT_string(const char* str);
	MT_string(const char* str, size_t n);
	MT_string(size_t n, char ch);
	MT_string(const MT_string& str, size_t pos, size_t n = MT_string::npos);

	const char* c_str() const {
		return m_data.c_str();
	}
	
	const char* data() const {
		return m_data.data();
	}

	size_t length() const {return m_data.length();}
	size_t size() const {return m_data.size();}
	bool empty() const {return m_data.empty();}

	bool operator == (const char* str) const;
	bool operator == (const MT_string& str) const;
	bool operator != (const MT_string& str) const;
	MT_string& operator+=(char ch);
	MT_string& operator+=(const char* str);
	MT_string& operator+=(const MT_string& str);
	
	size_t find(const char* find_str, size_t pos = 0) const;
	size_t find(char ch, size_t pos = 0) const;
	size_t find(const MT_string& str, size_t pos = 0) const ;

	MT_string substr(size_t pos = 0, size_t n = MT_string::npos) const;
	MT_string& replace(size_t pos, size_t n, const char* str);
	MT_string& replace(size_t pos, size_t n, const MT_string& str);
	MT_string& erase(size_t pos, size_t n);
	MT_string& erase(size_t pos);
	MT_string& erase();

	MT_string& insert(size_t pos, const MT_string& str);
	MT_string& insert(size_t pos, size_t n, char ch);

	MT_string& assign(const char* str, size_t n);
	
	MT_string& append(const MT_string& str);
	MT_string& append(const MT_string& str, size_t pos, size_t n);

	int compare(const MT_string& str) const;

	const char& operator[] ( size_t pos ) const;
	char& operator[] ( size_t pos );
	void clear();

	size_t toUpper();
	friend MT_string operator + (const MT_string& a, const MT_string& b);
	friend bool operator==(const char* a, const MT_string& b);
	friend bool operator!=(const char* a, const MT_string& b);
	friend std::ostream& operator<<(std::ostream& stream, const MT_string& str);
	friend bool operator<(const MT_string& a, const MT_string& b);

public:
	static const size_t npos;

private:
	size_t getCapacity(size_t string_size);
	size_t createCapacity(size_t new_size);
	void checkCapacity(size_t appended_string_size);

private:
	std::string m_data;
};

bool operator==(const char* a, const MT_string& b);
bool operator!=(const char* a, const MT_string& b);
MT_string operator + (const MT_string& a, const MT_string& b);
MT_string operator + (const char* str, const MT_string& mt_str);
std::ostream& operator<<(std::ostream& stream, const MT_string& str);
bool operator<(const MT_string& a, const MT_string& b);

inline MT_string::MT_string() {
	m_data.reserve(DEFAULT_CAPACITY);
}

inline MT_string::MT_string(const char* str) {
	m_data.reserve(getCapacity(strlen(str) + 1));
	m_data += str;
}

inline size_t MT_string::getCapacity(size_t string_size) {
	return (string_size < DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : createCapacity(string_size);
}

inline size_t MT_string::createCapacity(size_t new_size) {
	return (new_size / DEFAULT_CAPACITY + 1) * DEFAULT_CAPACITY;
}

inline MT_string::MT_string(const char* str, size_t n) {
	m_data.reserve(getCapacity(n + 1));
	m_data += std::string(str, n);
}

inline MT_string::MT_string(size_t n, char ch) {
	m_data.reserve(getCapacity(n + 1));
	m_data += std::string(n, ch);
}

inline MT_string::MT_string(const MT_string& str, size_t pos, size_t n) {
	m_data.reserve(getCapacity(n + 1));
	m_data += std::string(str.m_data, pos, n);
}

inline bool MT_string::operator==(const char* str) const {
	return (m_data == str);
}

inline bool MT_string::operator == (const MT_string& str) const {
	return (m_data == str.m_data);
}

inline bool MT_string::operator != (const MT_string& str) const {
	return (m_data != str.m_data);
}

inline MT_string& MT_string::operator+=(char ch) {
	checkCapacity(1);
	m_data += ch;
	return *this;
}

inline void MT_string::checkCapacity(size_t appended_string_size) {
	size_t new_size = appended_string_size + m_data.length();
	if(new_size > m_data.capacity())
		m_data.reserve(createCapacity(new_size));
}

inline MT_string& MT_string::operator+=(const char* str) {
	checkCapacity(strlen(str));
	m_data += str;
	return *this;
}

inline MT_string& MT_string::operator+=(const MT_string& str) {
	checkCapacity(str.length());
	m_data += str.m_data;
	return *this;
}

inline size_t MT_string::find(const char* find_str, size_t pos) const {
	return m_data.find(find_str, pos);
}

inline size_t MT_string::find(char ch, size_t pos) const {
	return m_data.find(ch, pos);
}

inline size_t MT_string::find(const MT_string& str, size_t pos) const{
	return m_data.find(str.m_data, pos);
}

inline MT_string MT_string::substr(size_t pos, size_t n) const{
	
	std::string s = m_data.substr(pos, n);

	return MT_string(s.data(), s.length());
}

inline MT_string& MT_string::replace(size_t pos, size_t n, const char* str) {
	
	if(strlen(str) > n)
		checkCapacity(strlen(str) - n);

	m_data.replace(pos, n, str);
	return *this;
}

inline MT_string& MT_string::replace(size_t pos, size_t n, const MT_string& str) {
	if(str.length() > n)
		checkCapacity(str.length() - n);

	m_data.replace(pos, n, str.m_data);
	return *this;
}

inline MT_string& MT_string::erase(size_t pos, size_t n) {
	m_data.erase(pos, n);
	return *this;
}

inline MT_string& MT_string::erase(size_t pos) {
	m_data.erase(pos);
	return *this;
}

inline MT_string& MT_string::erase() {
	m_data.erase();
	return *this;
}

inline MT_string& MT_string::insert(size_t pos, const MT_string& str) {
	checkCapacity(str.length());
	m_data.insert(pos, str.m_data);
	return *this;
}

inline MT_string& MT_string::insert(size_t pos, size_t n, char ch) {
	checkCapacity(n);
	m_data.insert(pos, n, ch);
	return *this;
}

inline MT_string& MT_string::assign(const char* str, size_t n) {
	m_data.clear();
	checkCapacity(n);

	m_data.assign(str, n);
	return *this;
}

inline MT_string& MT_string::append(const MT_string& str) {
	checkCapacity(str.length());
	m_data.append(str.m_data);
	return *this;
}

inline MT_string& MT_string::append(const MT_string& str, size_t pos, size_t n) {
	checkCapacity(n);
	m_data.append(str.m_data, pos, n);
	return *this;
}

inline int MT_string::compare(const MT_string& str) const {
	return m_data.compare(str.m_data);
}

inline const char& MT_string::operator[] ( size_t pos ) const {
	return m_data.operator[](pos);
}

inline char& MT_string::operator[] ( size_t pos ) {
	return m_data.operator[](pos);
}

inline void MT_string::clear() {
	m_data.clear();
}

inline size_t MT_string::toUpper() {
	size_t upper_count = 0;
	for (std::string::iterator i=m_data.begin(); i != m_data.end(); i++) {
		char current = *i;
		*i = (char)toupper(*i);
		if(*i != current)
			upper_count++;
	}
	return upper_count;
}

#endif /* MT_STRING__H */


