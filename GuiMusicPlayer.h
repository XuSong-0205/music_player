#pragma once
#include "MusicData.h"


/************************************************
 *              class GuiMusicPlayer			*
 * 交互层										*
 * 与用户进行交互								*
 * 并将交互结果发送给MusicDate进行处理			*
 ************************************************/

class GuiMusicPlayer
{
private:
	MusicData musicData;									// MusicData 的对象
	IMAGE backgroundImage;									// 背景图片
	bool haveBackgroundImage;								// 是否存在背景图
	bool playList = false;									// 播放列表是否展开
	bool quit = false;										// 是否退出
	int position = 0;										// 每一位对应一个部件的状态
															// 0 - 4 位表示在音乐列表的绝对歌曲位置
															// 5, 10 位滚动条，进度条
															// 11，12，13 位依次对应位置 上一曲，开始（暂停），下一曲
															// 14，15 位音量条，播放模式
	array<int, 2> numRange;									// 播放列表显示的音乐范围 numRange[0] 为 musicData.musicName
															// 显示在音乐列表开始的下标
															// numRange[1] 为在播放列表的结束下标
	const int WIDTH = 960;									// 窗口宽度
	const int HEIGHT = 640;									// 窗口高度

	GuiMusicPlayer();										// 私有的默认构造函数
	bool findBackgroundPicture()noexcept;					// 查找是否存在背景图片

	void event();											// 所有事件的入口
	void mouseEvent(const MOUSEMSG& mouse);					// 鼠标事件
	void keyBoardEvent(int key);							// 键盘事件
	void timerEvent();										// 定时器事件
	void drawEvent();										// 绘图事件

	bool mouseQuitEvent(const MOUSEMSG&);					// 鼠标退出事件
	bool mousePlayListArrowEvent(const MOUSEMSG&);			// 鼠标播放列表箭头事件
	bool mousePlayListEvent(const MOUSEMSG&);				// 鼠标播放列表事件
	bool mousePlayListScrollEvent(const MOUSEMSG&);			// 鼠标播放列表滚动事件
	bool mousePlayListScrollBarEvent(const MOUSEMSG&);		// 鼠标播放列表滚动条事件
	bool mouseMusicProgressBarEvent(const MOUSEMSG&);		// 鼠标音乐进度条事件
	bool mousePlayPauseButtonEvent(const MOUSEMSG&);		// 鼠标播放暂停按钮事件
	bool mousePreviousMusicButtonEvent(const MOUSEMSG&);	// 鼠标上一曲按钮事件
	bool mouseNextMusicButonEvent(const MOUSEMSG&);			// 鼠标下一曲按钮事件
	bool mouseVolumeBarEvent(const MOUSEMSG&);				// 鼠标音量条事件
	bool mousePlayModeEvent(const MOUSEMSG&);				// 鼠标播放模式事件

	bool keyboardSpaceEvent(int key);						// 键盘空格键事件
	bool keyboardEnterEvent(int key);						// 键盘回车键事件
	bool keyboardUpDownEvent(int key);						// 键盘上下键事件
	bool keyboardLeftRightEvent(int key);					// 键盘左右键事件

	void timerPlayMusicEvent();								// 定时器播放音乐事件

	void drawUiEvent();										// 画静态 ui
	void drawPlayListArrowEvent();							// 画播放列表箭头事件
	void drawPlayListEvent();								// 画播放列表事件
	void drawPlayListScrollBarEvent();						// 画播放列表滚动条事件
	void drawMusicProgressBarEvent();						// 画音乐进度条事件
	void drawMusicNameEvent();								// 画音乐名事件
	void drawMusicTimeEvent();								// 画音乐时长事件
	void drawPlayPauseButtonEvent();						// 画播放暂停按钮事件
	void drawVolumeBarEvent();								// 画音量条事件
	void drawPlayModeEvent();								// 画播放模式事件

public:
	GuiMusicPlayer(const GuiMusicPlayer&) = delete;			// 可移动，禁复制
	GuiMusicPlayer(GuiMusicPlayer&&) = default;
	GuiMusicPlayer& operator=(const GuiMusicPlayer&) = delete;
	GuiMusicPlayer& operator=(GuiMusicPlayer&&) = default;
	~GuiMusicPlayer();

	static GuiMusicPlayer& singleton();						// 返回该类唯一对象的引用，单例模式
	void run();												// 外部调用的接口
};