#ifndef __CHANGING_LOG__H
#define __CHANGING_LOG__H

#include <MT_smart_ptr.h>
#include <MT_string.h>
#include <stdio.h>

class ChangingLog
{
public:
	ChangingLog();
	ChangingLog(MT_string file_name);
	virtual ~ChangingLog() {}

public:
	int log(MT_string log_msg);

protected:
	virtual void changeFile();
	virtual bool isFileExists();
	virtual void changingFile();
	virtual FILE* openFile(MT_string file_name);
	bool needChangeLog();
	int writeToFile(const MT_string& log_msg);

protected:
	MT_string m_fileName;
	FILE* m_fileDesc;

	unsigned int m_lastFileCheckTime;
	unsigned int m_checkInterval;
	MT_mutex m_mutex;
};

typedef MT_smart_ptr<ChangingLog> ChangingLogPtr;

#endif //__CHANGING_LOG__H
