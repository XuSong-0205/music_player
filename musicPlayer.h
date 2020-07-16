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
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	// 设置入口地址 隐藏cmd窗口
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
	virtual ~MusicMCI();									// 虚析构函数
	bool open(LPCWSTR strSongPath)noexcept;					// 打开音乐
	bool play()noexcept;									// 播放音乐
	bool pause()noexcept;									// 暂停音乐
	bool stop()noexcept;									// 停止播放
	bool close()noexcept;									// 关闭音乐
	bool getCurrentTime(DWORD& pos)noexcept;				// 获取当前播放时间
	bool getTotalTime(DWORD& time)noexcept;					// 获取音乐总时长
	bool setVolume(size_t nVolumeValue)noexcept;			// 设置音量大小
	bool setStartTime(size_t start)noexcept;				// 设置播放位置
};



/************************************************
 *              class MusicData                 *
 * 数据操作层                                   *
 * 对相应的数据进行操作                         *
 * 并于MusicMCI和CmdMusicPlayer进行交互         *
 ***********************************************/

class MusicData
{
private:
	string filePath;										// 要查找的文件路径
	string musicFormat = "mp3";								// 要查找的文件格式
	wstring nowMusicName;									// 正在操作的音乐名
	vector<string> musicPathName;							// 存储全路径音乐文件名
	vector<wstring> musicName;								// 存储音乐文件名
	MusicMCI musicMci;										// 执行音乐播放操作的对象
	size_t status = 0;										// 播放状态 0未播放 1正在播放 2暂停播放 3播放完毕
	size_t volume = 400;									// 当前音量大小
	size_t number = 0;										// 正在操作的音乐编号
	size_t mode = 2;										// 音乐播放模式 0单个播放 1顺序播放(列表循环) 2随机播放

	void openMusic(size_t num);								// 打开并播放音乐
	void playMusic();										// 播放音乐
	void pauseMusic();										// 暂停播放
	void stopMusic();										// 停止播放
	void closeMusic();										// 关闭音乐
	void setMusicVolume(size_t vol);						// 设置音量大小
	bool setMusicStartTime(size_t start);					// 设置播放位置
	int getMusicCurrentTime();								// 获取当前播放音乐的当前播放时间
	int getMusicTotalTime();								// 获取当前播放音乐的总时间

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

	friend class GuiMusicPlayer;
};

wstring stringTowstring(const string& str);					// string 转 wstring



/************************************************
 *              class GuiMusicPlayer            *
 * 交互层                                       *
 * 与用户进行交互                               *
 * 并将交互结果发送给MusicDate进行处理          *
 ***********************************************/

class GuiMusicPlayer
{
private:
	MusicData musicData;									// MusicData 的对象
	IMAGE img;												// 背景图片
	bool bList = false;										// 列表是否展开
	array<int, 2> numRange;									// 播放列表显示的音乐范围 numRange[0]指向显示在音乐列表musicData.musicName的开始
															// numRange[1]指向musicData.musicName在播放列表的最后一个
	static const int WIDTH = 960;							// 窗口宽度
	static const int HEIGHT = 640;							// 窗口高度

	GuiMusicPlayer();										// 私有的默认构造函数
	bool findBgPicture()noexcept;							// 查找是否存在背景图片
	void ui();												// 画静态 ui
	void drawStartPause();									// 画开始暂停键
	void drawPlayInformation();								// 显示播放信息
public:
	GuiMusicPlayer(const GuiMusicPlayer&) = delete;			// 可移动，禁复制
	GuiMusicPlayer(GuiMusicPlayer&&) = default;
	GuiMusicPlayer& operator=(const GuiMusicPlayer&) = delete;
	GuiMusicPlayer& operator=(GuiMusicPlayer&&) = default;
	~GuiMusicPlayer();

	static GuiMusicPlayer& singleton();						// 返回该类唯一对象的引用，单例模式
	void run();												// 外部调用的接口，进入事件循环
};