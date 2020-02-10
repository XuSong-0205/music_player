#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<array>
#include<algorithm>
#include<ctime>
#include<cmath>
#include<cstdlib>
#include<conio.h>
#include<io.h>
#include<easyx.h>
#include<windows.h>
#include<mmsystem.h>
#include<Digitalv.h>
#pragma comment(lib, "winmm.lib")

using std::cin;
using std::cout;
using std::wcout;
using std::cerr;
using std::endl;
using std::string;
using std::wstring;
using std::vector;
using std::array;
using std::fstream;
using std::ios_base;



/************************************************
 *                class MusicMCI                *
 * ���ֲ��Ų�����                               *
 * ���еײ����ֲ���                             *
 * ��ͣ���رգ�������������ȡ����ʱ��Ȳ���     *
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
	BOOL setVolume(size_t nVolumeValue)noexcept;			// ��������
};



/************************************************
 *              class MusicData                 *
 * ���ݲ�����                                   *
 * ����Ӧ�����ݽ��в���                         *
 * ����MusicMCI��CmdMusicPlayer���н���            *
 ***********************************************/

class MusicData
{
private:
	string filePath;										// Ҫ���ҵ��ļ�·��
	string musicFormat = "mp3";								// Ҫ���ҵ��ļ���ʽ
	wstring nowMusicName;									// ���ڲ�����������
	vector<string> musicPathName;							// �洢ȫ·�������ļ���
	vector<wstring> musicName;								// �洢�����ļ���
	MusicMCI musicMci;										// ִ�����ֲ��Ų����Ķ���
	size_t status = 0;										// ����״̬ 0δ���� 1���ڲ��� 2��ͣ���� //3�������
	size_t volume = 500;									// ��ǰ������С
	size_t number = 0;										// ���ڲ��������ֱ��
	size_t mode = 2;										// ���ֲ���ģʽ 0�������� 1˳�򲥷�(�б�ѭ��) 2�������

	void openMusic(size_t num);								// �򿪲���������
	void playMusic();										// ��������
	void pauseMusic();										// ��ͣ����
	void stopMusic();										// ֹͣ����
	void closeMusic();										// �ر�����
	void setMusicVolume(size_t vol);						// ��������
	int getPlayerBackTimeMusic();							// ��ȡ��ǰ�������ֵĵ�ǰ����ʱ��
	int getTotalTime();										// ��ȡ��ǰ�������ֵ���ʱ��

	wstring stringTowstring(const string& str);				// stringתwstring
	void getFilePath();										// ��ȡ����·��
	void findMusicName(const string& path);					// Ѱ�������ļ�
	void wFilePath(fstream& file);							// ��filePathд���ļ�filePath.ad
	void rFilePath(fstream& file);							// ���ļ�filePath.ad��ȡ��filePath
	void wFileMusic(fstream& file);							// ��musicPathNameд���ļ�music.mn
	void rFileMusic(fstream& file);							// ���ļ�music.mn��ȡ��musicPathName
	void deleteMusicData(vector<int>& vec);					// ɾ����vec��������ͬ�±�ĸ���
public:
	MusicData();											// Ĭ�Ϲ��캯��
	~MusicData();											// ��������

	friend class CmdMusicPlayer;
	friend class GuiMusicPlayer;
};



/************************************************
 *              class GuiMusicPlayer            *
 * ������                                       *
 * ���û����н���                               *
 * ��������������͸�MusicDate���д���          *
 ***********************************************/

class GuiMusicPlayer
{
private:
	MusicData musicData;
	IMAGE img;
	bool bList = false;
	array<int, 2> numRange{ 0,12 };							// �����б���ʾ�����ַ�Χ numRange[0]ָ����ʾ�������б�musicData.musicName�Ŀ�ʼ
															// numRange[1]ָ��musicData.musicName�ڲ����б�����һ��
	static const int WIDTH = 960;
	static const int HEIGHT = 640;


	bool findBgPicture()noexcept;							// �����Ƿ���ڱ���ͼƬ
	void ui();
	void drawStartPause();									// ����ʼ��ͣ��
	void drawPlayInformation();								// ��ʾ������Ϣ
public:
	GuiMusicPlayer();
	~GuiMusicPlayer();

	void choose();
};