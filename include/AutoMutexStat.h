#ifndef __AUTOMUTEXSTAT__H
#define __AUTOMUTEXSTAT__H

#include <mt/common/Singleton.h>
#include <MT_string.h>
#include <map>

class LockTimeInfo {
public:
 
 LockTimeInfo() : m_commonTime(0),
		m_maxTime(0),
		m_lockCount(0) {
	}

	void add(unsigned long time) {
		m_commonTime += time;
		updateMaxTime(time);
		m_lockCount++;
	}
 
private:
	void updateMaxTime(unsigned long time) {
		if(m_maxTime < time) 
			m_maxTime = time;
	}

public:
	MT_string output() {
		MT_string out;
		out += "common: ";
		appendToString(out, m_commonTime);
		out += "max: ";
		appendToString(out, m_maxTime);
		out += "count: ";
		appendToString(out, m_lockCount);
		out += "average: ";
		appendToString(out, m_commonTime/m_lockCount);
		return out;
	}

private:
	void appendToString(MT_string& out, unsigned long count) {
		char str[20];
		sprintf(str, "%ld", count);
		out += str;
		out += " ";
	}

private:
	unsigned long m_commonTime;
	unsigned long m_maxTime;
	unsigned long m_lockCount;
};

typedef std::map<MT_string, LockTimeInfo> TimeMap;

class AutoMutexStatImplemention {
	friend class mt::Singleton<AutoMutexStatImplemention>;

private:
	AutoMutexStatImplemention() {};

public:
	void addWaitLockTime(const MT_string& name, unsigned long time) {
		m_waitLockTimeMutex.lock();
		TimeMap::iterator it = m_waitLockTime.find(name);
		if(it == m_waitLockTime.end()) {
			LockTimeInfo info;
			info.add(time);
			m_waitLockTime.insert(std::make_pair(name, info));
			m_waitLockTimeMutex.unlock();
			return;
		}
		it->second.add(time);
		m_waitLockTimeMutex.unlock();
	}

	void addLockingTime(const MT_string& name, unsigned long time) {
		m_lockingTimeMutex.lock();
		TimeMap::iterator it = m_lockingTime.find(name);
		if(it == m_lockingTime.end()) {
			LockTimeInfo info;
			info.add(time);
			m_lockingTime.insert(std::make_pair(name, info));
			m_lockingTimeMutex.unlock();
			return;
		}
		it->second.add(time);
		m_lockingTimeMutex.unlock();
	}

	MT_string output() {
		MT_string out = "Auto Mutexes Times: \n";
		out += outputWaitLockTime() + outputLockingTime();
		return out;
	}

private:
	MT_string outputWaitLockTime() {
		m_waitLockTimeMutex.lock();
		MT_string out = "\tWait Lock Time: \n\t";
		for(TimeMap::iterator it = m_waitLockTime.begin(); it != m_waitLockTime.end(); it++) {
			out += it->first + ": " + it->second.output() + "\n\t\t";
		}
		out += "\n";
		m_waitLockTime.clear();
		m_waitLockTimeMutex.unlock();
		return out;
	}

	MT_string outputLockingTime() {
		m_lockingTimeMutex.lock();
		MT_string out = "\tLocking Time: \n\t";
		for(TimeMap::iterator it = m_lockingTime.begin(); it != m_lockingTime.end(); it++) {
			out += it->first + ": " + it->second.output() + "\n\t\t";
		}
		out += "\n";
		m_lockingTime.clear();
		m_lockingTimeMutex.unlock();
		return out;
	}

private:
	TimeMap m_waitLockTime;
	TimeMap m_lockingTime;
	MT_mutex m_waitLockTimeMutex;
	MT_mutex m_lockingTimeMutex;
};

typedef mt::Singleton<AutoMutexStatImplemention> AutoMutexStat;

#endif /* __AUTOMUTEXSTAT__H */

