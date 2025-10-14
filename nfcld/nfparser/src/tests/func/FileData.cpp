/*
 * FileData.cpp
 *
 *  Created on: 28.01.2011
 *      Author: wocson
 */

#include <iostream>
#include <fstream>

#include "FileData.h"


FileData::FileData(const std::string& fname):m_filename(fname)
{

}

FileData::~FileData() {

}


uint32_t FileData::getFileSize(void)
{
	std::ifstream fs;
  	fs.open ( m_filename.c_str(), std::ios::binary );
	fs.seekg(0, std::ios::end);
	uint32_t size = fs.tellg();
	fs.seekg(0);
	fs.close();
	return size;
}

mt::ByteBuffer FileData::getDataFromFile(void) {
	uint32_t size = getFileSize();
	uint8_t resp[size];
	std::ifstream file_resp(m_filename.c_str(), std::ios::in | std::ios::binary);
	file_resp.read((char*)resp, size);
	mt::ByteBuffer buf;
	buf.append(resp, size);
	return buf;
}
