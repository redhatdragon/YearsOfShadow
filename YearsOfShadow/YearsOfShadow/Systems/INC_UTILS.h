#pragma once
#error "LEGACY CODE, DO NOT USE!"
#define _INC_UTILS_IDENT(x) x
#define _INC_UTILS_XSTR(x) #x
#define _INC_UTILS_STR(x) _INC_UTILS_XSTR(x)
#define INC_UTILS_PATH(x,y) _INC_UTILS_STR(_INC_UTILS_IDENT(x)_INC_UTILS_IDENT(y))
//#define INC_UTILS_INCLUDE()

//#define Dir sys/
//#define File socket.h
//
//#include PATH(Dir,File)
//#include PATH(std, io.h)
//#include INC_UTILS_PATH(std, io.h)