/**********************************************************************
 * 作者 : 悠远的苍穹                                                  *
 * QQ   : 2237505658                                                  *
 * 邮箱 : 2237505658@qq.com                                           *
 * 最后修改  : 2020-7-18                                              *
 * 编译环境  : Visual Studio 2019                                     *
 * EasyX版本 : EasyX 2020-5-20                                        *
 * 注意      : 暂只支持 x86 编译，使用 x64 编译将出错！               *
 **********************************************************************/

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <io.h>
#include <conio.h>
#include <graphics.h>
#include <windows.h>
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib, "winmm.lib")


/**************************************************************
 * 请在 Debug 模式下开启此宏，在 Release 模式下注释该宏       *
 * 在 Debug 模式下将开启 cmd 窗口显示各种调试信息             *
 * 在 Release 模式下将关闭 cmd 窗口                           *
 **************************************************************/

// #define DEBUG


#ifndef DEBUG
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	// 设置入口地址 隐藏cmd窗口
#endif // DEBUG


using std::wcout;
using std::wcerr;
using std::endl;
using std::string;
using std::wstring;
using std::vector;
using std::array;
using std::fstream;
using std::ios_base;