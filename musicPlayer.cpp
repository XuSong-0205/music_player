#include"musicPlayer.h"

/**************************************************************************************************************
*  class MusicMCI                                                                                             *
**************************************************************************************************************/

/**************************************************************
* MusicMci public method                                      *
**************************************************************/

MusicMCI::MusicMCI()noexcept
{
	nDeviceID = -1;
}

MusicMCI::~MusicMCI()
{
	if (nDeviceID == -1) this->close();
}

// 打开文件
// 成功返回 true，失败返回 false
bool MusicMCI::open(LPCWSTR strSongPath)noexcept
{
	MCI_OPEN_PARMS mciOP;

	mciOP.lpstrDeviceType = nullptr;
	mciOP.lpstrElementName = strSongPath;
	const DWORD dwReturn = mciSendCommand(0, MCI_OPEN,
		MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE, (DWORD)static_cast<LPVOID>(&mciOP));
	if (dwReturn == 0)
	{
		nDeviceID = mciOP.wDeviceID;
		return true;
	}
	else
	{
		nDeviceID = -1;
		return false;
	}
}

// 播放
// 成功返回非零值，失败返回0
bool MusicMCI::play()noexcept
{
	MCI_PLAY_PARMS mciPP{};

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_PLAY,
		MCI_NOTIFY, (DWORD)static_cast<LPVOID>(&mciPP));
	if (dwReturn == 0)
		return true;
	else
		return false;
}

// 暂停播放
// 成功返回 true，失败返回 false
bool MusicMCI::pause()noexcept
{
	MCI_GENERIC_PARMS mciGP{};

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_PAUSE,
		MCI_NOTIFY | MCI_WAIT, (DWORD)static_cast<LPVOID>(&mciGP));
	if (dwReturn == 0)
		return true;
	else
		return false;
}

// 停止播放并使进度返回到开头
// 成功返回 true，失败返回 false
bool MusicMCI::stop()noexcept
{
	MCI_SEEK_PARMS mciSP{};

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_SEEK,
		MCI_WAIT | MCI_NOTIFY | MCI_SEEK_TO_START, (DWORD)static_cast<LPVOID>(&mciSP));
	if (dwReturn == 0)
		return true;
	else
		return false;
}

// 关闭MCI设备
// 成功返回 true，失败返回 false
bool MusicMCI::close()noexcept
{
	MCI_GENERIC_PARMS mciGP{};

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_CLOSE,
		MCI_NOTIFY | MCI_WAIT, (DWORD)static_cast<LPVOID>(&mciGP));
	if (dwReturn == 0)
	{
		nDeviceID = -1;
		return true;
	}
	else
		return false;
}

// 获得当前播放进度，pos以ms为单位
// 成功返回 true，失败返回 false
bool MusicMCI::getCurrentTime(DWORD& pos)noexcept
{
	MCI_STATUS_PARMS mciSP{};

	mciSP.dwItem = MCI_STATUS_POSITION;
	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_STATUS,
		MCI_STATUS_ITEM, (DWORD)static_cast<LPVOID>(&mciSP));
	if (dwReturn == 0)
	{
		pos = mciSP.dwReturn;
		return true;
	}
	else
	{
		pos = 0;
		return false;
	}
}

// 获取音乐总时长，time以ms为单位
// 成功返回 true，失败返回 false
bool MusicMCI::getTotalTime(DWORD& time)noexcept
{
	MCI_STATUS_PARMS mciSP{};

	mciSP.dwItem = MCI_STATUS_LENGTH;
	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_STATUS,
		MCI_WAIT | MCI_STATUS_ITEM, (DWORD)static_cast<LPVOID>(&mciSP));		// 关键,取得长度
	if (dwReturn == 0)
	{
		time = mciSP.dwReturn;
		return true;
	}
	else
	{
		time = 0;
		return false;
	}
}

// 设置音量大小，音量值范围在0到1000
// 成功返回 true，失败返回 false
bool MusicMCI::setVolume(size_t nVolumeValue)noexcept
{
	if (nVolumeValue > 1000)
	{
		nVolumeValue = 1000;
	}
	else if (nVolumeValue < 0)
	{
		nVolumeValue = 0;
	}

	MCI_DGV_SETAUDIO_PARMS mciDSP;
	mciDSP.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciDSP.dwValue = nVolumeValue;
	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_SETAUDIO,
		MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)static_cast<LPVOID>(&mciDSP));
	if (dwReturn == 0)
		return true;
	else
		return false;
}

// 设置播放起始位置
// 成功返回 true，失败返回 false
bool MusicMCI::setStartTime(size_t start_time) noexcept
{
	DWORD end_time = 0;
	this->getTotalTime(end_time);

	if (start_time > end_time)
		return false;

	MCI_PLAY_PARMS mciPlay{};
	mciPlay.dwFrom = start_time;
	mciPlay.dwTo   = end_time;
	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_PLAY,
		MCI_TO | MCI_FROM, (DWORD)static_cast<LPVOID>(&mciPlay));

	if (dwReturn == 0)
		return true;
	else
		return false;
}



/**************************************************************************************************************
*  class MusicData                                                                                            *
**************************************************************************************************************/

/**************************************************************
* MusicData private method                                    *
**************************************************************/

// 打开并播放音乐
void MusicData::openMusic(size_t num)
{
	if (musicPathName.empty() || num > musicPathName.size() - 1)
	{
#ifdef DEBUG
		cout << "参数不符合要求，请重试！" << endl;
#endif // DEBUG

		status = 0;
		return;
	}

	wstring musci_name = stringTowstring(musicPathName.at(num));
	nowMusicName = musicName.at(num);								// 设置正在操作的音乐名
	number = num;													// 设置正在操作的音乐名编号
	if (musicMci.open(musci_name.c_str()))
	{
#ifdef DEBUG
		wcout.imbue(std::locale("", LC_CTYPE));
		cout << "音乐 ";
		wcout << nowMusicName;
		cout << " 打开成功！" << endl;
#endif // DEBUG

		playMusic();
		musicMci.setVolume(volume);
	}
	else
	{
#ifdef DEBUG
		wcout.imbue(std::locale("", LC_CTYPE));
		cout << "音乐 ";
		wcout << nowMusicName;
		cout << " 打开失败，请稍后重试！" << endl;
#endif // DEBUG
	}
}

// 播放音乐
void MusicData::playMusic()
{
#ifdef DEBUG
	wcout.imbue(std::locale("", LC_CTYPE));
#endif // DEBUG

	if (musicMci.play())
	{
#ifdef DEBUG
		cout << "音乐 ";
		wcout << nowMusicName;
		cout << " 播放成功！" << endl;
#endif // DEBUG

		status = 1;
	}
	else
	{
#ifdef DEBUG
		cout << "音乐 ";
		wcout << nowMusicName;
		cout << " 播放失败！" << endl;
#endif // DEBUG

		status = 0;
	}
}

// 暂停播放
void MusicData::pauseMusic()
{
#ifdef DEBUG
	wcout.imbue(std::locale("", LC_CTYPE));
#endif // DEBUG

	if (status)
	{
		if (musicMci.pause())
		{
#ifdef DEBUG
			cout << "音乐 ";
			wcout << nowMusicName;
			cout << " 已暂停！" << endl;
#endif // DEBUG

			status = 2;
		}
		else
		{
#ifdef DEBUG
			cout << "音乐 ";
			wcout << nowMusicName;
			cout << " 暂停失败！" << endl;
#endif // DEBUG
		}
	}
}

// 停止播放
void MusicData::stopMusic()
{
#ifdef DEBUG
	wcout.imbue(std::locale("", LC_CTYPE));
#endif // DEBUG

	if (musicMci.stop())
	{
#ifdef DEBUG
		cout << "音乐 ";
		wcout << nowMusicName;
		cout << " 已停止！" << endl;
#endif // DEBUG

		status = 0;
	}
	else
	{
#ifdef DEBUG
		cout << "音乐 ";
		wcout << nowMusicName;
		cout << " 停止失败！" << endl;
#endif // DEBUG
	}
}

// 关闭音乐
void MusicData::closeMusic()
{
#ifdef DEBUG
	wcout.imbue(std::locale("", LC_CTYPE));
#endif // DEBUG

	if (musicMci.close())
	{
#ifdef DEBUG
		cout << "音乐 ";
		wcout << nowMusicName;
		cout << " 已关闭！" << endl;
#endif // DEBUG

		status = 0;
	}
	else
	{
#ifdef DEBUG
		cout << "音乐 ";
		wcout << nowMusicName;
		cout << " 关闭失败！" << endl;
#endif // DEBUG
	}
}

// 设置音乐音量
void MusicData::setMusicVolume(size_t vol)
{
	if (status)
	{
		if (musicMci.setVolume(vol))
			volume = vol;
#ifdef DEBUG
		else
			cout << "音量设置失败！" << endl;
#endif // DEBUG
	}
	else
	{
		volume = vol;
	}
}

// 设置播放位置
bool MusicData::setMusicStartTime(size_t start_time)
{
	if (status)
	{
		if (musicMci.setStartTime(start_time))
		{
			return true;
		}
		else
		{
#ifdef DEBUG
			cout << "设置播放位置失败！" << endl;
#endif // DEBUG
			return false;
		}
	}

	return false;
}

// 获取音乐当前播放时间
int MusicData::getMusicCurrentTime()
{
	if (status)
	{
		DWORD playTime = 0;
		if (!musicMci.getCurrentTime(playTime))
		{
#ifdef DEBUG
			cout << "获取播放时长失败！" << endl;
#endif // DEBUG

			return 0;
		}
		return playTime / 1000;
	}

	return 0;
}

// 获取音乐总播放时间
int MusicData::getMusicTotalTime()
{
	if (status)
	{
		DWORD totalTime = 0;
		if (!musicMci.getTotalTime(totalTime))
		{
#ifdef DEBUG
			cout << "获取总时长失败！" << endl;
#endif // DEBUG

			return 0;
		}
		return totalTime / 1000;
	}

	return 0;
}


// 从文件filePath.ad中读取搜索路径
void MusicData::getFilePath()
{
	fstream fPath("filePath.ad", ios_base::in);
	if (!fPath.is_open())
	{
		cerr << "文件filePath.ad打开失败，此文件可能不存在！" << endl;
		cerr << "程序将尝试新建此文件!" << endl;

		fPath.open("filePath.ad", ios_base::in | ios_base::out | ios_base::trunc);
		if (!fPath.is_open())
		{
			cerr << "文件filePath.ad创建失败！，程序可能遇到了一些问题！" << endl;
			cerr << "请稍后重试！" << endl;
		}
		else
		{
			cout << "文件filePath.ad创建成功！" << endl;
		}
	}
	else
	{
		rFilePath(fPath);
	}
}

// 获取特定格式的文件名    
void MusicData::findMusicName(const string& path)
{
	_finddata_t fileinfo;															// 文件信息
	auto hFile = _findfirst((path + "\\*." + musicFormat).c_str(), &fileinfo);		// 查找当前文件夹是否存在指定格式文件
	if (hFile != -1)																// 如果该文件夹下存在指定格式文件
	{
		do
		{
			string str = fileinfo.name;
			musicPathName.push_back(path + "\\" + str);								// 写入音乐全路径名
			musicName.push_back(stringTowstring(str.substr(0, str.size() - musicFormat.size() - 1)));	// 写入音乐名
		} while (_findnext(hFile, &fileinfo) == 0);
	}

	hFile = _findfirst((path + "\\*").c_str(), &fileinfo);							// 查找子文件夹，查找所有文件
	if (hFile != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))										// 判断是否为文件夹
			{
				if (fileinfo.name[0] != '.')										// 文件夹名中不含"."和".."
				{
					findMusicName(path + "\\" + fileinfo.name);						// 递归遍历文件夹
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
}

// 将filePath写入文件filePath.ad
void MusicData::wFilePath(fstream& file)
{
	file << filePath;
}

// 将文件filePath.ad读取到filePath
void MusicData::rFilePath(fstream& file)
{
	string temp;
	getline(file, temp);
	if (temp.empty() || temp.at(0) == '\n')
	{
		cout << "文件内容为空！" << endl;
		cout << "文件路径设置为默认！" << endl;
	}
	else
		filePath = temp;
}

// 将musicPathName写入文件music.mn中
void MusicData::wFileMusic(fstream& file)
{
	if (!musicPathName.empty())
	{
		const int n = musicPathName.size();
		file << musicPathName.at(0);
		for (int i = 1; i < n; ++i)
			file << "\n" << musicPathName.at(i);
		cout << "文件music.mn写入完毕！" << endl;
	}
	else
		cout << "musicPathName为空！" << endl;
}

// 从music.mn读取到musicPathName和musicName中
void MusicData::rFileMusic(fstream& file)
{
	string s;
	if (file.eof())
		cout << "文件music.mn为空！" << endl;

	while (!file.eof())
	{
		getline(file, s);
		musicPathName.push_back(s);												// 写入音乐全路径名
		const auto pos = s.rfind("\\");
		s = s.substr(pos + 1, s.size() - pos - 1 - musicFormat.size() - 1);		// 截取出音乐名
		musicName.push_back(stringTowstring(s));								// 写入音乐名
	}
	cout << "文件musci.mn读取完毕！" << endl;
}

// 删除歌曲
void MusicData::deleteMusicData(vector<int>& vec)		
{
	sort(vec.begin(), vec.end());										// 对其进行排序，方便下面修正
	for (decltype(vec.size()) i = 0; i < vec.size(); ++i)
	{
		vec.at(i) -= i;													// 修正要删除的下标信息

		musicName.erase(musicName.begin() + vec.at(i));					// 删除对应下标的数据
		musicPathName.erase(musicPathName.begin() + vec.at(i));
	}
}

/**************************************************************
*  MusicData public method                                    *
**************************************************************/

// 默认构造函数
MusicData::MusicData()
{
	srand(time(nullptr) & 0xffffffff);
	getFilePath();																// 初始化搜索的文件路径

	cout << "文件搜索路径为：" << filePath << endl;
	fstream file("music.mn", ios_base::in);
	if (!file.is_open())
	{
		cerr << "文件music.mn打开失败，此文件可能不存在" << endl;
		cerr << "程序将尝试新建此文件，并初始化文件内容" << endl;

		file.open("music.mn", ios_base::out);
		if (!file.is_open())
		{
			cerr << "文件music.mn创建失败！" << endl;
			cerr << "程序可能遇到了一些问题！" << endl;
			cerr << "程序即将退出！" << endl;
			system("pause");
			exit(0);
		}

		cout << "文件music.mn创建成功！" << endl;
		findMusicName(filePath);
		wFileMusic(file);
	}
	else
	{
		rFileMusic(file);						// 若文件存在，直接读取其内容
	}

	file.close();
	nowMusicName = L"";

	if (!musicPathName.empty() && musicPathName.back() == "")
		musicPathName.pop_back();											// 删除最后一个空白行

	if (!musicName.empty() && musicName.back() == L"")
		musicName.pop_back();
}

// 析构函数
MusicData::~MusicData()
{
	closeMusic();
}


// 非成员函数
// string转wstring
wstring stringTowstring(const string& str)
{
	wstring result;
	// 获取缓冲区大小，并申请空间，缓冲区大小按字符计算
	const size_t len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), nullptr, 0);
	vector<TCHAR> buffer(len + 1u);
	// 多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), &buffer.at(0), len);
	buffer.at(len) = '\0';			// 添加字符串结尾
	result.append(&buffer.at(0));
	return result;
}


/**********************************************************************************************************************
*  class GuiMusicPlayer                                                                                               *
**********************************************************************************************************************/

/**************************************************************
* GuiMusicPlayer private method                               *
**************************************************************/

// 查找背景图片是否存在
// 查找到返回 true，否则返回 false
bool GuiMusicPlayer::findBgPicture()noexcept
{
	long hFile = 0;
	struct _finddata_t fileinfo;																		// 文件信息 
	if ((hFile = _findfirst("background.jpg", &fileinfo)) != -1)										// 查找所有文件
		return true;

	return false;
}

// 画程序的整体 ui 界面(静态)
void GuiMusicPlayer::ui()
{
	constexpr COLORREF c0 = 0XAA00AA;
	settextcolor(0X0000AA);																				// 字体颜色
	settextstyle(15, 0, L"宋体");																		// 字体样式
	setbkmode(TRANSPARENT);																				// 文字输出背景透明
	outtextxy(WIDTH - 40, 10, L"退出");
	setlinecolor(0XE8E8E8);
	line(0, 40, WIDTH, 40);																				// 上边界线
	line(0, HEIGHT - 60, WIDTH, HEIGHT - 60);															// 画进度条
	line(0, HEIGHT - 59, WIDTH, HEIGHT - 59);

	outtextxy(50, 60, L"我的音乐");
	outtextxy(50, 180, L"播放列表");						

	/*********************************************************************************************************************
	* 画播放框ui                                                                                                         *
	*********************************************************************************************************************/
	setlinecolor(c0);																					// 设置画线颜色
	setfillcolor(c0);																					// 设置填充颜色

	line(WIDTH / 2 - 64, HEIGHT - 37, WIDTH / 2 - 64, HEIGHT - 23);										// |
	line(WIDTH / 2 - 65, HEIGHT - 37, WIDTH / 2 - 65, HEIGHT - 23);

	const array<array<POINT, 2>, 3> a0{ WIDTH / 2 - 48,HEIGHT - 38,WIDTH / 2 - 48,HEIGHT - 22,WIDTH / 2 - 60,HEIGHT - 30 };
	solidpolygon(&a0.at(0).at(0), 3);																	// <|

	setfillcolor(c0);
	solidcircle(WIDTH / 2, HEIGHT - 30, 20);															// ⚪

	const array<array<POINT, 2>, 3> a1{ WIDTH / 2 + 48,HEIGHT - 38,WIDTH / 2 + 48,HEIGHT - 22,WIDTH / 2 + 60,HEIGHT - 30 };
	solidpolygon(&a1.at(0).at(0), 3);																	// |>

	line(WIDTH / 2 + 64, HEIGHT - 37, WIDTH / 2 + 64, HEIGHT - 23);										// |
	line(WIDTH / 2 + 65, HEIGHT - 37, WIDTH / 2 + 65, HEIGHT - 23);

	setlinecolor(0XAA00AA);
	const array<POINT, 14> a2{ WIDTH / 2 + 120,HEIGHT - 34,WIDTH / 2 + 125,HEIGHT - 34,WIDTH / 2 + 129,HEIGHT - 38,
		WIDTH / 2 + 129,HEIGHT - 22,WIDTH / 2 + 125,HEIGHT - 26,WIDTH / 2 + 120,HEIGHT - 26,WIDTH / 2 + 120,HEIGHT - 34 };
	polyline(&a2.at(0), 7);																				// 画小喇叭

	arc(WIDTH / 2 + 125, HEIGHT - 35, WIDTH / 2 + 135, HEIGHT - 25, -3.1415926 / 3, 3.1415926 / 3);		// 画圆弧

	setlinecolor(0XFFFFFF);
	line(WIDTH / 2 + 140, HEIGHT - 30, WIDTH / 2 + 240, HEIGHT - 30);									// 画音量条

	drawStartPause();
}

// 画 开始/暂停 按钮(动态)
void GuiMusicPlayer::drawStartPause()
{
	setfillcolor(0XAA00AA);
	solidcircle(WIDTH / 2, HEIGHT - 30, 20);

	setfillcolor(WHITE);
	if (musicData.status == 1)																			// 画播放
	{
		solidrectangle(WIDTH / 2 - 7, HEIGHT - 38, WIDTH / 2 - 3, HEIGHT - 22);							
		solidrectangle(WIDTH / 2 + 7, HEIGHT - 38, WIDTH / 2 + 3, HEIGHT - 22);

	}
	else																								// 画暂停
	{
		const array<array<POINT, 2>, 3> a0{ WIDTH / 2 - 7,HEIGHT - 40,WIDTH / 2 - 7,HEIGHT - 20,WIDTH / 2 + 10,HEIGHT - 30 };
		solidpolygon(&a0.at(0).at(0), 3);																
	}
}

// 画显示各种信息(动态)
void GuiMusicPlayer::drawPlayInformation()
{
	settextcolor(0XAA00AA);																			// 字体颜色
	settextstyle(14, 0, L"宋体");
	static int ti = 0;
	static IMAGE i0, i1, i2, i3, i4, i5, i6, i7;
	if(!ti)																							// 只读取一次该区域
	{
		getimage(&i0, 30, HEIGHT - 45, WIDTH / 2 - 100, 20);										// 歌曲名下的背景

		getimage(&i1, WIDTH / 2 + 110, HEIGHT - 40, 140, HEIGHT - 20);								// 音量条

		getimage(&i2, WIDTH / 2 + 300, HEIGHT - 40, 60, 20);										// 播放模式

		getimage(&i3, 30, HEIGHT - 30, 120, 20);													// 音乐时间

		getimage(&i4, 0, HEIGHT - 65, WIDTH, 10);													// 进度条

		getimage(&i5, 260, 40, 660, 540);															// 播放列表

		getimage(&i6, 130, 180, 50, 20);															// 播放列表字体旁的箭头

		getimage(&i7, WIDTH - 10, 40, WIDTH - 2, HEIGHT - 80);										// 滚动条
		++ti;
	}
		
	putimage(30, HEIGHT - 45, &i0);																	// 先显示背景
	putimage(WIDTH / 2 + 110, HEIGHT - 40, &i1);													// 音量条
	putimage(WIDTH / 2 + 300, HEIGHT - 40, &i2);													// 播放模式
	putimage(30, HEIGHT - 30, &i3);																	// 音乐时间
	putimage(0, HEIGHT - 65, &i4);																	// 进度条
	putimage(260, 40, &i5);																			// 播放列表
	putimage(130, 180, &i6);																		// 播放列表箭头
	putimage(WIDTH - 10, 40, &i7);																	// 滚动条

	if (bList)																						// 是否显示播放列表
	{
		if (!musicData.musicName.empty())															// 显示播放列表
		{
			for (int i = 0; i <= 12; ++i)
			{
				if (i + numRange.at(0) > numRange.at(1)) break;										// 超出显示范围，退出

				wstring s0;
				if (musicData.musicName.at(i + numRange.at(0)).size() > 60)							// 超出一定长度的名字只显示一部分
					s0 = musicData.musicName.at(i + numRange.at(0)).substr(0, 60) + L"...";
				else
					s0 = musicData.musicName.at(i + numRange.at(0));
				outtextxy(260, 40 + 20 + i * 40, s0.c_str());
			}

			const int length = (HEIGHT - 120) / 11;
			const double k = numRange.at(0) / (musicData.musicName.size() + 0.0);					// 比列位置
			const int y1 = static_cast<int>(40 + (HEIGHT - 120) * k) + 4;							// 滚动条的左上角的纵坐标
			setfillcolor(0XAA00AA);																	// 滚动条颜色，紫色
			solidcircle(WIDTH - 6, y1, 4);															// 画上半圆
			solidrectangle(WIDTH - 10, y1, WIDTH - 2, y1 + length);									// 画滚动条
			solidcircle(WIDTH - 6, y1 + length, 4);													// 画下半圆
		}
		else
			outtextxy(260, 40 + 15, L"列表为空！");
	}
	
	if (bList)																						// 画播放列表的箭头
	{
		setlinecolor(0X0000AA);	
		line(140, 185, 162, 185);
		line(159, 179, 165, 185);
		line(159, 191, 165, 185);
	}
	else
	{
		setlinecolor(0X0000AA);
		line(140, 185, 162, 185);
		line(137, 185, 143, 179);
		line(137, 185, 143, 191);
	}

	if (musicData.nowMusicName.size() < 26)
		outtextxy(30, HEIGHT - 45, musicData.nowMusicName.c_str());									// 再显示音乐名
	else
		outtextxy(30, HEIGHT - 45, (musicData.nowMusicName.substr(0, 26) + L"...").c_str());

	static int t0 = 0, t1 = 0;																		// 显示音乐时长
	{
		if (musicData.status)
		{
			t0 = musicData.getMusicCurrentTime();
			t1 = musicData.getMusicTotalTime();
		}
		wstring s0;
		if (t0 / 60 < 10)																			// 当前时长
			s0 = std::to_wstring(0) + std::to_wstring(t0 / 60);
		else
			s0 = std::to_wstring(t0 / 60);

		if (t0 % 60 < 10)
			s0 += L":0" + std::to_wstring(t0 % 60) + L"/";
		else
			s0 += L":" + std::to_wstring(t0 % 60) + L"/";

		if (t1 / 60 < 10)
			s0 += std::to_wstring(0) + std::to_wstring(t1 / 60);
		else
			s0 += std::to_wstring(t1 / 60);

		if (t1 % 60 < 10)
			s0 += L":0" + std::to_wstring(t1 % 60);
		else
			s0 += L":" + std::to_wstring(t1 % 60);

		settextstyle(14, 0, L"宋体");
		outtextxy(30, HEIGHT - 30, s0.c_str());
	}

	setlinecolor(0XE8E8E8);																			// 显示进度条信息
	line(0, HEIGHT - 60, WIDTH, HEIGHT - 60);
	line(0, HEIGHT - 59, WIDTH, HEIGHT - 59);
	setlinecolor(0XAA00AA);
	line(0, HEIGHT - 60, static_cast<int>(WIDTH * ((t0 + 0.0) / t1)), HEIGHT - 60);
	line(0, HEIGHT - 59, static_cast<int>(WIDTH * ((t0 + 0.0) / t1)), HEIGHT - 59);
	setfillcolor(0XAA00AA);
	solidcircle(static_cast<int>(WIDTH * ((t0 + 0.0) / t1)), HEIGHT - 60, 4);						// 画进度条小球


	if (musicData.mode == 0)																		// 显示播放模式
		outtextxy(WIDTH / 2 + 300, HEIGHT - 40, L"单曲循环");
	else if (musicData.mode == 1)
		outtextxy(WIDTH / 2 + 300, HEIGHT - 40, L"顺序播放");
	else if (musicData.mode == 2)
		outtextxy(WIDTH / 2 + 300, HEIGHT - 40, L"随机播放");


	setlinecolor(0XAA00AA);
	setfillcolor(0XAA00AA);
	line(WIDTH / 2 + 140, HEIGHT - 30, WIDTH / 2 + 140 + musicData.volume / 10, HEIGHT - 30);
	line(WIDTH / 2 + 140, HEIGHT - 31, WIDTH / 2 + 140 + musicData.volume / 10, HEIGHT - 31);
	solidrectangle(WIDTH / 2 + 140 + musicData.volume / 10, HEIGHT - 35,
		WIDTH / 2 + 140 + musicData.volume / 10 + 3, HEIGHT - 25);									// 画音量竖小短线

	drawStartPause();
}


/**************************************************************
* GuiMusicPlayer public method                                *
**************************************************************/

GuiMusicPlayer::GuiMusicPlayer()
{
	srand(time(nullptr) & 0xffffffff);
	initgraph(WIDTH, HEIGHT
#ifdef DEBUG
		, SHOWCONSOLE
#endif
	
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

	setbkcolor(WHITE);													// 设置填充色 白色
	loadimage(&img, L"background.jpg",WIDTH,HEIGHT);					// 加载背景图片
	putimage(0, 0, &img);												// 显示背景图片
	if (!findBgPicture())												// 查询是否存在背景图片
		cleardevice();													// 使用背景色清空画面
}

GuiMusicPlayer::~GuiMusicPlayer()
{
	closegraph();
}

// 各种功能的选择，实现
// 外部调用的接口
void GuiMusicPlayer::choose()
{
	ui();

	MOUSEMSG m0{};
	BeginBatchDraw();
	while(true)
	{ 
		if (MouseHit())																		// 检测是否有鼠标消息
		{
			m0 = GetMouseMsg();																// 获取鼠标消息
			FlushMouseMsgBuffer();

			if (m0.x >= WIDTH - 40 && m0.x <= WIDTH - 10 && m0.y >= 10 && m0.y <= 25)		// 按退出键退出
			{
				if (m0.mkLButton) break;
			}
			
			// 播放列表操作
			if (bList && m0.x > 260 && m0.x < WIDTH - 20 && m0.y > 40 && m0.y < HEIGHT - 80)// 鼠标在播放列表
			{
				if (m0.mkLButton)															// 左键按下
				{
					const size_t ty = (m0.y - 40) / 40 + numRange.at(0);					// 计算出是播放列表中哪一个编号的歌曲
					if (musicData.status == 0)												// 未播放，开始播放
					{
						musicData.openMusic(ty);											// 打开并播放
					}
					else if (musicData.status == 1)											// 正在播放
					{
						if (musicData.number == ty)											// 若是正在播放的音乐
						{
							musicData.pauseMusic();											// 暂停
						}
						else																// 若不是，关闭原来播放的音乐，打开新音乐
						{
							musicData.closeMusic();
							musicData.openMusic(ty);
						}

					}
					else if (musicData.status == 2)											// 暂停播放
					{
						musicData.playMusic();												// 继续
					}
				}

				if (bList && m0.wheel / 120)												// 鼠标滚动消息
				{																			// 更新播放列表显示内容
					const int tw = m0.wheel / 120;
					const int ts = musicData.musicName.size() - 1;

					if (ts > 13)
					{
						if (tw >= 0)
							numRange.at(0) = (numRange.at(0) - tw) < 0 ? 0 : (numRange.at(0) - tw);
						else
							numRange.at(0) = (numRange.at(0) - tw) > (ts - 12) ? (ts - 12) : (numRange.at(0) - tw);

						numRange.at(1) = numRange.at(0) + 12;
					}
				}

			}

			// 滚动条操作
			if (bList && m0.x >= WIDTH - 10 && m0.x <= WIDTH - 2 && m0.y > 40 && m0.y < HEIGHT - 80)
			{
				if (m0.mkLButton)
				{
					const int length = (HEIGHT - 120) / 11;
					const double k = numRange.at(0) / (musicData.musicName.size() + 0.0);			// 比列位置
					const int y1 = static_cast<int>(40 + (HEIGHT - 120) * k);						// 滚动条的左上角的纵坐标
					const int msize = musicData.musicName.size();									// 音乐列表的长度
					if (m0.y < y1 && numRange.at(0) > 0)											// 上移一个页面
					{
						numRange.at(0) = numRange.at(0) - 12 >= 0 ? numRange.at(0) - 12 : 0;
						numRange.at(1) = numRange.at(0) + 12 < msize ? numRange.at(0) + 12 : msize - 1;
					}
					else if (m0.y > y1 + length && numRange.at(1) < msize - 1)	// 下移一个页面
					{
						numRange.at(1) = numRange.at(1) + 12 < msize ? numRange.at(1) + 12 : msize - 1;
						numRange.at(0) = numRange.at(1) - 12 >= 0 ? numRange.at(1) - 12 : 0;
					}
				}
			}

			// 其它操作
			if (m0.mkLButton)
			{
				if (m0.y >= HEIGHT - 68 && m0.y <= HEIGHT - 52 &&
					m0.x >= 0 && m0.x <= WIDTH)												// 音乐进度条
				{
					const double k = m0.x / (WIDTH + 0.0);									// 计算出进度条比列
					const size_t start_time = static_cast<size_t>
											(k * musicData.getMusicTotalTime() * 1000);		// 计算处新的音乐播放时间
					if (musicData.setMusicStartTime(start_time))							// 设置播放位置
						musicData.status = 1;												// 设置当前播放状态为 正在播放
				}
				else if (pow(m0.x - WIDTH / 2, 2) + pow(m0.y - HEIGHT + 30, 2) <= 400)		// 是否在圆形播放，暂停按钮内
				{																			// 根据播放状态进行操作
					if (musicData.status == 0)												// 若未播放
					{
						musicData.openMusic(musicData.number);								// 打开并播放
					}
					else if (musicData.status == 1)											// 正在播放
					{
						musicData.pauseMusic();												// 暂停
					}
					else if (musicData.status == 2)											// 暂停播放
					{
						musicData.playMusic();												// 继续
					}
				
				}
				else if (m0.x >= WIDTH / 2 + 140 && m0.x <= WIDTH / 2 + 240 && 
					m0.y <= HEIGHT - 25 && m0.y >= HEIGHT - 35)								// 音量调节
				{
					musicData.setMusicVolume((m0.x - WIDTH / 2 - 140) * 10);				// 设置音量
				}
				else if (m0.x >= WIDTH / 2 - 65 && m0.x <= WIDTH / 2 - 48 &&
					m0.y >= HEIGHT - 38 && m0.y <= HEIGHT - 22)	// 上一曲
				{
					if (musicData.musicPathName.empty())		// 无歌曲，跳过
						continue;

					if (musicData.status)																		// 是否需要关闭音乐
						musicData.closeMusic();

					if (musicData.mode == 0 || musicData.mode == 1)												// 若不是随机播放，上一曲
					{
						musicData.openMusic(musicData.number == 0 ? musicData.musicName.size() - 1 : musicData.number - 1);
					}
					else																						// 是随机播放，随机一曲
					{
						musicData.openMusic(rand() % musicData.musicName.size());
					}
				}
				else if (m0.x >= WIDTH / 2 + 48 && m0.x <= WIDTH / 2 + 65 &&
					m0.y >= HEIGHT - 38 && m0.y <= HEIGHT - 22)	// 下一曲
				{
					if (musicData.musicPathName.empty())		// 无歌曲，跳过
						continue;

					if (musicData.status)
						musicData.closeMusic();

					if (musicData.mode == 0 || musicData.mode == 1)
					{
						musicData.openMusic(musicData.number + 1 > musicData.musicName.size() - 1 ? 0 : musicData.number + 1);
					}
					else
					{
						musicData.openMusic(rand() % musicData.musicName.size());
					}
				}
				else if (m0.x >= WIDTH / 2 + 300 && m0.x <= WIDTH / 2 + 360 &&
					m0.y <= HEIGHT - 25 && m0.y >= HEIGHT - 40)		// 播放模式
				{
					musicData.mode = ++musicData.mode % 3;			// 顺序变更播放模式
				}
				else if (m0.x >= 50 && m0.x <= 165 && m0.y >= 175 && m0.y <= 200)
				{
						bList = !bList;														// 列表是否展开
				}
			}
		}
		drawPlayInformation();

		// 得到音乐的播放进度信息
		static int t0 = 0, t1 = 0;
		if (musicData.status)
		{
			t0 = musicData.getMusicCurrentTime();
			t1 = musicData.getMusicTotalTime();
		}

		// 检查音乐是否播放结束，并作出下一步操作
		if (musicData.status && t0 == t1)
		{
			musicData.closeMusic();
			if (musicData.mode == 0)														// 单曲循环
			{
				musicData.openMusic(musicData.number);
			}
			else if(musicData.mode==1)														// 顺序播放
			{
				musicData.openMusic(musicData.number + 1 > musicData.musicName.size() - 1 ?
					0 : musicData.number + 1);
			}
			else if (musicData.mode == 2)													// 随机播放
			{
				musicData.openMusic(rand() % musicData.musicName.size());
			}


		}
		
		Sleep(20);
		FlushBatchDraw();
	}

	EndBatchDraw();
}