#ifndef __MT__DATA__H
#define __MT__DATA__H

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

#ifdef _64_BIT_SYSTEM
typedef long int_t;
typedef unsigned long uint_t;
#else
typedef int int_t;
typedef unsigned int uint_t;
#endif

#endif //__MT__DATA__H
