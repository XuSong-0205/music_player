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
	MCIDEVICEID nDeviceID;									// 设备ID

public:
	MusicMCI()noexcept;										// 默认构造函数
	~MusicMCI()noexcept;									// 析构函数
	BOOL open(LPCWSTR strSongPath)noexcept;					// 打开音乐
	BOOL play()noexcept;									// 播放音乐
	BOOL pause()noexcept;									// 暂停音乐
	BOOL stop()noexcept;									// 停止播放
	BOOL close()noexcept;									// 关闭音乐
	BOOL getPlayBackTime(DWORD& pos)noexcept;				// 获取播放时间
	BOOL setVolume(int nVolumeValue)noexcept;				// 音量调节
};


/***********************************************
 *                class MusicPlayer            *
 ***********************************************/


class MusicPlayer
{
private:
	string filePath="E:\\音乐";								// 要查找的文件路径
	string musicFormat = "mp3";								// 要查找的文件格式
	string nowMusicName;									// 正在播放的音乐
	fstream file;											// 存储全路径音乐名的文件对象
	vector<string> musicPathName;							// 存储全路径音乐文件名
	vector<string> musicName;								// 存储音乐文件名
	MusicMCI musicMci;										// 执行音乐播放操作的对象

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