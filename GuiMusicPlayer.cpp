#include "GuiMusicPlayer.h"


/****************************************************************************************************************
 *  class GuiMusicPlayer																						*
 ****************************************************************************************************************/

/****************************************************************
 * GuiMusicPlayer private method								*
 ****************************************************************/

// 私有的构造函数
GuiMusicPlayer::GuiMusicPlayer()
{
	srand(time(nullptr) & 0xffffffff);
	initgraph(WIDTH, HEIGHT

#ifdef _DEBUG
		, SHOWCONSOLE
#endif	// _DEBUG
	);

	if (musicData.musicName.empty())
	{
		numRange = { 0, 0 };
	}
	else if (musicData.musicName.size() >= 12)
	{
		numRange = { 0, 12 };
	}
	else
	{
		numRange = { 0, static_cast<int>(musicData.musicName.size() - 1) };
	}

	if (findBackgroundPicture())
	{
		haveBackgroundImage = true;										// 存在背景图片
		loadimage(&backgroundImage, L"background.jpg", WIDTH, HEIGHT);	// 加载背景图片
	}
	else
	{
		haveBackgroundImage = false;
	}
}

// 查找背景图片是否存在
// 查找到返回 true，否则返回 false
bool GuiMusicPlayer::findBackgroundPicture()noexcept
{
	WIN32_FIND_DATA fileinfo;													// 文件信息 
	if (FindFirstFile(L"background.jpg", &fileinfo) != INVALID_HANDLE_VALUE)	// 查找背景图片
		return true;

	return false;
}


/****************************************************************
 * 事件系统														*
 ****************************************************************/

// 事件循环的入口
void GuiMusicPlayer::event()
{
	MOUSEMSG mouse;
	int key = 0;
	BeginBatchDraw();

	// 进入事件循环
	while (true)
	{
		// 如果有鼠标消息，进入鼠标事件
		if (MouseHit())
		{
			// 获取鼠标消息
			mouse = GetMouseMsg();
			// 清空鼠标消息缓冲区
			FlushMouseMsgBuffer();
			// 处理鼠标事件
			mouseEvent(mouse);
		}

		// 如果有键盘消息，进入键盘事件
		if (_kbhit())
		{
			// 获取键盘按键信息
			key = _getch();
			// 如果读取到的键码等于 224，则为功能键，第二次读取结果为其真实键码
			// 8 - 15 为第一次读取的键码，0 - 7 位为真实键码
			if (key == 224)
			{
				key <<= 8;
				key |= _getch();
			}
			// 处理键盘事件
			keyBoardEvent(key);
		}

		// 定时器事件
		timerEvent();

		// 绘图事件
		drawEvent();

		// 是否退出
		if (quit)
		{
			break;
		}

		Sleep(10);
		FlushBatchDraw();
	}

	EndBatchDraw();
}

// 鼠标事件
void GuiMusicPlayer::mouseEvent(const MOUSEMSG& mouse)
{
	// 处理鼠标退出事件
	if (mouseQuitEvent(mouse))
	{
		return;
	}

	// 处理鼠标播放列表箭头事件
	if (mousePlayListArrowEvent(mouse))
	{
		return;
	}

	// 事件过滤器
	// 若无音乐，下列事件将被过滤
	if (musicData.musicName.empty())
	{
		return;
	}

	// 处理鼠标播放列表事件
	if (mousePlayListEvent(mouse))
	{
		return;
	}

	// 处理鼠标播放列表滚动条事件
	if (mousePlayListScrollBarEvent(mouse))
	{
		return;
	}

	// 处理鼠标音乐进度条事件
	if (mouseMusicProgressBarEvent(mouse))
	{
		return;
	}

	// 处理鼠标播放暂停按钮事件
	if (mousePlayPauseButtonEvent(mouse))
	{
		return;
	}

	// 处理鼠标上一曲按钮事件
	if (mousePreviousMusicButtonEvent(mouse))
	{
		return;
	}

	// 处理鼠标下一曲按钮事件
	if (mouseNextMusicButonEvent(mouse))
	{
		return;
	}

	// 处理鼠标音量调事件
	if (mouseVolumeBarEvent(mouse))
	{
		return;
	}

	// 处理鼠标播放模式事件
	if (mousePlayModeEvent(mouse))
	{
		return;
	}
}

// 键盘事件
void GuiMusicPlayer::keyBoardEvent(int key)
{
	// 处理键盘空格键事件
	if (keyboardSpaceEvent(key))
	{
		return;
	}

	// 处理键盘回车键事件
	if (keyboardEnterEvent(key))
	{
		return;
	}

	// 处理键盘左右键事件
	if (keyboardLeftRightEvent(key))
	{
		return;
	}

	// 处理键盘上下键事件
	if (keyboardUpDownEvent(key))
	{
		return;
	}
}

// 定时器事件
void GuiMusicPlayer::timerEvent()
{
	// 处理定时器音乐播放事件
	timerPlayMusicEvent();
}

// 绘图事件
void GuiMusicPlayer::drawEvent()
{
	// 画静态 ui
	drawUiEvent();

	// 画播放列表箭头事件
	drawPlayListArrowEvent();

	// 画播放列表事件
	drawPlayListEvent();

	// 画进度条事件
	drawMusicProgressBarEvent();

	// 画音乐名事件
	drawMusicNameEvent();

	// 画音乐时长事件
	drawMusicTimeEvent();

	// 画播放暂停按钮事件
	drawPlayPauseButtonEvent();

	// 画音量条事件
	drawVolumeBarEvent();

	// 画播放模式事件
	drawPlayModeEvent();
}


/****************************************************************
 * 鼠标事件														*
 * 鼠标事件在一次事件循环中只会响应一个							*
 ****************************************************************/

// 鼠标退出事件
bool GuiMusicPlayer::mouseQuitEvent(const MOUSEMSG& mouse)
{
	if (mouse.mkLButton && mouse.x >= WIDTH - 40 && mouse.x <= WIDTH - 10
		&& mouse.y >= 10 && mouse.y <= 25)					// 按退出键退出
	{
		quit = true;										// 设置 quit 为 true

		return true;										// 该事件已被处理，返回 true
	}

	return false;
}

// 鼠标播放列表箭头事件
bool GuiMusicPlayer::mousePlayListArrowEvent(const MOUSEMSG& mouse)
{
	if (mouse.mkLButton && mouse.x >= 50 && mouse.x <= 165
		&& mouse.y >= 175 && mouse.y <= 200)								// 播放列表箭头
	{
		playList = !playList;												// 播放列表是否展开

		return true;														// 该事件已被处理
	}

	return false;
}

// 鼠标播放列表事件
bool GuiMusicPlayer::mousePlayListEvent(const MOUSEMSG& mouse)
{
	if (playList && mouse.x > 260 && mouse.x < WIDTH - 20
		&& mouse.y > 50 && mouse.y < HEIGHT - 70)								// 在播放列表
	{
		const size_t ty = (mouse.y - 50) / 40 + numRange.at(0);					// 计算出是播放列表中哪一个编号的歌曲
		position = (position & 0xfffffff0) |
			(static_cast<int>(ty) - numRange.at(0)) + 1;						// 设置在播放列表的绝对位置

		if (mouse.mkLButton)													// 鼠标左键按下
		{
			if (ty >= musicData.musicName.size())								// 是否超出播放列表
			{
				return true;													// 超出则不做处理，直接返回 true				
			}

			if (musicData.status == 0)											// 未播放，开始播放
			{
				musicData.openMusic(ty);										// 打开并播放
			}
			else if (musicData.status == 1)										// 正在播放
			{
				if (musicData.number == ty)										// 若是正在播放的音乐
				{
					musicData.pauseMusic();										// 暂停
				}
				else															// 若不是，关闭原来播放的音乐，打开新音乐
				{
					musicData.closeMusic();
					musicData.openMusic(ty);
				}

			}
			else if (musicData.status == 2)										// 暂停播放
			{
				if (musicData.number == ty)										// 若是刚才的音乐
				{
					musicData.playMusic();										// 继续播放
				}
				else															// 若不是刚才的音乐
				{
					musicData.closeMusic();										// 关闭原来的音乐
					musicData.openMusic(ty);									// 打开新的音乐
				}
			}

			return true;														// 该事件已被处理
		}

		if (mousePlayListScrollEvent(mouse))									// 鼠标播放列表滚动事件
		{
			return true;
		}
	}
	else
	{
		position &= 0xfffffff0;													// 抹去 0 - 4 位
	}

	return false;
}

// 鼠标播放列表滚动事件
// 该事件需在播放列表展开时才会触发
// 该事件属于播放列表事件的一部分
bool GuiMusicPlayer::mousePlayListScrollEvent(const MOUSEMSG& mouse)
{
	if (mouse.wheel / 120)															// 有鼠标滚动消息
	{																				// 更新播放列表显示内容
		const int mouseLen = mouse.wheel / 120;										// 鼠标滚动长度
		const int musicListLen = static_cast<int>(musicData.musicName.size() - 1);	// 音乐列表的长度

		if (musicListLen > 13)
		{
			if (mouseLen >= 0)														// 向上滚动
			{
				numRange.at(0) = (numRange.at(0) - mouseLen < 0 ? 0 : (numRange.at(0) - mouseLen));
			}
			else																	// 向下滚动
			{
				numRange.at(0) = (numRange.at(0) - mouseLen) > (musicListLen - 12)
					? (musicListLen - 12) : (numRange.at(0) - mouseLen);
			}

			numRange.at(1) = numRange.at(0) + 12;
		}

		return true;																// 该事件已被处理
	}

	return false;
}

// 鼠标播放列表滚动条事件
// 该事件需在播放列表展开时才会触发
bool GuiMusicPlayer::mousePlayListScrollBarEvent(const MOUSEMSG& mouse)
{
	if (playList && mouse.x >= WIDTH - 10 && mouse.x <= WIDTH - 2
		&& mouse.y > 40 && mouse.y < HEIGHT - 80)										// 滚动条操作
	{
		position |= (1 << 5);															// 在滚动条范围内

		if (mouse.mkLButton)															// 左键按下
		{
			const int mSize = static_cast<int>(musicData.musicName.size());				// 音乐列表的长度
			const double k0 = 13 / (mSize + 0.0);										// 当前页面音乐占所有音乐的比列
			if (k0 >= 1)
			{
				return false;															// 总音乐数小于等于 13 时，不画滚动条
																						// 所以也不响应滚动条事件，直接返回 false
			}

			const double k1 = numRange.at(0) / (mSize + 0.0);							// 比列位置
			const int length = static_cast<int>((HEIGHT - 106) * k0);					// 滚动条长度，随动
			const int y1 = static_cast<int>(40 + (HEIGHT - 106) * k1) + 3;				// 滚动条的左上角的纵坐标
			const int y2 = y1 + length;													// 右下角纵坐标

			static int prevY = 0;														// 初始化上次的 y 坐标为 0
			if(mouse.y < y1)															// 在滚动条上方
			{
				numRange.at(0) = numRange.at(0) - 12 >= 0 ? numRange.at(0) - 12 : 0;	// 上移一个页面
				numRange.at(1) = numRange.at(0) + 12 < mSize ? numRange.at(0) + 12 : mSize - 1;
				prevY = 0;																// 离开滚动条区域，清 0
			}
			else if (mouse.y >= y1 && mouse.y <= y2)									// 鼠标在滚动条上
			{
				if (prevY == mouse.y)													// 此次点击位置和上次相同
				{
					prevY = 0;															// 清 0 
				}

				if (!prevY)																// 若上次不在滚动条区域
				{
					prevY = mouse.y;													// 记录当前鼠标纵坐标
																						// 记为滚动条的中间值
				}
				else if(prevY != mouse.y)
				{
					const int lenY = mouse.y - prevY;									// 上次到这次的移动距离
					const double moveMusicLen = (HEIGHT - 106) / (mSize + 0.0);			// 移动播放列表中一首歌曲
																						// 所需移动鼠标的最小距离
					if (abs(lenY) >= moveMusicLen)										// 鼠标移动距离达到该距离
					{
						const int moveMusicNum = static_cast<int>(lenY / moveMusicLen);	// 要移动的音乐数
						if (lenY > 0)
						{
							numRange.at(1) = numRange.at(1) + moveMusicNum < mSize ?
								numRange.at(1) + moveMusicNum : mSize - 1;
							numRange.at(0) = numRange.at(1) - 12 >= 0 ?
								numRange.at(1) - 12 : 0;								// 移动
						}
						else if(lenY < 0)
						{
							numRange.at(0) = numRange.at(0) + moveMusicNum >= 0 ?
								numRange.at(0) + moveMusicNum : 0;
							numRange.at(1) = numRange.at(0) + 12 < mSize ?
								numRange.at(0) + 12 : mSize - 1;						// 移动
						}
						
						prevY = mouse.y;												// 更新该值
					}
				}
			}
			else if (mouse.y > y2)														// 在滚动条下方
			{
				numRange.at(1) = numRange.at(1) + 12 < mSize ? numRange.at(1) + 12 : mSize - 1;
				numRange.at(0) = numRange.at(1) - 12 >= 0 ? numRange.at(1) - 12 : 0;	// 下一一个页面
				prevY = 0;																// 离开滚动条区域，清 0
			}
		}

		return true;																	// 该事件已被处理
	}
	else
	{
		position &= ~(1 << 5);
	}

	return false;
}

// 鼠标音乐进度条事件
bool GuiMusicPlayer::mouseMusicProgressBarEvent(const MOUSEMSG& mouse)
{
	if (mouse.y >= HEIGHT - 68 && mouse.y <= HEIGHT - 52 &&
		mouse.x >= 0 && mouse.x <= WIDTH)								// 音乐进度条
	{
		position |= (1 << 10);											// 在进度条

		if (mouse.mkLButton)
		{
			const double k = mouse.x / (WIDTH + 0.0);					// 计算出进度条比列
			const size_t start_time = static_cast<size_t>
				(k * musicData.getMusicTotalTime() * 1000);				// 计算处新的音乐播放时间
			if (musicData.setMusicStartTime(start_time))				// 设置播放位置
			{
				musicData.status = 1;									// 设置当前播放状态为 正在播放
			}

			return true;												// 该事件已被处理
		}
	}
	else
	{
		position &= ~(1 << 10);											// 抹去该位
	}

	return false;
}

// 鼠标播放暂停按钮事件
bool GuiMusicPlayer::mousePlayPauseButtonEvent(const MOUSEMSG& mouse)
{
	if (pow(mouse.x - WIDTH / 2, 2) +
		pow(mouse.y - HEIGHT + 30, 2) <= 400)									// 是否在圆形播放，暂停按钮内
	{																			// 根据播放状态进行操作
		position |= (1 << 12);													// 在开始按钮内

		if (mouse.mkLButton)
		{
			if (musicData.status == 0)											// 若未播放
			{
				musicData.openMusic(musicData.number);							// 打开并播放
			}
			else if (musicData.status == 1)										// 正在播放
			{
				musicData.pauseMusic();											// 暂停
			}
			else if (musicData.status == 2)										// 暂停播放
			{
				musicData.playMusic();											// 继续
			}

			return true;														// 该事件已被处理
		}
	}
	else
	{
		position &= ~(1 << 12);
	}

	return false;
}

// 鼠标上一曲按钮事件
bool GuiMusicPlayer::mousePreviousMusicButtonEvent(const MOUSEMSG& mouse)
{
	if (mouse.x >= WIDTH / 2 - 65 && mouse.x <= WIDTH / 2 - 48 &&
		mouse.y >= HEIGHT - 38 && mouse.y <= HEIGHT - 22)							// 上一曲
	{
		position |= (1 << 11);														// 在上一曲按钮内

		if (mouse.mkLButton)
		{
			if (musicData.status)													// 是否需要关闭音乐
			{
				musicData.closeMusic();
			}

			if (musicData.mode == 0 || musicData.mode == 1)							// 若不是随机播放，上一曲
			{
				musicData.openMusic(musicData.number == 0 ? 
					musicData.musicName.size() - 1 : musicData.number - 1);
			}
			else																	// 是随机播放，随机一曲
			{
				musicData.openMusic(rand() % musicData.musicName.size());
			}

			return true;															// 事件已被处理
		}
	}
	else
	{
		position &= ~(1 << 11);
	}

	return false;
}

// 鼠标下一曲按钮事件
bool GuiMusicPlayer::mouseNextMusicButonEvent(const MOUSEMSG& mouse)
{
	if (mouse.x >= WIDTH / 2 + 48 && mouse.x <= WIDTH / 2 + 65 &&
		mouse.y >= HEIGHT - 38 && mouse.y <= HEIGHT - 22)				// 下一曲
	{
		position |= (1 << 13);											// 在下一曲按钮内

		if (mouse.mkLButton)
		{
			if (musicData.status)
			{
				musicData.closeMusic();
			}

			if (musicData.mode == 0 || musicData.mode == 1)
			{
				musicData.openMusic(musicData.number + 1 > 
					musicData.musicName.size() - 1 ? 0 : musicData.number + 1);
			}
			else
			{
				musicData.openMusic(rand() % musicData.musicName.size());
			}

			return true;												// 该事件已被处理
		}
	}
	else
	{
		position &= ~(1 << 13);
	}

	return false;
}

// 鼠标音量条事件
bool GuiMusicPlayer::mouseVolumeBarEvent(const MOUSEMSG& mouse)
{
	if (mouse.x >= WIDTH / 2 + 140 && mouse.x <= WIDTH / 2 + 240 &&
		mouse.y <= HEIGHT - 25 && mouse.y >= HEIGHT - 35)						// 音量条
	{
		position |= (1 << 14);													// 在音量条内

		if (mouse.mkLButton)
		{
			musicData.setMusicVolume((mouse.x - WIDTH / 2 - 140) * 10);			// 设置音量

			return true;														// 该事件已被处理
		}
	}
	else
	{
		position &= ~(1 << 14);
	}

	return false;
}

// 鼠标播放模式事件
bool GuiMusicPlayer::mousePlayModeEvent(const MOUSEMSG& mouse)
{
	if (mouse.x >= WIDTH / 2 + 300 && mouse.x <= WIDTH / 2 + 360 &&
		mouse.y <= HEIGHT - 25 && mouse.y >= HEIGHT - 40)		// 播放模式
	{
		position |= (1 << 15);									// 在播放模式内

		if (mouse.mkLButton)
		{
			musicData.mode = ++musicData.mode % 3;				// 顺序变更播放模式

			return true;										// 该事件已被处理
		}
	}
	else
	{
		position &= ~(1 << 15);
	}

	return false;
}


/****************************************************************
 * 键盘事件														*
 * 键盘事件在一个事件循环中也应只响应一个						*
 ****************************************************************/

// 键盘空格键事件
// 播放，暂停当前音乐
bool GuiMusicPlayer::keyboardSpaceEvent(int key)
{
	if (key == 0X20)
	{
		if (musicData.status == 1) musicData.pauseMusic();		// 若正在播放，暂停
		else if (musicData.status == 2) musicData.playMusic();	// 若暂停，开始播放

		return true;											// 该事件以处理
	}

	return false;												// 该事件未处理
}

// 键盘回车键事件
// 若鼠标不在播放列表中，将打开或关闭播放列表
// 若在播放列表，则播放当前所指向的音乐（红色）
bool GuiMusicPlayer::keyboardEnterEvent(int key)
{
	if (key == 0XD)
	{
		size_t currIndex = position & 0XF;
		if (currIndex != 0)													// 是否在播放列表中
		{
			currIndex += numRange.at(0) - 1;
			if (musicData.status != 0) musicData.closeMusic();				// 不是未播放状态，关闭当前音乐

			musicData.openMusic(currIndex);									// 打开并播放 currIndex 指向的音乐
		}
		else
		{
			playList = !playList;											// 关闭或打开播放列表
		}

		return true;
	}

	return false;
}

// 键盘上下键事件
// 若不在播放列表，则进行上一曲，下一曲操作
// 若在播放列表中，进行播放列表中音乐的选取，滚动播放列表
bool GuiMusicPlayer::keyboardUpDownEvent(int key)
{
	if (key == ((0XE0 << 8) | 0X48) || key == ((0XE0 << 8) | 0X50))				// 是否为上，下键
	{
		int dir = key == ((0XE0 << 8) | 0X48) ? -1 : 1;							// 上 -1,下 1
		int currIndex = position & 0XF;											// 在当前音乐播放列表中所指的位置
		
		if (currIndex == 0)														// 鼠标不在播放列表，进行上一曲，下一曲操作
		{
			if (musicData.status) musicData.closeMusic();						// 关闭之前的音乐

			if (musicData.mode == 2)											// 随机播放
			{
				musicData.openMusic(rand() % musicData.musicName.size());	
			}
			else
			{
				if (dir == -1)													// 上一曲
				{
					musicData.openMusic(musicData.number == 0 ?
						musicData.musicName.size() - 1 : musicData.number - 1);
				}
				else if (dir == 1)												// 下一曲
				{
					musicData.openMusic(musicData.number + 1 >=
						musicData.musicName.size() ? 0 : musicData.number + 1);
				}
			}
		}
		else
		{
			if (currIndex == 1 && dir == -1)									// 在播放列表第一个，且向上翻页
			{
				numRange.at(0) -= numRange.at(0) > 0 ? 1 : 0;					// 向上滚动播放列表
				numRange.at(1) = numRange.at(0) + 12;
			}
			else if (currIndex == 13 && dir == 1)								// 在播放列表最后一个，且向下翻页
			{
				numRange.at(1) += numRange.at(1) <								// 向下滚动播放列表
					musicData.musicName.size() - 1 ? 1 : 0;
				numRange.at(0) = numRange.at(1) - 12;
			}
			else
			{
				position += dir;												// 向上或向下在播放列表中移动
			}
		}

		return true;
	}

	return false;
}

// 键盘左右键事件
// 左键减小音量，右键增大音量
bool GuiMusicPlayer::keyboardLeftRightEvent(int key)
{
	auto vol = musicData.volume;
	if (key == ((0XE0 << 8) | 0X4B))											// 左键，音量减小 50
	{
		vol = vol < 50 ? 0 : vol - 50;
		musicData.setMusicVolume(vol);

		return true;															// 该事件已被处理
	}
	else if (key == ((0XE0 << 8) | 0X4D))										// 右键，音量增加 50
	{
		vol = vol > 950 ? 1000 : vol + 50;
		musicData.setMusicVolume(vol);

		return true;
	}

	return false;
}


/****************************************************************
 * 定时器事件													*
 * 定时器事件依照次序挨个响应									*
 ****************************************************************/

// 定时器音乐播放事件
void GuiMusicPlayer::timerPlayMusicEvent()
{
	if (musicData.status != 1)												// 若不是正在播放状态，退出
	{
		return;
	}

	const auto t0 = musicData.getMusicCurrentTime();						// 获取当前音乐时长
	const auto t1 = musicData.getMusicTotalTime();							// 获取音乐总时长

	if (t0 == t1)															// 若相等，则此音乐播放完毕
	{
		musicData.closeMusic();												// 首先，关闭音乐
																			// 查询播放模式，进行下一步播放
		if (musicData.mode == 0)											// 单曲循环
		{
			musicData.openMusic(musicData.number);
		}
		else if (musicData.mode == 1)										// 顺序播放
		{
			musicData.openMusic(musicData.number + 1 > musicData.musicName.size() - 1 ?
				0 : musicData.number + 1);
		}
		else if (musicData.mode == 2)										// 随机播放
		{
			musicData.openMusic(rand() % musicData.musicName.size());
		}
	}
}


/****************************************************************
 * 绘图事件														*
 * 绘图事件依照次序挨个响应										*
 ****************************************************************/

 // 画程序的整体 ui 界面(静态)
void GuiMusicPlayer::drawUiEvent()
{
	if (haveBackgroundImage)											// 如果存在背景图
	{
		putimage(0, 0, &backgroundImage);								// 显示背景图片
	}
	else																// 若不存在
	{
		setbkcolor(WHITE);												// 设置填充色 白色
		cleardevice();													// 使用背景色清空画面
	}

	constexpr COLORREF c0 = 0XAA00AA;									// 紫色
	constexpr COLORREF c1 = 0X0000AA;									// 红色

	settextcolor(c1);																		// 字体颜色
	settextstyle(15, 0, L"宋体");															// 字体样式
	setbkmode(TRANSPARENT);																	// 文字输出背景透明
	outtextxy(WIDTH - 40, 10, L"退出");
	setlinecolor(0XE8E8E8);
	line(0, 40, WIDTH, 40);																	// 上边界线
	line(0, HEIGHT - 60, WIDTH, HEIGHT - 60);												// 画进度条
	line(0, HEIGHT - 59, WIDTH, HEIGHT - 59);

	outtextxy(50, 60, L"我的音乐");
	outtextxy(50, 180, L"播放列表");

	/***************************************************************************************
	 * 画播放框 ui                                                                         *
	 ***************************************************************************************/

	// 画上一曲按键
	COLORREF color = position & (1 << 11) ? c1 : c0;										// 选择颜色
	setlinecolor(color);																	// 设置画线颜色
	setfillcolor(color);																	// 设置填充颜色
	
	line(WIDTH / 2 - 64, HEIGHT - 37, WIDTH / 2 - 64, HEIGHT - 23);							// |
	line(WIDTH / 2 - 65, HEIGHT - 37, WIDTH / 2 - 65, HEIGHT - 23);

	const array<array<POINT, 2>, 3> a0{ WIDTH / 2 - 48,HEIGHT - 38,
		WIDTH / 2 - 48,HEIGHT - 22,WIDTH / 2 - 60,HEIGHT - 30 };
	solidpolygon(&a0.at(0).at(0), 3);														// <|
	// 画上一曲按键结束

	// 画下一曲按键
	color = position & (1 << 13) ? c1 : c0;													// 选择颜色
	setlinecolor(color);																	// 设置画线颜色
	setfillcolor(color);																	// 设置填充颜色

	const array<array<POINT, 2>, 3> a1{ WIDTH / 2 + 48,HEIGHT - 38,
		WIDTH / 2 + 48,HEIGHT - 22,WIDTH / 2 + 60,HEIGHT - 30 };
	solidpolygon(&a1.at(0).at(0), 3);														// |>

	line(WIDTH / 2 + 64, HEIGHT - 37, WIDTH / 2 + 64, HEIGHT - 23);							// |
	line(WIDTH / 2 + 65, HEIGHT - 37, WIDTH / 2 + 65, HEIGHT - 23);
	// 画开始键按键结束

	// 画音量条，喇叭
	color = position & (1 << 14) ?  c1 : c0;												// 选择颜色
	setlinecolor(color);																	// 设置画线颜色
	setfillcolor(color);																	// 设置填充颜色

	const array<POINT, 14> a2{ WIDTH / 2 + 120,HEIGHT - 34,
		WIDTH / 2 + 125,HEIGHT - 34,WIDTH / 2 + 129,HEIGHT - 38,
		WIDTH / 2 + 129,HEIGHT - 22,WIDTH / 2 + 125,HEIGHT - 26,
		WIDTH / 2 + 120,HEIGHT - 26,WIDTH / 2 + 120,HEIGHT - 34 };
	polyline(&a2.at(0), 7);																	// 画小喇叭

	arc(WIDTH / 2 + 125, HEIGHT - 35, WIDTH / 2 + 135,
		HEIGHT - 25, -3.1415926 / 3, 3.1415926 / 3);										// 画圆弧

	setlinecolor(0XE8E8E8);																	// 浅灰色
	line(WIDTH / 2 + 140, HEIGHT - 30, WIDTH / 2 + 240, HEIGHT - 30);						// 画音量条
	// 画音量条，喇叭结束
}

// 画播放列表箭头事件
void GuiMusicPlayer::drawPlayListArrowEvent()
{
	setlinecolor(0X0000AA);
	if (playList)														// 画播放列表的箭头
	{
		line(140, 185, 162, 185);
		line(159, 179, 165, 185);
		line(159, 191, 165, 185);
	}
	else
	{
		line(140, 185, 162, 185);
		line(137, 185, 143, 179);
		line(137, 185, 143, 191);
	}
}

// 画播放列表事件
void GuiMusicPlayer::drawPlayListEvent()
{
	if (!playList)
	{
		return;																		// 未展开播放列表，返回
	}

	settextstyle(14, 0, L"宋体");													// 字体大小，样式
	if (!musicData.musicName.empty())												// 显示播放列表
	{
		for (int i = 0; i <= 12; ++i)
		{
			if (i + numRange.at(0) > numRange.at(1))
			{
				break;																// 超出显示范围，退出
			}

			COLORREF color = i == (position & 0x0000000f) - 1 ? 0X0000AA : 0XAA00AA;// 选择颜色
			settextcolor(color);

			wstring s0;
			if (musicData.musicName.at(i + numRange.at(0)).size() > 60)				// 超出一定长度的名字只显示一部分
			{
				s0 = musicData.musicName.at(i + numRange.at(0)).substr(0, 60) + L"...";
			}
			else
			{
				s0 = musicData.musicName.at(i + numRange.at(0));
			}

			outtextxy(260, 40 + 20 + i * 40, s0.c_str());							// 再播放列表显示音乐名
		}

		drawPlayListScrollBarEvent();												// 仅在播放列表不为空时，显示滚动条
	}
	else
	{
		outtextxy(260, 40 + 15, L"音乐列表为空！");
	}
}

// 画播放列表滚动条事件
// 仅在展开播放列表时显示滚动条
// 滚动条的绘制属于画播放列表的一部分
void GuiMusicPlayer::drawPlayListScrollBarEvent()
{
	COLORREF color = position & (1 << 5) ? 0X0000AA : 0XAA00AA;							// 选择颜色
	setfillcolor(color);

	const double k0 = 13 / (musicData.musicName.size() + 0.0);							// 当前页面音乐占所有音乐的比列
	if (k0 >= 1)
	{
		return;																			// 音乐总数小于等于当前页面显示的音乐数
																						// 直接返回，不画滚动条
	}
	const int length = static_cast<int>((HEIGHT - 106) * k0);							// 滚动条长度，随总歌曲数变化
	const double k1 = numRange.at(0) / (musicData.musicName.size() + 0.0);				// 当前音乐在整个播放列表中的比列
	const int y1 = static_cast<int>(40 + (HEIGHT - 106) * k1) + 3;						// 滚动条的左上角的纵坐标
	solidrectangle(WIDTH - 10, y1, WIDTH - 2, y1 + length);								// 画滚动条
}

// 画音乐进度条事件
void GuiMusicPlayer::drawMusicProgressBarEvent()
{
	const int t0 = musicData.getMusicCurrentTime();										// 当前时长
	const int t1 = musicData.getMusicTotalTime();										// 总时长
	setlinecolor(0XE8E8E8);																// 浅灰色
	line(0, HEIGHT - 60, WIDTH, HEIGHT - 60);
	line(0, HEIGHT - 59, WIDTH, HEIGHT - 59);

	COLORREF color = position & (1 << 10) ? 0X0000AA : 0XAA00AA;
	setlinecolor(color);
	setfillcolor(color);

	line(0, HEIGHT - 60, static_cast<int>(WIDTH * ((t0 + 0.0) / t1)), HEIGHT - 60);
	line(0, HEIGHT - 59, static_cast<int>(WIDTH * ((t0 + 0.0) / t1)), HEIGHT - 59);
	solidcircle(static_cast<int>(WIDTH * ((t0 + 0.0) / t1)), HEIGHT - 60, 4);			// 画进度条小球
}

// 画音乐名事件
void GuiMusicPlayer::drawMusicNameEvent()
{
	settextcolor(0XAA00AA);																// 字体颜色
	settextstyle(14, 0, L"宋体");														// 字体大小，样式

	if (musicData.nowMusicName.size() < 26)
	{
		outtextxy(30, HEIGHT - 45, musicData.nowMusicName.c_str());						// 显示音乐名
	}
	else
	{
		outtextxy(30, HEIGHT - 45, (musicData.nowMusicName.substr(0, 26) + L"...").c_str());
	}
}

// 画音乐时长事件
void GuiMusicPlayer::drawMusicTimeEvent()
{
	settextcolor(0XAA00AA);													// 字体颜色
	settextstyle(14, 0, L"宋体");											// 字体大小，样式
	if (musicData.status == 0)												// 若未播放，直接返回
	{
		return;
	}

	const int t0 = musicData.getMusicCurrentTime();
	const int t1 = musicData.getMusicTotalTime();
	{
		wstring s0;
		if (t0 / 60 < 10)													// 当前时长，分
		{
			s0 = std::to_wstring(0) + std::to_wstring(t0 / 60);
		}
		else
		{
			s0 = std::to_wstring(t0 / 60);
		}

		if (t0 % 60 < 10)													// 秒
		{
			s0 += L":0" + std::to_wstring(t0 % 60) + L"/";
		}
		else
		{
			s0 += L":" + std::to_wstring(t0 % 60) + L"/";
		}

		if (t1 / 60 < 10)													// 总时长，分
		{
			s0 += std::to_wstring(0) + std::to_wstring(t1 / 60);
		}
		else
		{
			s0 += std::to_wstring(t1 / 60);
		}

		if (t1 % 60 < 10)													// 总时长，秒
		{
			s0 += L":0" + std::to_wstring(t1 % 60);
		}
		else
		{
			s0 += L":" + std::to_wstring(t1 % 60);
		}

		settextstyle(14, 0, L"宋体");
		outtextxy(30, HEIGHT - 30, s0.c_str());								// 显示
	}
}

// 画播放暂停按钮事件
void GuiMusicPlayer::drawPlayPauseButtonEvent()
{
	COLORREF color = position & (1 << 12) ? 0X0000AA : 0XAA00AA;

	setfillcolor(color);
	solidcircle(WIDTH / 2, HEIGHT - 30, 20);

	setfillcolor(WHITE);
	if (musicData.status == 1)																// 画播放
	{
		solidrectangle(WIDTH / 2 - 7, HEIGHT - 38, WIDTH / 2 - 3, HEIGHT - 22);
		solidrectangle(WIDTH / 2 + 7, HEIGHT - 38, WIDTH / 2 + 3, HEIGHT - 22);

	}
	else																					// 画暂停
	{
		const array<array<POINT, 2>, 3> a0{ WIDTH / 2 - 7,HEIGHT - 40,
			WIDTH / 2 - 7,HEIGHT - 20,WIDTH / 2 + 10,HEIGHT - 30 };
		solidpolygon(&a0.at(0).at(0), 3);
	}
}

// 画音量条事件
void GuiMusicPlayer::drawVolumeBarEvent()
{
	COLORREF color = position & (1 << 14) ? 0X0000AA : 0XAA00AA;

	setfillcolor(color);
	solidrectangle(WIDTH / 2 + 140, HEIGHT - 30,
		WIDTH / 2 + 140 + static_cast<int>(musicData.volume / 10), HEIGHT - 31);			// 画音量条
	solidrectangle(WIDTH / 2 + 140 + static_cast<int>(musicData.volume / 10), HEIGHT - 35,
		WIDTH / 2 + 140 + static_cast<int>(musicData.volume / 10 + 3), HEIGHT - 25);		// 画音量竖小短线
}

// 画播放模式事件
void GuiMusicPlayer::drawPlayModeEvent()
{
	COLORREF color = position & (1 << 15) ? 0X0000AA : 0XAA00AA;

	settextcolor(color);
	settextstyle(14, 0, L"宋体");												// 字体大小，样式
	wstring playMode;
	if (musicData.mode == 0)													// 显示播放模式
	{
		playMode = L"单曲循环";
	}
	else if (musicData.mode == 1)
	{
		playMode = L"顺序播放";
	}
	else if (musicData.mode == 2)
	{
		playMode = L"随机播放";
	}

	outtextxy(WIDTH / 2 + 300, HEIGHT - 36, playMode.c_str());
}


/****************************************************************
 * GuiMusicPlayer public method									*
 ****************************************************************/

// 析构函数
GuiMusicPlayer::~GuiMusicPlayer()
{
	closegraph();
}

// 单例模式，返回该类唯一对象的引用
GuiMusicPlayer& GuiMusicPlayer::singleton()
{
	static GuiMusicPlayer gmp;
	return gmp;
}

// 外部调用的接口
void GuiMusicPlayer::run()
{
	event();
}