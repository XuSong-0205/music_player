#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <conio.h>
#include <io.h>
#include <graphics.h>
#include <windows.h>
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib, "winmm.lib")

// #define DEBUG

#ifndef DEBUG
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	// ������ڵ�ַ ����cmd����
#endif // 



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
	virtual ~MusicMCI();									// ����������
	bool open(LPCWSTR strSongPath)noexcept;					// ������
	bool play()noexcept;									// ��������
	bool pause()noexcept;									// ��ͣ����
	bool stop()noexcept;									// ֹͣ����
	bool close()noexcept;									// �ر�����
	bool getCurrentTime(DWORD& pos)noexcept;				// ��ȡ��ǰ����ʱ��
	bool getTotalTime(DWORD& time)noexcept;					// ��ȡ������ʱ��
	bool setVolume(size_t nVolumeValue)noexcept;			// ����������С
	bool setStartTime(size_t start)noexcept;				// ���ò���λ��
};



/************************************************
 *              class MusicData                 *
 * ���ݲ�����                                   *
 * ����Ӧ�����ݽ��в���                         *
 * ����MusicMCI��CmdMusicPlayer���н���         *
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
	size_t status = 0;										// ����״̬ 0δ���� 1���ڲ��� 2��ͣ���� 3�������
	size_t volume = 400;									// ��ǰ������С
	size_t number = 0;										// ���ڲ��������ֱ��
	size_t mode = 2;										// ���ֲ���ģʽ 0�������� 1˳�򲥷�(�б�ѭ��) 2�������

	void openMusic(size_t num);								// �򿪲���������
	void playMusic();										// ��������
	void pauseMusic();										// ��ͣ����
	void stopMusic();										// ֹͣ����
	void closeMusic();										// �ر�����
	void setMusicVolume(size_t vol);						// ����������С
	bool setMusicStartTime(size_t start);					// ���ò���λ��
	int getMusicCurrentTime();								// ��ȡ��ǰ�������ֵĵ�ǰ����ʱ��
	int getMusicTotalTime();								// ��ȡ��ǰ�������ֵ���ʱ��

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

	friend class GuiMusicPlayer;
};

wstring stringTowstring(const string& str);					// string ת wstring



/************************************************
 *              class GuiMusicPlayer            *
 * ������                                       *
 * ���û����н���                               *
 * ��������������͸�MusicDate���д���          *
 ***********************************************/

class GuiMusicPlayer
{
private:
	MusicData musicData;									// MusicData �Ķ���
	IMAGE img;												// ����ͼƬ
	bool bList = false;										// �б��Ƿ�չ��
	array<int, 2> numRange;									// �����б���ʾ�����ַ�Χ numRange[0]ָ����ʾ�������б�musicData.musicName�Ŀ�ʼ
															// numRange[1]ָ��musicData.musicName�ڲ����б�����һ��
	static const int WIDTH = 960;							// ���ڿ��
	static const int HEIGHT = 640;							// ���ڸ߶�

	GuiMusicPlayer();										// ˽�е�Ĭ�Ϲ��캯��
	bool findBgPicture()noexcept;							// �����Ƿ���ڱ���ͼƬ
	void ui();												// ����̬ ui
	void drawStartPause();									// ����ʼ��ͣ��
	void drawPlayInformation();								// ��ʾ������Ϣ
public:
	GuiMusicPlayer(const GuiMusicPlayer&) = delete;			// ���ƶ���������
	GuiMusicPlayer(GuiMusicPlayer&&) = default;
	GuiMusicPlayer& operator=(const GuiMusicPlayer&) = delete;
	GuiMusicPlayer& operator=(GuiMusicPlayer&&) = default;
	~GuiMusicPlayer();

	static GuiMusicPlayer& singleton();						// ���ظ���Ψһ��������ã�����ģʽ
	void run();												// �ⲿ���õĽӿڣ������¼�ѭ��
};