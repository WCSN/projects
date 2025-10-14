/*
 * srvfunc.h
 *
 *  Created on: 01.02.2011
 *      Author: wocson
 */

#ifndef SRVFUNC_H_
#define SRVFUNC_H_

#include <mt/common/DynamicBuffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define START_NFPARSER start_nfparser
#define STOP_NFPARSER stop_nfparser

#define UDPTIMEOUT 3000

int start_nfparser(const std::string& conf);
int stop_nfparser(void);

void outbytes(std::string name, mt::ByteBuffer buffout);

#endif /* SRVFUNC_H_ */
