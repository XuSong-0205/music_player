#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<windows.h>
#include<mmsystem.h>
#include<Digitalv.h>
#pragma comment(lib, "winmm.lib")


/***********************************************
 *                    CMCI.h                    *
 ***********************************************/

class CMCI
{
private:
	MCIDEVICEID nDeviceID;

public:
	CMCI(void);
	~CMCI(void);
	BOOL Open(LPCWSTR strSongPath)noexcept;
	BOOL Play();
	BOOL Pause();
	BOOL Stop();
	BOOL Close();
	BOOL GetCurrentPos(DWORD& pos);
	BOOL SetVolume(int nVolumeValue);
};