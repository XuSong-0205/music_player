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
	MCI_STATUS_PARMS mciSP{};

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

// 获取音乐总时长，time以ms为单位
// 成功返回TRUE，失败返回FALSE
BOOL MusicMCI::getMusicTime(DWORD& time)noexcept
{
	MCI_STATUS_PARMS mciSP{};

	mciSP.dwItem = MCI_STATUS_LENGTH;
	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_STATUS,
		MCI_WAIT | MCI_STATUS_ITEM, (DWORD)static_cast<LPVOID>(&mciSP));		// 关键,取得长度
	if (dwReturn == 0)
	{
		time = mciSP.dwReturn;
		return TRUE;
	}
	else
	{
		time = 0;
		return FALSE;
	}
}

// 音量设定，音量值范围在0到1000
// 成功返回非零值，失败返回0
BOOL MusicMCI::setVolume(size_t nVolumeValue)noexcept
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



/**************************************************************************************************************
*  class MusicData                                                                                            *
**************************************************************************************************************/

/**************************************************************
* MusicData private method                                    *
**************************************************************/

// 打开并播放音乐
void MusicData::openMusic(size_t num)
{
	if (num > musicPathName.size() - 1)
	{
		cout << "参数不符合要求，请重试！" << endl;
		status = 0;
		return;
	}

	wstring musci_name = stringTowstring(musicPathName.at(num));
	nowMusicName.assign(musicName.at(num));							// 设置正在操作的音乐名
	number = num;													// 设置正在操作的音乐名编号
	if (musicMci.open(musci_name.c_str()))
	{
		cout << "音乐打开成功！" << endl;

		playMusic();
		musicMci.setVolume(volume);
	}
	else
		cout << "音乐打开失败，请稍后重试！" << endl;
}

// 播放音乐
void MusicData::playMusic()
{
	if (musicMci.play())
	{
		status = 1;
	}
	else
	{
		cout << "音乐";
		wcout << nowMusicName;
		cout << "播放失败！" << endl;
		status = 0;
	}
}

// 暂停播放
void MusicData::pauseMusic()
{
	if (status)
	{
		if (musicMci.pause())
		{
			cout << "音乐已暂停！" << endl;
			status = 2;
		}
		else
			cout << "音乐暂停失败！" << endl;
	}
}

// 停止播放
void MusicData::stopMusic()
{
	if (musicMci.stop())
	{
		cout << "音乐已停止！" << endl;
		status = 0;
	}
	else
		cout << "音乐停止失败！" << endl;
}

// 关闭音乐
void MusicData::closeMusic()
{
	if (musicMci.close())
	{
		cout << "音乐已关闭！" << endl;
		status = 0;
	}
	else
		cout << "音乐关闭失败！" << endl;
}

// 获取音乐当前播放时间
int MusicData::getPlayerBackTimeMusic()
{
	if (status)
	{
		DWORD playTime = 0;
		if (!musicMci.getPlayBackTime(playTime))
		{
			cout << "获取播放时长失败！" << endl;
			return 0;
		}
		return playTime / 1000;
	}
}

// 获取音乐总播放时间
int MusicData::getTotalTime()
{
	if (status)
	{
		DWORD totalTime = 0;
		if (!musicMci.getMusicTime(totalTime))
		{
			cout << "获取总时长失败！" << endl;
			return 0;
		}
		return totalTime / 1000;
	}
}


// string转wstring
wstring MusicData::stringTowstring(const string& str)
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
		wFilePath(fPath);
	}
}

// 获取特定格式的文件名    
void MusicData::findMusicName(const string& path)
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
					findMusicName(p.assign(path).append("\\").append(&fileinfo.name[0]));				// 递归遍历文件夹
				}
			}
			else
			{
				string suffix(&fileinfo.name[0]);
				if (suffix.size() > musicFormat.size())
					suffix = suffix.substr(suffix.size() - musicFormat.size());
				if (suffix == musicFormat)
				{
					musicPathName.push_back(p.assign(path).append("\\").append(&fileinfo.name[0]));		// 是文件，储存文件名

					string temp(&fileinfo.name[0]);
					temp.assign(temp.substr(0, temp.size() - musicFormat.size() - 1));					// 截取出音乐名
					musicName.push_back(stringTowstring(temp));											// 写入
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

// 将filePath写入文件filePath.ad
void MusicData::wFilePath(fstream& file)
{
	file << filePath << endl;
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
		filePath = std::move(temp);
}


// 将musicPathName写入文件music.mn中
void MusicData::wFileMusic(fstream& file)
{
	if (!musicPathName.empty())
		for (auto& x : musicPathName)
			file << x << endl;
	cout << "文件music.mn写入完毕！" << endl;
}

// 从music.mn读取到musicPathName和musicName中
void MusicData::rFileMusic(fstream& file)
{
	string s;
	do
	{
		getline(file, s);
		musicPathName.push_back(s);
		s.assign(s.substr(s.rfind("\\") + 1, s.size() - s.rfind("\\") - 1 - musicFormat.size() - 1));	// 截取出音乐名

		musicName.push_back(stringTowstring(s));														// 写入

	} while (!file.eof());
	cout << "文件musci.mn读取完毕！" << endl;
}

// 设置音乐音量
void MusicData::setMusicVolume(size_t vol)
{
	if (status)
	{
		if (musicMci.setVolume(vol))
			volume = vol;
		else
			cout << "音量设置失败！" << endl;
	}
	else
	{
		volume = vol;
	}
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

	rFileMusic(file);
	file.close();
	nowMusicName = L"";

	if (!musicPathName.empty())
		musicPathName.pop_back();																		// 删除最后一个空白行

	if (!musicName.empty())
		musicName.pop_back();
}

// 析构函数
MusicData::~MusicData()
{
	closeMusic();
}



///**********************************************************************************************************************
//*  class CmdMusicPlayer                                                                                               *
//**********************************************************************************************************************/
//
///**************************************************************
//* CmdMusicPlayer private method                               *
//**************************************************************/
//
//// 设置光标位置
//void CmdMusicPlayer::pos(short x, short y)noexcept
//{
//	COORD pos{ x,y };
//	HANDLE hOutput = nullptr;
//	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleCursorPosition(hOutput, pos);
//}
//
//// 设置音乐音量
//void CmdMusicPlayer::setMusicVolume()
//{
//	size_t vol = 0;
//	cout << "当前音量：" << musicData.volume << endl;
//	cout << "请输入音量值（0-1000）：";
//	cin >> vol;
//	if (vol <= 1000)
//		musicData.setMusicVolume(vol);
//	else
//	{
//		cout << "输入错误！" << endl;
//		Sleep(1000);
//	}
//}
//
//// 设置搜索路径并写入filePath.ad中
//void CmdMusicPlayer::setFilePath()
//{
//	string temp;
//	cout << "当前搜索路径为：" << musicData.filePath << endl;
//	cout << "请输入新的搜索路径(请输入绝对路径！)：";
//	cin >> temp;
//	cout << "新的搜索路径为：" << temp << endl;
//	cout << "是否修改？（y/n）:";
//	char ch = 0;
//	cin >> ch;
//	if ('y' == ch || 'Y' == ch)
//	{
//		musicData.filePath = temp;
//		fstream fp("filePath.ad", ios_base::out);
//		if (!fp.is_open())
//		{
//			cerr << "文件filePath.ad打开失败，请稍后重试！" << endl;
//		}
//		else
//		{
//			musicData.wFilePath(fp);										
//			fp.close();
//			cout << "搜索路径写入完毕！" << endl;
//
//			musicData.findMusicName(musicData.filePath);					// 寻找音乐文件
//			fstream file("music.mn", ios_base::out);						// 截断打开，重新写入
//			if (!file.is_open())
//			{
//				cerr << "文件music.mn打开失败，请稍后重试此功能！" << endl;
//				Sleep(1000);
//			}
//			else
//			{
//				musicData.wFileMusic(file);									// 写入音乐全路径名
//				file.close();
//				cout << "写入新歌曲名成功！" << endl;
//			}
//			cout << "以搜索完毕新路径下的歌曲！" << endl;
//		}
//	}
//	else
//	{
//		cout << "已取消修改！" << endl;
//	}
//}
//
//// 设置播放模式
//void CmdMusicPlayer::setPlayMode()
//{
//	size_t cho = 0;
//	cout << "0.单曲循环" << endl;
//	cout << "1.顺序播放" << endl;
//	cout << "2.随机播放" << endl;
//	cout << "当前播放模式：";
//	if (musicData.mode == 0)
//		cout << "单曲循环" << endl;
//	else if (musicData.mode == 1)
//		cout << "顺序播放" << endl;
//	else if (musicData.mode == 2)
//		cout << "随机播放" << endl;
//
//	cout << "请选择你要选择的播放模式：";
//	cin >> cho;
//	if (cho <= 2)
//		musicData.mode = cho;
//}
//
//// 删除音乐
//void CmdMusicPlayer::deleteMusic()
//{
//	system("cls");
//	showMusicName();
//	cout << "（提示：输入想删除音乐的序号并回车，若输入完毕，输入非数字即可！）" << endl;
//	cout << "请输入你想删除的音乐：";
//	vector<int> tvec;
//	int it = 0;
//	while (cin >> it)
//	{
//		tvec.push_back(it);
//		cout << "请输入你想删除的音乐：";
//	}
//	cin.clear();
//	while (cin.get() != '\n');											// 清空输入缓冲区
//	cout << "将要删除以下歌曲：" << endl;
//	for (auto x : tvec)
//	{
//		cout << x << "  ";
//		wcout << musicData.musicName.at(x) << endl;
//	}
//	cout << "是否删除?（y/n）:";
//	char ch = 0;
//	cin >> ch;
//	if (ch == 'y' || ch == 'Y')
//	{
//		cout << "删除音乐中，请稍后..." << endl;
//		musicData.deleteMusicData(tvec);								// 删除其中的音乐
//		cout << "音乐列表删除完成！" << endl;
//		cout << "文件music.mn内容更新中..." << endl;
//
//		fstream ft("music.mn", ios_base::out);
//		if (!ft.is_open())
//		{
//			cerr << "文件music.mn打开失败了，请稍后重试！" << endl;
//		}
//		else
//		{
//			musicData.wFileMusic(ft);
//			ft.close();
//			cout << "文件music.mn内容更新完成！" << endl;
//		}
//	}
//	else
//	{
//		cout << "已取消音乐删除！" << endl;
//	}
//	Sleep(2000);
//}
//
//// 选择音乐播放
//void CmdMusicPlayer::chooseMusicPlay()
//{
//	system("cls");
//	size_t choose = 0;
//	showMusicName();
//	cout << "（提示：输入超出范围的值将退出选择！）" << endl;
//	cout << "请选择你想播放的歌曲：";
//	cin >> choose;
//	if (choose < musicData.musicName.size())
//	{
//		musicData.number = choose;
//		if (musicData.status)
//			musicData.closeMusic();
//		musicData.openMusic(choose);
//	}
//	else
//	{
//		cout << "已退出选择！" << endl;
//		Sleep(1000);
//	}
//	system("cls");
//}
//
//// 显示音乐播放列表
//void CmdMusicPlayer::showMusicName()
//{
//	if (musicData.musicName.empty())
//	{
//		cout << "无播放列表！" << endl;
//		return;
//	}
//
//	size_t i = 0;
//	cout << "播放列表：" << endl;
//	for (auto& x : musicData.musicName)
//	{
//		cout << i++ << "  "; 
//		wcout << x << endl;
//	}
//}
//
//// 显示播放信息
//void CmdMusicPlayer::showPlayInformation()
//{
//	int t = 0, t0 = 0;
//	if (musicData.status)
//	{
//		t = musicData.getPlayerBackTimeMusic();
//		t0 = musicData.getTotalTime();
//	}
//	auto chonum = [](size_t a, size_t b)noexcept {return a > b ? 0 : a; };		// lamdom表达式
//	if (musicData.status && t == t0)											// 播放是否结束
//	{
//		musicData.status = 3;
//		musicData.closeMusic();
//		if (musicData.mode == 0)
//		{
//			musicData.openMusic(musicData.number);
//			system("cls");
//		}
//		else if (musicData.mode == 1)
//		{
//			musicData.openMusic(chonum(musicData.number + 1, musicData.musicName.size() - 1));
//			system("cls");
//			if (!musicData.status)
//			{
//				wcout << L"音乐" << musicData.nowMusicName << L"播放失败！" << endl;
//				wcout << L"即将播放下一首音乐：" << musicData.musicName.at(chonum(musicData.number + 1,
//					musicData.musicName.size() - 1)) << endl;
//				Sleep(2000);
//				musicData.openMusic(chonum(musicData.number + 1, musicData.musicName.size()));
//			}
//			system("cls");
//		}
//		else if (musicData.mode == 2)
//		{
//			musicData.openMusic(rand() % musicData.musicName.size());
//			system("cls");
//			if (!musicData.status)
//			{
//				const size_t stemp = rand() % musicData.musicName.size();
//				wcout << L"音乐：" << musicData.nowMusicName << L"  播放失败！" << endl;
//				wcout << L"即将播放下一首音乐：" << musicData.musicName.at(stemp) << endl;
//				Sleep(2000);
//				musicData.openMusic(stemp);
//			}
//			system("cls");
//		}
//	}
//	static short x = 120 / 2;
//	if (musicData.status == 1)
//		x = --x < 0 ? 120 : --x;
//	pos(0, 27);
//	cout << "                                                                                                                            ";
//	pos(x, 27);
//	if (musicData.status == 0)
//		wcout << L"暂无播放！" << endl;
//	else if (musicData.status == 1)
//		wcout << L"正在播放：" << musicData.nowMusicName << endl;
//	else if (musicData.status == 2)
//		wcout << L"已暂停播放：" << musicData.nowMusicName << endl;
//	else if (musicData.status == 3)
//		wcout << L"播放结束：" << musicData.nowMusicName << endl;
//
//	pos(0, 28);
//	cout << "                                                                                                                            ";
//	pos(0, 28);
//	cout << "已播放：" << t / 60 << "分" << t % 60 << "秒" << "			"
//		<< "总时长：" << t0 / 60 << "分" << t0 % 60 << "秒" << "		"
//		<< "音量：" << musicData.volume << "		"
//		<< "播放模式：";
//	if (musicData.mode == 0)
//		cout << "单曲循环" << endl;
//	else if (musicData.mode == 1)
//		cout << "顺序播放" << endl;
//	else if (musicData.mode == 2)
//		cout << "随机播放" << endl;
//}
//
//
///**************************************************************
//* CmdMusicPlayer public method                                   *
//**************************************************************/
//
//// 功能选择
//int CmdMusicPlayer::chooseFunction()
//{
//	system("cls");
//	do
//	{
//		showPlayInformation();
//
//		pos(0, 0);
//		cout << "0.退出" << endl;
//		cout << "1.暂停" << endl;
//		cout << "2.继续" << endl;
//		cout << "3.音量调节" << endl;
//		cout << "4.删除音乐" << endl;
//		cout << "5.选择音乐播放" << endl;
//		cout << "6.设置播放模式" << endl;
//		cout << "7.设置搜索路径" << endl;
//		cout << "请输入你想选择的功能：";
//		Sleep(500);
//	} while (!_kbhit());
//
//	size_t n = 0;
//	cin >> n;
//	switch (n)
//	{
//	case 0:cout << "已退出播放！" << endl; Sleep(1000); return 0;
//	case 1:musicData.pauseMusic(); break;
//	case 2:musicData.playMusic(); break;
//	case 3:setMusicVolume(); break;
//	case 4:deleteMusic(); break;
//	case 5:chooseMusicPlay(); break;
//	case 6:setPlayMode(); break;
//	case 7:setFilePath(); break;
//	default:cout << "选择错误！" << endl; break;
//	}
//	return 1;
//}



/**********************************************************************************************************************
*  class GuiMusicPlayer                                                                                               *
**********************************************************************************************************************/

/**************************************************************
* GuiMusicPlayer private method                               *
**************************************************************/

bool GuiMusicPlayer::findBgPicture()noexcept
{
	long hFile = 0;
	struct _finddata_t fileinfo;																		// 文件信息 
	if ((hFile = _findfirst("background.jpg", &fileinfo)) != -1)										// 查找所有文件
		return true;

	return false;
}

void GuiMusicPlayer::ui()
{
	constexpr COLORREF c0 = YELLOW;
	settextcolor(BLUE);																					// 字体颜色
	settextstyle(15, 0, L"宋体");																		// 字体样式
	setbkmode(TRANSPARENT);																				// 文字输出背景透明
	outtextxy(WIDTH - 40, 10, L"设置");
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

	setlinecolor(WHITE);
	const array<POINT, 14> a2{ WIDTH / 2 + 120,HEIGHT - 34,WIDTH / 2 + 125,HEIGHT - 34,WIDTH / 2 + 129,HEIGHT - 38,
		WIDTH / 2 + 129,HEIGHT - 22,WIDTH / 2 + 125,HEIGHT - 26,WIDTH / 2 + 120,HEIGHT - 26,WIDTH / 2 + 120,HEIGHT - 34 };
	polyline(&a2.at(0), 7);																				// 画小喇叭

	arc(WIDTH / 2 + 125, HEIGHT - 35, WIDTH / 2 + 135, HEIGHT - 25, -3.1415926 / 3, 3.1415926 / 3);		// 画圆弧

	line(WIDTH / 2 + 140, HEIGHT - 30, WIDTH / 2 + 240, HEIGHT - 30);									// 画音量条

	drawStartPause();
	showPlayList();
}

void GuiMusicPlayer::drawStartPause()
{
	setfillcolor(YELLOW);
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

void GuiMusicPlayer::showPlayList()
{
	settextcolor(0XAA00AA);																				// 字体颜色
	settextstyle(15, 0, L"宋体");																		// 字体样式

	if (!musicData.musicName.empty())
	{
		for (int i = numRange.at(0); i <=numRange.at(1) ; ++i)
		{
			outtextxy(260, 40 + 20 + i * 40, musicData.musicName.at(i).c_str());
		}

	}
	else
	{
		outtextxy(260, 40 + 10, L"列表为空！");
	}
}

void GuiMusicPlayer::drawPlayInformation()
{
	settextcolor(0XAA00AA);																			// 字体颜色
	static int i = 0;
	static IMAGE i0, i1, i2, i3, i4;
	if(!i)																							// 只读取一次还区域
	{
		getimage(&i0, 30, HEIGHT - 45, WIDTH / 2 - 100, 20);										// 歌曲名下的背景

		getimage(&i1, WIDTH / 2 + 110, HEIGHT - 40, 140, HEIGHT - 20);								// 音量条

		getimage(&i2, WIDTH / 2 + 300, HEIGHT - 40, 60, 20);										// 播放模式

		getimage(&i3, 30, HEIGHT - 30, 120, 20);													// 音乐时间

		getimage(&i4, 0, HEIGHT - 65, WIDTH, 10);													// 进度条
		++i;
	}
		
	putimage(30, HEIGHT - 45, &i0);																	// 先显示背景
	putimage(WIDTH / 2 + 110, HEIGHT - 40, &i1);													// 音量条
	putimage(WIDTH / 2 + 300, HEIGHT - 40, &i2);													// 播放模式
	putimage(30, HEIGHT - 30, &i3);																	// 音乐时间
	putimage(0, HEIGHT - 65, &i4);																	// 进度条
	settextstyle(14, 0, L"宋体");
	if (musicData.nowMusicName.size() < 26)
		outtextxy(30, HEIGHT - 45, musicData.nowMusicName.c_str());									// 再显示音乐名
	else
		outtextxy(30, HEIGHT - 45, (musicData.nowMusicName.substr(0, 26) + L"...").c_str());

	static int t0 = 0, t1 = 0;																		// 显示音乐时长
	{
		if (musicData.status)
		{
			t0 = musicData.getPlayerBackTimeMusic();
			t1 = musicData.getTotalTime();
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
	setlinecolor(YELLOW);
	line(0, HEIGHT - 60, static_cast<int>(WIDTH * ((t0 + 0.0) / t1)), HEIGHT - 60);
	line(0, HEIGHT - 59, static_cast<int>(WIDTH * ((t0 + 0.0) / t1)), HEIGHT - 60);
	setfillcolor(YELLOW);
	solidcircle(static_cast<int>(WIDTH * ((t0 + 0.0) / t1)), HEIGHT - 60, 4);						// 画进度条小球


	if (musicData.mode == 0)
		outtextxy(WIDTH / 2 + 300, HEIGHT - 40, L"单曲循环");
	else if (musicData.mode == 1)
		outtextxy(WIDTH / 2 + 300, HEIGHT - 40, L"顺序播放");
	else if (musicData.mode == 2)
		outtextxy(WIDTH / 2 + 300, HEIGHT - 40, L"随机播放");


	setlinecolor(YELLOW);
	setfillcolor(YELLOW);
	line(WIDTH / 2 + 140, HEIGHT - 30, WIDTH / 2 + 140 + musicData.volume / 10, HEIGHT - 30);
	line(WIDTH / 2 + 140, HEIGHT - 31, WIDTH / 2 + 140 + musicData.volume / 10, HEIGHT - 31);
	solidrectangle(WIDTH / 2 + 140 + musicData.volume / 10, HEIGHT - 35,
		WIDTH / 2 + 140 + musicData.volume / 10 + 3, HEIGHT - 25);									// 画音量竖小短线

}


/**************************************************************
* GuiMusicPlayer public method                                *
**************************************************************/

GuiMusicPlayer::GuiMusicPlayer()
{
	srand(time(nullptr) & 0xffffffff);
	initgraph(WIDTH, HEIGHT);

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

			if (m0.mkRButton)																// 右键退出
				break;
			
			if (m0.x > 260 && m0.x < WIDTH && m0.y>40 && m0.y < HEIGHT - 60)				// 鼠标在播放列表
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

				if (m0.uMsg == WM_MOUSEWHEEL)												// 鼠标滚动消息
				{																			// 更新播放列表显示内容
					const int tw = m0.wheel / 120;
					if (tw >= 0)
						numRange.at(0) = static_cast<size_t>(numRange.at(0) + tw) > (musicData.musicName.size() - 1 - 12) ?
						numRange.at(0) + tw : numRange.at(0);
					else
						numRange.at(0) = (numRange.at(0) + tw) > 0 ? (numRange.at(0) + tw) : 0;
					numRange.at(1) = numRange.at(0) + 12;
				}

				
			}

			if (m0.mkLButton)
			{
				if (pow(m0.x - WIDTH / 2, 2) + pow(m0.y - HEIGHT + 30, 2) <= 400)			// 是否在圆形播放，暂停按钮内
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
					if (musicData.mode == 0)														// 顺序变更播放模式
						musicData.mode = 1;
					else if (musicData.mode == 1)
						musicData.mode = 2;
					else if (musicData.mode == 2)
						musicData.mode = 0;
				}

				FlushMouseMsgBuffer();
			}
		}
		showPlayList();
		drawPlayInformation();
		drawStartPause();

		static int t0 = 0, t1 = 0;
		if (musicData.status)
		{
			t0 = musicData.getPlayerBackTimeMusic();
			t1 = musicData.getTotalTime();
		}

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
		
		FlushBatchDraw();
	}

	EndBatchDraw();
}