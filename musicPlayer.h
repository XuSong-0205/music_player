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
	BOOL getMusicTime(DWORD& time)noexcept;					// 获取音乐总时长
	BOOL setVolume(int nVolumeValue)noexcept;				// 音量调节
};


/***********************************************
 *                class MusicPlayer            *
 ***********************************************/


class MusicPlayer
{
private:
	string filePath="";										// 要查找的文件路径
	string musicFormat = "mp3";								// 要查找的文件格式
	string nowMusicName;									// 正在播放的音乐
	fstream file;											// 存储全路径音乐名的文件对象
	vector<string> musicPathName;							// 存储全路径音乐文件名
	vector<string> musicName;								// 存储音乐文件名
	MusicMCI musicMci;										// 执行音乐播放操作的对象
	int status = 0;											// 播放状态 0未播放 1正在播放 2暂停播放 3播放完毕
	int vole = 500;											// 当前音量大小
	int number = 0;											// 正在播放的音乐编号
	int mode = 0;											// 音乐播放模式 0单个播放 1顺序播放(列表循环) 2随机播放

	wstring stringTowstring(const string& str);
	void findMusicName(const string& path);					// 寻找音乐文件
	void getFilePath();										// 获取搜索路径
	void wFile();
	void rFile();
	void openMusic(int num);
	void playMusic();
	void pauseMusic();
	void stopMusic();
	void closeMusic();
	void setVolumeMusic(int volume);
	int getPlayerBackTimeMusic();							// 获取当前播放音乐的当前播放时长
	int getTotalTime();										// 获取当前播放音乐的总时长
	void setFilePath();										// 设置文件搜索路径
	void setPlayMode();										// 设置音乐播放模式  顺序播放（列表循环） 随机播放 单个播放


	void pos(int x, int y);									// 设置光标位置
public:
	MusicPlayer();
	~MusicPlayer();
	void showMusicName();
	void showPlayTime();
	void chooseMusicPlay();
	int chooseFunction();
};