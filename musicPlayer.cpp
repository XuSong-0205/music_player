#include"musicPlayer.h"


//  MusiMusicMCI method
MusicMCI::MusicMCI(void)noexcept
{
	nDeviceID = -1;
}

MusicMCI::~MusicMCI(void)noexcept
{
}

// 打开文件
// 成功返回非零值，失败返回0
BOOL MusicMCI::open(LPCWSTR strSongPath)noexcept
{
	MCI_OPEN_PARMS mciOP;

	mciOP.lpstrDeviceType = nullptr;
	mciOP.lpstrElementName = strSongPath;
	const DWORD dwReturn = mciSendCommand(0, MCI_OPEN,
		MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE, (DWORD)static_cast<LPVOID>(&mciOP));
	if (dwReturn == 0)
	{
		nDeviceID = mciOP.wDeviceID;
		return TRUE;
	}
	else
	{
		nDeviceID = -1;
		return FALSE;
	}
}

// 播放
// 成功返回非零值，失败返回0
BOOL MusicMCI::play()noexcept
{
	MCI_PLAY_PARMS mciPP{};

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_PLAY,
		MCI_NOTIFY, (DWORD)static_cast<LPVOID>(&mciPP));
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// 暂停播放
// 成功返回非零值，失败返回0
BOOL MusicMCI::pause()noexcept
{
	MCI_GENERIC_PARMS mciGP{};

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_PAUSE,
		MCI_NOTIFY | MCI_WAIT, (DWORD)static_cast<LPVOID>(&mciGP));
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// 停止播放并使进度返回到开头
// 成功返回非零值，失败返回0
BOOL MusicMCI::stop()noexcept
{
	MCI_SEEK_PARMS mciSP{};

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_SEEK,
		MCI_WAIT | MCI_NOTIFY | MCI_SEEK_TO_START, (DWORD)static_cast<LPVOID>(&mciSP));
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// 关闭MCI设备
// 成功返回非零值，失败返回0
BOOL MusicMCI::close()noexcept
{
	MCI_GENERIC_PARMS mciGP{};

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_CLOSE,
		MCI_NOTIFY | MCI_WAIT, (DWORD)static_cast<LPVOID>(&mciGP));
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// 获得当前播放进度，pos以ms为单位
// 成功返回非零值，失败返回0
BOOL MusicMCI::getPlayBackTime(DWORD& pos)noexcept
{
	MCI_STATUS_PARMS mciSP;

	mciSP.dwItem = MCI_STATUS_POSITION;
	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_STATUS,
		MCI_STATUS_ITEM, (DWORD)static_cast<LPVOID>(&mciSP));
	if (dwReturn == 0)
	{
		pos = mciSP.dwReturn;
		return TRUE;
	}
	else
	{
		pos = 0;
		return FALSE;
	}
}

// 音量设定，音量值范围在0到1000
// 成功返回非零值，失败返回0
BOOL MusicMCI::setVolume(int nVolumeValue)noexcept
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
		return TRUE;
	else
		return FALSE;
}



// MusicPlayer
MusicPlayer::MusicPlayer()
{
	file.open("music.mn", ios_base::in);
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
		this->findMusicName(filePath);
		this->wFile();
	}

	this->rFile();
	file.close();
	nowMusicName = "";

	if (!musicPathName.empty())
		musicPathName.pop_back();																		// 删除最后一个空白行

	for (const auto& x : musicPathName)
		musicName.push_back(x.substr(x.rfind("\\") + 1,
			x.size() - x.rfind("\\") - 1 - musicFormat.size() - 1));									// 截取出音乐名
}


// 获取特定格式的文件名    
void MusicPlayer::findMusicName(const string& path)
{
	long  hFile = 0;																					// 文件句柄  64位下long 改为 intptr_t
	struct _finddata_t fileinfo;																		//	文件信息 
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)					// 查找所有文件
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))															// 判断是否为文件夹
			{
				if (strcmp(&fileinfo.name[0], ".") != 0 && strcmp(&fileinfo.name[0], "..") != 0)		// 文件夹名中不含"."和".."
				{
					this->findMusicName(p.assign(path).append("\\").append(&fileinfo.name[0]));			// 递归遍历文件夹
				}
			}
			else
			{
				string suffix(&fileinfo.name[0]);
				if (suffix.size() > musicFormat.size())
					suffix = suffix.substr(suffix.size() - musicFormat.size());
				if (suffix == musicFormat)
					musicPathName.push_back(p.assign(path).append("\\").append(&fileinfo.name[0]));				// 如果不是文件夹，储存文件名
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

wstring MusicPlayer::stringTowstring(const string& str)
{
	wstring result;
	// 获取缓冲区大小，并申请空间，缓冲区大小按字符计算
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), nullptr, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	// 多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';				// 添加字符串结尾
	// 删除缓冲区并返回值
	result.append(buffer);
	delete[] buffer;
	return result;
}

void MusicPlayer::playMusic(int num)
{
	if (num > musicPathName.size())
	{
		cout << "参数不符合要求，请重试！" << endl;
		return;
	}

	wstring musci_name = stringTowstring(musicPathName.at(num));
	if (musicMci.open(musci_name.c_str()))
	{
		nowMusicName.assign(musicName.at(num));				// 设置正在播放的音乐
		cout << "音乐打开成功！" << endl;
		if (musicMci.play())
			cout << "音乐" << nowMusicName << "播放成功！" << endl;
		else
			cout << "音乐" << nowMusicName << "播放失败！" << endl;
	}
	else
		cout << "音乐打开失败，请稍后重试！" << endl;

	cout << musicPathName.at(num) << endl;
}

void MusicPlayer::pauseMusic()
{
	if (musicMci.pause())
		cout << "音乐已暂停！" << endl;
	else
		cout << "音乐暂停失败！" << endl;
}

void MusicPlayer::stopMusic()
{
	if (musicMci.stop())
		cout << "音乐已停止！" << endl;
	else
		cout << "音乐停止失败！" << endl;
}

void MusicPlayer::closeMusic()
{
	if (musicMci.close())
		cout << "音乐已关闭！" << endl;
	else
		cout << "音乐关闭失败！" << endl;
}

void MusicPlayer::setVolumeMusic(int volume)
{
	if (musicMci.setVolume(volume))
		cout << "音量设置成功！" << endl;
	else
		cout << "音量设置失败！" << endl;
}

int MusicPlayer::getPlayerBackTimeMusic()
{
	DWORD playTime = 0;
	if (musicMci.getPlayBackTime(playTime))
		cout << "获取播放时长成功！" << endl;
	else
		cout << "获取播放时长失败！" << endl;

	return playTime / 1000;
}

void MusicPlayer::wFile()
{
	if (!musicPathName.empty())
		for (auto& x : musicPathName)
			file << x << endl;
	cout << "文件music.mn写入完毕！" << endl;
}

void MusicPlayer::rFile()
{
	string s;
	do
	{
		getline(file, s);
		musicPathName.push_back(s);
	} while (!file.eof());
	cout << "文件musci.mn读取完毕！" << endl;
}

void MusicPlayer::showMusicName()
{
	if (musicName.empty())
	{
		cout << "无播放列表！" << endl;
		return;
	}

	int i = 0;
	for (auto& x : musicName)
		cout << i++ << "  " << x << endl;
}

void MusicPlayer::chooseMusicPlay()
{
	int choose = -1;
	this->showMusicName();
	cout << "请选择你想播放的歌曲：";
	cin >> choose;

	this->stopMusic();
	this->playMusic(choose);
}

void MusicPlayer::chooseFunction()
{
	int n = -1;
	do
	{
		this->showPlayTime();

		cout << "0.退出播放" << endl;
		cout << "1.暂停播放" << endl;
		cout << "2.设置音量" << endl;
		cout << "3.选择其它音乐播放" << endl;
		cout << "请输入你想选择的功能：";
		Sleep(500);
		system("cls");
	} while (!_kbhit());
	n = _getch() - '0';
	switch (n)
	{
	case 0:cout << "已退出播放！" << endl; Sleep(1000); exit(0);
	case 1:this->pauseMusic(); break;
	case 2: {int vol = 300; cout << "请输入音量值（0-1000）：";
		cin >> vol; this->setVolumeMusic(vol); break; }
	case 3:this->chooseMusicPlay(); break;
	default:cout << "选择错误！" << endl; break;
	}
}

void MusicPlayer::showPlayTime()
{
	//static time_t start = time(nullptr);
	const int t = this->getPlayerBackTimeMusic();
	cout << "正在播放：" << nowMusicName << endl;
	cout << "已播放：" << t / 60 << "分" << t % 60 << "秒" << endl;
	cout << "按任意键进行功能选择！" << endl;
}

MusicPlayer::~MusicPlayer()
{
	this->closeMusic();
}