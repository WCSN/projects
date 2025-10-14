/*
 * Codec.h
 *
 *  Created on: 11.12.2009
 *      Author: hakamasy
 */

#ifndef CODEC_H_
#define CODEC_H_

#include <iconv.h>
#include <string>
#include "mt/proto/smpp/common/DataTypes.h"
#include "mt/logger/Logable.h"
#include "mt/common/SmartPtr.h"



namespace mt{

namespace smpp{




class Codec : public Logable {
	CodingScheme toCS;
	CodingScheme fromCS;

	std::string from;
	std::string to;
	iconv_t cd;
	std::string Encode7Bit(std::string data);

public :

	Codec(const std::string &from, const std::string &to);
	int32_t init();

	int Encode(char* str, unsigned int len, std::string &body);

	int Encode(const std::string &str, std::string &body);
	//int Encode(char* str, unsigned int len, char** buf);
	int Decode(char*, unsigned int);

};

typedef SmartPtr<Codec> CodecPtr;

class CodecFactory : public Logable{

public :
	static CodecPtr getCodec(const std::string &from, const std::string &to);
};


}
}
#endif /* CODEC_H_ */
