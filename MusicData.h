#pragma once
#include "MusicMCI.h"


/************************************************
 *              class MusicData                 *
 * 数据操作层                                   *
 * 对相应的数据进行操作                         *
 * 并于MusicMCI和GuiMusicPlayer进行交互         *
 ************************************************/

class MusicData
{
private:
	MusicMCI musicMci;										// 执行音乐播放操作的对象
	string filePath;										// 要查找的文件路径
	string musicFormat = "mp3";								// 要查找的文件格式
	wstring nowMusicName;									// 正在操作的音乐名
	vector<string> musicPathName;							// 存储全路径音乐文件名
	vector<wstring> musicName;								// 存储音乐文件名
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
	int getMusicCurrentTime();								// 获取音乐的当前播放时间
	int getMusicTotalTime();								// 获取音乐的总时间

	void getFilePath();										// 获取搜索路径
	void findMusicName(const string& path);					// 寻找音乐文件
	void wFilePath(fstream& file);							// 将 filePath 写入文件 filePath.ini
	void rFilePath(fstream& file);							// 将文件 filePath.ini 读取到 filePath
	void wFileMusic(fstream& file);							// 将 musicPathName 写入文件 music.mn
	void rFileMusic(fstream& file);							// 将文件 music.mn 读取到 musicPathName

public:
	MusicData();											// 默认构造函数
	~MusicData();											// 析构函数

	friend class GuiMusicPlayer;
};

wstring stringTowstring(const string& str);					// string 转 wstring