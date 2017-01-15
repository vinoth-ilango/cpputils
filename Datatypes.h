#ifndef DATATYPES_H
#define DATATYPES_H

#include <stdint.h>

//! signed char, min (-128) max (127)
typedef int8_t    int8;
//! unsigned char, min (0) max (255)
typedef uint8_t   uint8;

//! short, min (-32768) max (32767)
typedef int16_t   int16;
//! unsigned short min (0) max (65,535)
typedef uint16_t  uint16;

//! int,min (-2,147,483,648) max (2,147,483,647)
typedef int32_t   int32;
//! unsigned int, min (0) max (4,294,967,295)
typedef uint32_t  uint32;

//! long long, min (-9,223,372,036,854,775,808) max (9,223,372,036,854,775,807)
typedef int64_t   int64;
//! unsigned long long, min(0) max(8,446,744,073,709,551,615)
typedef uint64_t  uint64;


#endif // DATATYPES_H

