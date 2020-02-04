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
	BOOL getMusicTime(DWORD& time)noexcept;					// ��ȡ������ʱ��
	BOOL setVolume(int nVolumeValue)noexcept;				// ��������
};


/***********************************************
 *                class MusicPlayer            *
 ***********************************************/


class MusicPlayer
{
private:
	string filePath="";										// Ҫ���ҵ��ļ�·��
	string musicFormat = "mp3";								// Ҫ���ҵ��ļ���ʽ
	string nowMusicName;									// ���ڲ��ŵ�����
	fstream file;											// �洢ȫ·�����������ļ�����
	vector<string> musicPathName;							// �洢ȫ·�������ļ���
	vector<string> musicName;								// �洢�����ļ���
	MusicMCI musicMci;										// ִ�����ֲ��Ų����Ķ���
	int status = 0;											// ����״̬ 0δ���� 1���ڲ��� 2��ͣ���� 3�������
	int vole = 500;											// ��ǰ������С
	int number = 0;											// ���ڲ��ŵ����ֱ��
	int mode = 0;											// ���ֲ���ģʽ 0�������� 1˳�򲥷�(�б�ѭ��) 2�������

	wstring stringTowstring(const string& str);
	void findMusicName(const string& path);					// Ѱ�������ļ�
	void getFilePath();										// ��ȡ����·��
	void wFile();
	void rFile();
	void openMusic(int num);
	void playMusic();
	void pauseMusic();
	void stopMusic();
	void closeMusic();
	void setVolumeMusic(int volume);
	int getPlayerBackTimeMusic();							// ��ȡ��ǰ�������ֵĵ�ǰ����ʱ��
	int getTotalTime();										// ��ȡ��ǰ�������ֵ���ʱ��
	void setFilePath();										// �����ļ�����·��
	void setPlayMode();										// �������ֲ���ģʽ  ˳�򲥷ţ��б�ѭ���� ������� ��������


	void pos(int x, int y);									// ���ù��λ��
public:
	MusicPlayer();
	~MusicPlayer();
	void showMusicName();
	void showPlayTime();
	void chooseMusicPlay();
	int chooseFunction();
};