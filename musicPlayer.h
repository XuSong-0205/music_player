#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
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
 * ����MusicMCI��MusicPlayer���н���            *
 ***********************************************/

class MusicData
{
private:
	string filePath = "";									// Ҫ���ҵ��ļ�·��
	string musicFormat = "mp3";								// Ҫ���ҵ��ļ���ʽ
	string nowMusicName;									// ���ڲ�����������
	vector<string> musicPathName;							// �洢ȫ·�������ļ���
	vector<string> musicName;								// �洢�����ļ���
	MusicMCI musicMci;										// ִ�����ֲ��Ų����Ķ���
	size_t status = 0;										// ����״̬ 0δ���� 1���ڲ��� 2��ͣ���� 3�������
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

	friend class MusicPlayer;
};


/************************************************
 *              class MusicPlayer               *
 * ������                                       *
 * ���û����н���                               *
 * ��������������͸�MusicDate���д���          *
 ***********************************************/

class MusicPlayer
{
private:
	MusicData musicData;									// �������������

	void pos(short x, short y)noexcept;						// ��꺯��
	void setMusicVolume();									// �������ֲ�������
	void setFilePath();										// �����ļ�����·��
	void setPlayMode();										// �������ֲ���ģʽ
	void deleteMusic();										// �Ӳ����б���ɾ������
	void chooseMusicPlay();									// �������б���ѡ�����ֲ���
	void showMusicName();									// ��ʾ���������б�
	void showPlayInformation();								// ��ʾ���ֵĲ�����Ϣ
public:
	MusicPlayer();											// Ĭ�Ϲ��캯��
	~MusicPlayer();											// ��������
	int chooseFunction();									// ����ѡ��
};