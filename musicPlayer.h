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
 *              class MusicPlayer              *
 ***********************************************/

class MusicPlayer
{
private:
	string filePath="";										// Ҫ���ҵ��ļ�·��
	string musicFormat = "mp3";								// Ҫ���ҵ��ļ���ʽ
	string nowMusicName;									// ���ڲ�����������
	fstream file;											// �洢ȫ·�����������ļ�����
	vector<string> musicPathName;							// �洢ȫ·�������ļ���
	vector<string> musicName;								// �洢�����ļ���
	MusicMCI musicMci;										// ִ�����ֲ��Ų����Ķ���
	size_t status = 0;										// ����״̬ 0δ���� 1���ڲ��� 2��ͣ���� 3�������
	size_t vole = 500;										// ��ǰ������С
	size_t number = 0;										// ���ڲ��������ֱ��
	size_t mode = 1;										// ���ֲ���ģʽ 0�������� 1˳�򲥷�(�б�ѭ��) 2�������

	wstring stringTowstring(const string& str);				// stringתwstring
	void findMusicName(const string& path);					// Ѱ�������ļ�
	void getFilePath();										// ��ȡ����·��
	void wFile();											// ��musicPathNameд���ļ�music.mn
	void rFile();											// ���ļ�music.mn��ȡ��musicPathName
	void openMusic(size_t num);
	void playMusic();
	void pauseMusic();
	void stopMusic();
	void closeMusic();
	void setVolumeMusic(size_t volume);
	void setFilePath();										// �����ļ�����·��
	void setPlayMode();										// �������ֲ���ģʽ  ����ѭ�� ˳�򲥷� �������
	void showPlayTime();									// ��ʾ���ڲ������ֵ���Ϣ
	void showMusicName();									// ��ʾ���������б�
	int getPlayerBackTimeMusic();							// ��ȡ��ǰ�������ֵĵ�ǰ����ʱ��
	int getTotalTime();										// ��ȡ��ǰ�������ֵ���ʱ��

	void pos(short x, short y)noexcept;						// ���ù��λ��
public:
	MusicPlayer();
	~MusicPlayer();
	void chooseMusicPlay();
	int chooseFunction();
};