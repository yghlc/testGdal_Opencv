/***************************************************************
 * Name:      typedefine.h
 * Author:    Huang Lingcao
 * E-mail:    huanglingcao@gmail.com
 * Created:   2013-04-05
 * Copyright: Wuhan University 2013
 * License:
 **************************************************************/

#ifndef TYPEDEFINE_H_INCLUDED
#define TYPEDEFINE_H_INCLUDED
#endif

//类型定义
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef BYTE*       LPBYTE;


#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


#define FORMAT_CHAR_LENGTH 1024


#ifdef __LINUX__      //自己定义的宏，标记 LINUX 平台，如果该宏没定义，就默认为WINDOW 平台
                              //LINUX 平台时，需要在 编译选项里面定义该宏  Huang 2013-4-6
//#define  \\  /        //字符变换
#ifndef BOOL
#define BOOL int
#endif
#endif

#ifndef __AFX_H__    //没有包含MFC
#ifndef CString
#define CString  string     //将window下的CString 换为 C++ 标准库的 string
#endif
#endif

//系统头文件包含
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

#ifdef __LINUX__
#include <sys/stat.h>
#include <unistd.h>
#else


//#ifdef __AFX_H__ 
////如果已经包含了MFC的头文件，就不能包含 <windows.h>
//#else
//#include <windows.h>
//#endif
//
//#endif


//自定义头文件包含




#endif // TYPEDEFINE_H_INCLUDED
