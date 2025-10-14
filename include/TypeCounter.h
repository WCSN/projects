#ifndef TYPECOUNTER__H
#define TYPECOUNTER__H

#include <map>
#include <MT_string.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "MT_threads.h"

class SmartPtrDebugInfo {
public:
 SmartPtrDebugInfo() : m_smartPtrsCount(0),
		m_referencesCount(0) {
	}

 SmartPtrDebugInfo(int ptr_count, int ref_count) : m_smartPtrsCount(ptr_count),
		m_referencesCount(ref_count) {
		}

	void incPtrsCount() {
		MT_automutex amtx(m_ptrMutex,__FILE__,__LINE__);
		m_smartPtrsCount++;
	}

	void decPtrsCount() {
		MT_automutex amtx(m_ptrMutex,__FILE__,__LINE__);
		m_smartPtrsCount--;
	}

	void incRefCount() {
		MT_automutex amtx(m_refMutex,__FILE__,__LINE__);
		m_referencesCount++;
	}

	void decRefCount() {
		MT_automutex amtx(m_refMutex,__FILE__,__LINE__);
		m_referencesCount--;
	}

	int getPtrsCount() {
		MT_automutex amtx(m_ptrMutex,__FILE__,__LINE__);
		return m_smartPtrsCount;
	}

	int getRefCount() {
		MT_automutex amtx(m_refMutex,__FILE__,__LINE__);
		return m_referencesCount;
	}

private:
	volatile int m_smartPtrsCount;
	volatile int m_referencesCount;
	MT_mutex m_ptrMutex;
	MT_mutex m_refMutex;
};

typedef std::map<MT_string, SmartPtrDebugInfo> CounterMap;

class TypeCounter {
public:
	static void addCounter(const MT_string& name, bool increment_real_object_count = false) {
		CounterMap::iterator it = m_counter.find(name);
		if(it == m_counter.end()) {
			SmartPtrDebugInfo debug_info(1, ((increment_real_object_count) ? 1 : 0));
			m_counter[name] = debug_info;
		} else {
			it->second.incPtrsCount();
			if(increment_real_object_count)
				it->second.incRefCount();
		}
	}

	static void delCounter(const MT_string& name) {
		CounterMap::iterator it = m_counter.find(name);
		if(it != m_counter.end()) {
			if(it->second.getPtrsCount() <= 0) {
				std::stringstream stream;
				stream << "delCounter count: " << it->second.getPtrsCount() << "; name " << name;
				m_errors.push_back(stream.str().c_str());
			} else {
				it->second.decPtrsCount();
			}
		} else {
			std::stringstream stream;
			stream << "delCounter no counter with name " << name;
			m_errors.push_back(stream.str().c_str());
		}
	}

	static void decrementRealObjectsCount(const MT_string& name) {
		CounterMap::iterator it = m_counter.find(name);
		if(it != m_counter.end()) {
			if(it->second.getRefCount() <= 0) {
				std::stringstream stream;
				stream << "decrementRealObjectsCount: error m_referencesCount: "
							 << it->second.getRefCount() << "; type T: " << name;
				m_errors.push_back(stream.str().c_str());
			} else {
				it->second.decRefCount();
			}
		} else {
			std::stringstream stream;
			stream << "decrementRealObjectsCount: no counter for " << name << std::endl;
			m_errors.push_back(stream.str().c_str());
		}
	}
	
	static int count(const MT_string& name) {
		CounterMap::iterator it = m_counter.find(name);
		if(it != m_counter.end()) {
			return it->second.getPtrsCount();
		}
		else
			return 0;
	}

	static MT_string  print() {
		std::stringstream stream;
		stream << "Smart ptrs: " << std::endl;
		for(CounterMap::iterator it = m_counter.begin(); it != m_counter.end(); it++) {
			stream << "\t " << it->first << ": \t " << it->second.getPtrsCount()  << " : " << it->second.getRefCount() << std::endl;
		}
		stream << "----------------------------------------" << std::endl;
		for(std::vector<MT_string>::iterator it = m_errors.begin(); it != m_errors.end(); it++) {
			stream << *it << std::endl;
		}
		stream << "----------------------------------------" << std::endl;
		return stream.str().c_str();
	}

	static CounterMap m_counter;
	static std::vector<MT_string> m_errors;
	
};

#endif /* TYPECOUNTER__H */
