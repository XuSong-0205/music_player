#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<conio.h>
#include<io.h>
#include<windows.h>
#include<mmsystem.h>
#include<Digitalv.h>
#pragma comment(lib, "winmm.lib")

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::wstring;
using std::vector;
using std::fstream;
using std::ios_base;






/***********************************************
 *                class MusicMCI               *
 ***********************************************/

class MusicMCI
{
private:
	MCIDEVICEID nDeviceID;									// �豸ID

public:
	MusicMCI()noexcept;										// Ĭ�Ϲ��캯��
	~MusicMCI()noexcept;									// ��������
	BOOL open(LPCWSTR strSongPath)noexcept;					// ������
	BOOL play()noexcept;									// ��������
	BOOL pause()noexcept;									// ��ͣ����
	BOOL stop()noexcept;									// ֹͣ����
	BOOL close()noexcept;									// �ر�����
	BOOL getPlayBackTime(DWORD& pos)noexcept;				// ��ȡ����ʱ��
	BOOL setVolume(int nVolumeValue)noexcept;				// ��������
};


/***********************************************
 *                class MusicPlayer            *
 ***********************************************/


class MusicPlayer
{
private:
	string filePath="E:\\����";								// Ҫ���ҵ��ļ�·��
	string musicFormat = "mp3";								// Ҫ���ҵ��ļ���ʽ
	string nowMusicName;									// ���ڲ��ŵ�����
	fstream file;											// �洢ȫ·�����������ļ�����
	vector<string> musicPathName;							// �洢ȫ·�������ļ���
	vector<string> musicName;								// �洢�����ļ���
	MusicMCI musicMci;										// ִ�����ֲ��Ų����Ķ���

	wstring stringTowstring(const string& str);
	void findMusicName(const string& path);
public:
	MusicPlayer();
	~MusicPlayer();
	void playMusic(int num);
	void pauseMusic();
	void stopMusic();
	void closeMusic();
	void setVolumeMusic(int volume);
	int getPlayerBackTimeMusic();
	void wFile();
	void rFile();
	void showMusicName();
	void chooseMusicPlay();
	void chooseFunction();
	void showPlayTime();
};