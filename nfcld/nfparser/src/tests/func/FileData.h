/*
 * FileData.h
 *
 *  Created on: 28.01.2011
 *      Author: wocson
 */

#ifndef FILEDATA_H_
#define FILEDATA_H_

#include <mt/logger/Logable.h>
#include <mt/common/DynamicBuffer.h>


class FileData: public mt::Logable
{
public:
	FileData(const std::string& fname);
	mt::ByteBuffer getDataFromFile(void);
	virtual ~FileData();

private:
	std::string m_filename;
	uint32_t getFileSize(void);
};

#endif /* FILEDATA_H_ */
