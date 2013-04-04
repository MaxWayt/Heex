/*
** SharedDefines.h for Heex
*/

#ifndef SHAREDDEFINES_H_
# define SHAREDDEFINES_H_

// Debug mode
//#define DEBUG
#ifdef DEBUG
#undef DEBUG
#endif


#define MAP_PRECISION 5

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned long uint64;
typedef long int64;

#endif /* !SHAREDDEFINES_H_ */
