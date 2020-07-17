/**************************************************************
 * ����     : ��Զ�Ĳ��                                      *
 * QQ       : 2237505658                                      *
 * Emall    : 2237505658@qq.com                               *
 * ����޸� : 2020/7/17                                       *
 **************************************************************/


#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
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

// ����Ҫ������ Debug ģʽ�¿����ú꣬�� Release ģʽ��ע�͸ú�
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
	bool open(LPCWSTR music)noexcept;						// ������
	bool play()noexcept;									// ��������
	bool pause()noexcept;									// ��ͣ����
	bool stop()noexcept;									// ֹͣ����
	bool close()noexcept;									// �ر�����
	bool getCurrentTime(DWORD& pos)noexcept;				// ��ȡ��ǰ����ʱ��
	bool getTotalTime(DWORD& time)noexcept;					// ��ȡ������ʱ��
	bool setVolume(size_t volume)noexcept;					// ����������С
	bool setStartTime(size_t start)noexcept;				// ���ò���λ��
};



/************************************************
 *              class MusicData                 *
 * ���ݲ�����                                   *
 * ����Ӧ�����ݽ��в���                         *
 * ����MusicMCI��GuiMusicPlayer���н���         *
 ***********************************************/

class MusicData
{
private:
	MusicMCI musicMci;										// ִ�����ֲ��Ų����Ķ���
	string filePath;										// Ҫ���ҵ��ļ�·��
	string musicFormat = "mp3";								// Ҫ���ҵ��ļ���ʽ
	wstring nowMusicName;									// ���ڲ�����������
	vector<string> musicPathName;							// �洢ȫ·�������ļ���
	vector<wstring> musicName;								// �洢�����ļ���
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
	int getMusicCurrentTime();								// ��ȡ���ֵĵ�ǰ����ʱ��
	int getMusicTotalTime();								// ��ȡ���ֵ���ʱ��

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
	IMAGE backgroundImage;									// ����ͼƬ
	bool playList = false;									// �����б��Ƿ�չ��
	bool quit = false;										// �Ƿ��˳�
	array<int, 2> numRange;									// �����б���ʾ�����ַ�Χ numRange[0] Ϊ musicData.musicName
															// ��ʾ�������б�ʼ���±�
															// numRange[1] Ϊ�ڲ����б�Ľ����±�
	const int WIDTH = 960;									// ���ڿ��
	const int HEIGHT = 640;									// ���ڸ߶�

	GuiMusicPlayer();										// ˽�е�Ĭ�Ϲ��캯��
	bool findBackgroundPicture()noexcept;					// �����Ƿ���ڱ���ͼƬ
	void ui();												// ����̬ ui

	void event();											// �����¼������
	void mouseEvent(const MOUSEMSG& mouse);					// ����¼�
	void keyBoardEvent(int key);							// �����¼�
	void timerEvent();										// ��ʱ���¼�
	void drawEvent();										// ��ͼ�¼�

	bool mouseQuitEvent(const MOUSEMSG&);					// ����˳��¼�
	bool mousePlayListArrowEvent(const MOUSEMSG&);			// ��겥���б��ͷ�¼�
	bool mousePlayListEvent(const MOUSEMSG&);				// ��겥���б��¼�
	bool mousePlayListScrollEvent(const MOUSEMSG&);			// ��겥���б�����¼�
	bool mousePlayListScrollBarEvent(const MOUSEMSG&);		// ��겥���б�������¼�
	bool mouseMusicProgressBarEvent(const MOUSEMSG&);		// ������ֽ������¼�
	bool mousePlayPauseButtonEvent(const MOUSEMSG&);		// ��겥����ͣ��ť�¼�
	bool mousePreviousMusicButtonEvent(const MOUSEMSG&);	// �����һ����ť�¼�
	bool mouseNextMusicButonEvent(const MOUSEMSG&);			// �����һ����ť�¼�
	bool mouseVolumeBarEvent(const MOUSEMSG&);				// ����������¼�
	bool mousePlayModeEvent(const MOUSEMSG&);				// ��겥��ģʽ�¼�

	void timerPlayMusicEvent();								// ��ʱ�����������¼�

	void drawBackgroundEvent();								// ������ͼ�¼�
	void drawPlayListArrowEvent();							// �������б��ͷ�¼�
	void drawPlayListEvent();								// �������б��¼�
	void drawPlayListScrollBarEvent();						// �������б�������¼�
	void drawMusicProgressBarEvent();						// �����ֽ������¼�
	void drawMusicNameEvent();								// ���������¼�
	void drawMusicTimeEvent();								// ������ʱ���¼�
	void drawPlayPauseButtonEvent();						// ��������ͣ��ť�¼�
	void drawVolumeBarEvent();								// ���������¼�
	void drawPlayModeEvent();								// ������ģʽ�¼�

public:
	GuiMusicPlayer(const GuiMusicPlayer&) = delete;			// ���ƶ���������
	GuiMusicPlayer(GuiMusicPlayer&&) = default;
	GuiMusicPlayer& operator=(const GuiMusicPlayer&) = delete;
	GuiMusicPlayer& operator=(GuiMusicPlayer&&) = default;
	~GuiMusicPlayer();

	static GuiMusicPlayer& singleton();						// ���ظ���Ψһ��������ã�����ģʽ
	void run();												// �ⲿ���õĽӿ�
};