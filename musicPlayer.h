/**********************************************************************
 * ���� : ��Զ�Ĳ��                                                  *
 * QQ   : 2237505658                                                  *
 * ���� : 2237505658@qq.com                                           *
 * ����޸�  : 2020-7-18                                              *
 * ���뻷��  : Visual Studio 2019                                     *
 * EasyX�汾 : EasyX 2020-5-20                                        *
 * ע��      : ��ֻ֧�� x86 ���룬ʹ�� x64 ���뽫����               *
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
 * ���� Debug ģʽ�¿����˺꣬�� Release ģʽ��ע�͸ú�       *
 * �� Debug ģʽ�½����� cmd ������ʾ���ֵ�����Ϣ             *
 * �� Release ģʽ�½��ر� cmd ����                           *
 **************************************************************/

// #define DEBUG


#ifndef DEBUG
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	// ������ڵ�ַ ����cmd����
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