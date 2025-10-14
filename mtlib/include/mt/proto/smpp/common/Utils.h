/*
 * Utils.h
 *
 *  Created on: 23.09.2010
 *      Author: hakamasy
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "mt/proto/smpp/common/DataTypes.h"


namespace mt {
namespace smpp{
class Utils{

	static uint8_t m_udhSmsId;

public :


	static std::vector<MessagePart>* segmentateSmsBody(const uint8_t* msg,
			uint32_t len);




};

}
}
#endif /* UTILS_H_ */
