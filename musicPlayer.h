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
 * 音乐播放操作层                               *
 * 进行底层音乐播放                             *
 * 暂停，关闭，设置音量，获取播放时间等操作     *
 ***********************************************/

class MusicMCI
{
private:
	MCIDEVICEID nDeviceID;									// 设备ID

public:
	MusicMCI()noexcept;										// 默认构造函数
	BOOL open(LPCWSTR strSongPath)noexcept;					// 打开音乐
	BOOL play()noexcept;									// 播放音乐
	BOOL pause()noexcept;									// 暂停音乐
	BOOL stop()noexcept;									// 停止播放
	BOOL close()noexcept;									// 关闭音乐
	BOOL getPlayBackTime(DWORD& pos)noexcept;				// 获取播放时间
	BOOL getMusicTime(DWORD& time)noexcept;					// 获取音乐总时长
	BOOL setVolume(size_t nVolumeValue)noexcept;			// 音量调节
};


/************************************************
 *              class MusicData                 *
 * 数据操作层                                   *
 * 对相应的数据进行操作                         *
 * 并于MusicMCI和MusicPlayer进行交互            *
 ***********************************************/

class MusicData
{
private:
	string filePath = "";									// 要查找的文件路径
	string musicFormat = "mp3";								// 要查找的文件格式
	string nowMusicName;									// 正在操作的音乐名
	vector<string> musicPathName;							// 存储全路径音乐文件名
	vector<string> musicName;								// 存储音乐文件名
	MusicMCI musicMci;										// 执行音乐播放操作的对象
	size_t status = 0;										// 播放状态 0未播放 1正在播放 2暂停播放 3播放完毕
	size_t volume = 500;									// 当前音量大小
	size_t number = 0;										// 正在操作的音乐编号
	size_t mode = 2;										// 音乐播放模式 0单个播放 1顺序播放(列表循环) 2随机播放

	void openMusic(size_t num);								// 打开并播放音乐
	void playMusic();										// 播放音乐
	void pauseMusic();										// 暂停播放
	void stopMusic();										// 停止播放
	void closeMusic();										// 关闭音乐
	void setMusicVolume(size_t vol);						// 设置音乐
	int getPlayerBackTimeMusic();							// 获取当前播放音乐的当前播放时间
	int getTotalTime();										// 获取当前播放音乐的总时间

	wstring stringTowstring(const string& str);				// string转wstring
	void getFilePath();										// 获取搜索路径
	void findMusicName(const string& path);					// 寻找音乐文件
	void wFilePath(fstream& file);							// 将filePath写入文件filePath.ad
	void rFilePath(fstream& file);							// 将文件filePath.ad读取到filePath
	void wFileMusic(fstream& file);							// 将musicPathName写入文件music.mn
	void rFileMusic(fstream& file);							// 将文件music.mn读取到musicPathName
	void deleteMusicData(vector<int>& vec);					// 删除与vec中数据相同下标的歌曲
public:
	MusicData();											// 默认构造函数
	~MusicData();											// 析构函数

	friend class MusicPlayer;
};


/************************************************
 *              class MusicPlayer               *
 * 交互层                                       *
 * 与用户进行交互                               *
 * 并将交互结果发送给MusicDate进行处理          *
 ***********************************************/

class MusicPlayer
{
private:
	MusicData musicData;									// 音乐数据类对象

	void pos(short x, short y)noexcept;						// 光标函数
	void setMusicVolume();									// 设置音乐播放音量
	void setFilePath();										// 设置文件搜索路径
	void setPlayMode();										// 设置音乐播放模式
	void deleteMusic();										// 从播放列表中删除音乐
	void chooseMusicPlay();									// 在音乐列表中选择音乐播放
	void showMusicName();									// 显示音乐名称列表
	void showPlayInformation();								// 显示音乐的播放信息
public:
	MusicPlayer();											// 默认构造函数
	~MusicPlayer();											// 析构函数
	int chooseFunction();									// 功能选择
};