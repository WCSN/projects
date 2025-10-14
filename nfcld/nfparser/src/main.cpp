//============================================================================
// Name        : nfparser.cpp
// Author      : Wocson
// Version     :
// Copyright   : Mobitecnologies
// Description :
//============================================================================

#include <iostream>                                                                                                          

#include <mt/common/Exception.h>                                                                                             
#include <mt/logger/LoggerFactory.h>                                                                                         

#include "nfparser.h"

int main(int argc, char **argv) {
	try {
		NfParser nfparser(argc, argv);
		nfparser.start();
	} catch (const mt::Exception& me) {
		std::cerr << "Global mt::Exception. Reason: " << me.displayText()
				<< std::endl;
	} catch (...) {
		std::cerr << "Global unknown exception." << std::endl;
	}
	return 0;

}
