/*
 * Codec.cpp
 *
 *  Created on: 11.12.2009
 *      Author: hakamasy
 */

#include "mt/proto/smpp/common/Codec.h"
#include <string.h>
#include <errno.h>

namespace mt{

namespace smpp{

CodecPtr CodecFactory::getCodec(const std::string &from, const std::string &to){
	CodecPtr ptr;
	Codec* codec = new Codec(from, to);
	int32_t ret = codec->init();
	if(ret < 0){
		delete codec;
		return ptr;
	}

	ptr = codec;
	return ptr;

}


Codec::Codec(const std::string &f, const std::string &t) {

	from = f;
	to = t;
	if (!to.compare("UCS-2BE")) {
		toCS = UCS2;
	} else if (!to.compare("ASCII")) {
		toCS = ASCII;
	}

}

int32_t Codec::init() {

	cd = iconv_open(to.c_str(), from.c_str());

	if (cd == (iconv_t)(-1)) {
		return -1;
	}
	return 0;
}

// Encode data using a 7-bit encoding
std::string Codec::Encode7Bit(std::string data) { //wstring data
	unsigned char cprev = 0, cnext = 0, c = 0;
	unsigned int i = 0, j = 0, ofs = 0;
	std::string ret;

	for (i = 0; i < data.length(); i += 8) {
		cprev = cnext = 0;
		for (j = i; j < i + 8 && j < data.length(); j++) {
			ofs = j - i;
			c = (unsigned char) data[j];
			cprev |= c << (8 - ofs);
			cnext |= c >> ofs;

			if (j > i) {
				ret += cprev;
			}
			cprev = cnext;
			cnext = 0;
		}
	}
	if (j < i) {
		ret += cprev;
	}

	return ret;
}

int Codec::Encode(char* str, unsigned int len, std::string &body) {
	size_t inL, outL = 512;
	char* pIn = str;
	inL = len;
	char out[outL];
	char* pOut = out;

	size_t l = iconv(cd, &pIn, &inL, &pOut, &outL);
	if (l == size_t(-1)) {
		printf("can't convert '%s' (%s)\n", pIn, strerror(errno));
		return -1;
	}
	*pOut = '\0';

	body.assign(out);

	int ret = pOut - out;

	return ret;
}

int Codec::Encode(const std::string &str, std::string &body) {
	size_t inL, outL = 512;
	char* pIn = (char*) str.c_str();
	inL = str.length();
	char out[outL];
	int ret = 0;

	if (toCS == UCS2) {
		char* pOut = out;

		size_t len = iconv(cd, &pIn, &inL, &pOut, &outL);
		if (len == size_t(-1)) {
			printf("can't convert %s (%s)\n", pIn, strerror(errno));
			return -1;
		}
		*pOut = '\0';
		ret = pOut - out;
		body.assign(out, ret);
	} else {

		body = Encode7Bit(str);
		ret = body.length();
	}
	return ret;
}



////////////////////////////////////////////////////////////////////////////////////

// Encode data using a 7-bit encoding (segmented SMS)
/*
 string Encode7BitSegment(wstring data) {
 unsigned char cprev, cnext, c;
 INT i = 0, j = 0, ofs;
 string ret;

 ret += (unsigned char) data[0] << 1;

 for (i = 1; i < data.length(); i += 8) {
 cprev = cnext = 0;
 for (j = i; j < i + 8 && j < data.length(); j++) {
 ofs = j - i;
 c = (unsigned char) data[j];
 cprev |= c << (8 - ofs);
 cnext |= c >> ofs;

 if (j > i) {
 ret += cprev;
 }
 cprev = cnext;
 cnext = 0;
 }
 }
 if (j < i) {
 ret += cprev;
 }

 return ret;
 }
 */

////////////////////////////////////////////////////////////////////////////////////

// Decode data using a 7-bit encoding
/*
 wstring Decode7Bit(string data) {
 unsigned char cprev, cnext, c;
 INT i, j, ofs;
 wstring ret;

 for (i = 0; i < data.length(); i += 7) {
 cprev = cnext = 0;
 for (j = i; j < i + 7 && j < data.length(); j++) {
 ofs = j - i;
 c = (unsigned char) data[j];
 cprev |= (c & (0xFF >> (1 + ofs))) << ofs;
 cnext |= (c & (0xFF << (7 - ofs))) >> (7 - ofs);
 ret += (wchar_t) cprev;
 cprev = cnext;
 cnext = 0;
 }
 if (j == i + 7) {
 ret += (wchar_t) cprev;
 }
 }
 return ret;
 }

 ////////////////////////////////////////////////////////////////////////////////////
 */

// Decode data using a 7-bit encoding (Segmented SMS)
/*wstring Decode7BitSegment(string data) {
 unsigned char cprev, cnext, c;
 INT i, j, ofs;
 wstring ret;

 c = (unsigned char) data[0];
 ret += c >> 1;

 for (i = 1; i < data.length(); i += 7) {
 cprev = cnext = 0;
 for (j = i; j < i + 7 && j < data.length(); j++) {
 ofs = j - i;
 c = (unsigned char) data[j];
 cprev |= (c & (0xFF >> (1 + ofs))) << ofs;
 cnext |= (c & (0xFF << (7 - ofs))) >> (7 - ofs);
 ret += (wchar_t) cprev;
 cprev = cnext;
 cnext = 0;
 }
 if (j == i + 7) {
 ret += (wchar_t) cprev;
 }
 }
 return ret;
 }*/



}
}
