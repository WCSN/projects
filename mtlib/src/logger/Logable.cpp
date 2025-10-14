/*
 * Logable.cpp
 *
 *  Created on: 18.07.2010
 *      Author: hakamasy
 */


#include "mt/logger/Logable.h"

using namespace mt;


//LoggerPtr Logable::m_logger;


Logable::Logable(){

	if(!m_logger)
		m_logger = LoggerFactory::instance()->getLogger<Logable>("Logable");

}
