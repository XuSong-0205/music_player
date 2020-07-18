#include "MusicData.h"


/**************************************************************************************************************
 *  class MusicData                                                                                           *
 **************************************************************************************************************/

/**************************************************************
 * MusicData private method                                   *
 **************************************************************/

// 打开并播放音乐
void MusicData::openMusic(size_t num)
{
	if (musicPathName.empty() || num > musicPathName.size() - 1)
	{
#ifdef DEBUG
		wcout << L"参数不符合要求，请重试！" << endl;
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
		wcout << L"音乐 " << nowMusicName << L" 打开成功！" << endl;
#endif // DEBUG

		playMusic();
		musicMci.setVolume(volume);
	}
	else
	{
#ifdef DEBUG
		wcout << L"音乐 " << nowMusicName << L" 打开失败！" << endl;
#endif // DEBUG
	}
}

// 播放音乐
void MusicData::playMusic()
{
	if (musicMci.play())
	{
#ifdef DEBUG
		wcout << L"音乐 " << nowMusicName << L" 播放成功！" << endl;
#endif // DEBUG

		status = 1;
	}
	else
	{
#ifdef DEBUG
		wcout << L"音乐 " << nowMusicName << L" 播放失败！" << endl;
#endif // DEBUG

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
#ifdef DEBUG
			wcout << L"音乐 " << nowMusicName << L" 已暂停！" << endl;
#endif // DEBUG

			status = 2;
		}
		else
		{
#ifdef DEBUG
			wcout << L"音乐 " << nowMusicName << L" 暂停失败！" << endl;
#endif // DEBUG
		}
	}
}

// 停止播放
void MusicData::stopMusic()
{
	if (musicMci.stop())
	{
#ifdef DEBUG
		wcout << L"音乐 " << nowMusicName << L" 已暂停！" << endl;
#endif // DEBUG

		status = 0;
	}
	else
	{
#ifdef DEBUG
		wcout << L"音乐 " << nowMusicName << L" 暂停失败！" << endl;
#endif // DEBUG
	}
}

// 关闭音乐
void MusicData::closeMusic()
{
	if (musicMci.close())
	{
#ifdef DEBUG
		wcout << L"音乐 " << nowMusicName << L" 已关闭！" << endl;
#endif // DEBUG

		status = 0;
	}
	else
	{
#ifdef DEBUG
		wcout << L"音乐 " << nowMusicName << L" 关闭失败！" << endl;
#endif // DEBUG
	}
}

// 设置音乐音量
// 参数允许范围，0 - 1000
void MusicData::setMusicVolume(size_t vol)
{
	if (status)
	{
		if (musicMci.setVolume(vol))
			volume = vol;
#ifdef DEBUG
		else
		{
			wcout << L"音量设置失败！" << endl;
		}
#endif // DEBUG
	}
	else
	{
		volume = vol;
	}
}

// 设置播放位置
// 参数单位，ms
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
			wcout << L"设置播放位置失败！" << endl;
#endif // DEBUG
			return false;
		}
	}

	return false;
}

// 获取音乐当前播放时间，单位 s
int MusicData::getMusicCurrentTime()
{
	if (status)
	{
		DWORD playTime = 0;
		if (!musicMci.getCurrentTime(playTime))
		{
#ifdef DEBUG
			wcout << L"获取播放时长失败！" << endl;
#endif // DEBUG

			return 0;
		}
		return playTime / 1000;
	}

	return 0;
}

// 获取音乐总播放时间，单位 s
int MusicData::getMusicTotalTime()
{
	if (status)
	{
		DWORD totalTime = 0;
		if (!musicMci.getTotalTime(totalTime))
		{
#ifdef DEBUG
			wcout << L"获取总时长失败！" << endl;
#endif // DEBUG

			return 0;
		}
		return totalTime / 1000;
	}

	return 0;
}


// 从文件 filePath.ini 中读取搜索路径
void MusicData::getFilePath()
{
	fstream fPath("filePath.ini", ios_base::in);
	if (!fPath.is_open())
	{
#ifdef DEBUG
		wcout << L"文件 filePath.ini 打开失败！" << endl;
		wcerr << L"程序将尝试新建此文件!" << endl;
#endif // DEBUG

		fPath.open("filePath.ini", ios_base::in | ios_base::out | ios_base::trunc);
		if (fPath.is_open())
		{
#ifdef DEBUG
			wcout << L"文件 filePath.ini 创建成功！" << endl;
#endif // DEBUG

			filePath = "..";								// 设置默认为当前路径
			wFilePath(fPath);								// 写入
		}
	}
	else
	{
		rFilePath(fPath);									// 读取
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

// 将 filePath 写入文件 filePath.ini
void MusicData::wFilePath(fstream& file)
{
	file << filePath;
}

// 将文件 filePath.ini 读取到 filePath
void MusicData::rFilePath(fstream& file)
{
	string temp;
	getline(file, temp);
	if (temp.empty())
	{
#ifdef DEBUG
		wcout << L"文件内容为空！" << endl;
		wcout << L"文件路径设置为默认！" << endl;
#endif // DEBUG

		filePath = "..";							// 默认为当前路径
		wFilePath(file);							// 写入
	}
	else
	{
		filePath = temp;
	}
}

// 将 musicPathName 写入文件 music.mn 中
void MusicData::wFileMusic(fstream& file)
{
	if (!musicPathName.empty())
	{
		const int n = musicPathName.size();
		file << musicPathName.at(0);
		for (int i = 1; i < n; ++i)
			file << "\n" << musicPathName.at(i);
#ifdef DEBUG
		wcout << L"文件 music.mn 写入完毕！" << endl;
#endif // DEBUG
	}
#ifdef DEBUG
	else
	{
		wcout << L"musicPathName 为空！" << endl;
	}
#endif // DEBUG
}

// 从 music.mn 读取到 musicPathName 和 musicName 中
void MusicData::rFileMusic(fstream& file)
{
	string s;
	if (file.eof())
	{
#ifdef DEBUG
		wcout << L"文件 music.mn 为空！" << endl;
#endif // DEBUG
	}

	while (!file.eof())
	{
		getline(file, s);
		musicPathName.push_back(s);												// 写入音乐全路径名
		const auto pos = s.rfind("\\");
		s = s.substr(pos + 1, s.size() - pos - 1 - musicFormat.size() - 1);		// 截取出音乐名
		musicName.push_back(stringTowstring(s));								// 写入音乐名
	}
#ifdef DEBUG
	wcout << L"文件 musci.mn 读取完毕！" << endl;
#endif // DEBUG
}


/**************************************************************
 *  MusicData public method                                   *
 **************************************************************/

// 默认构造函数
MusicData::MusicData()
{
	srand(time(nullptr) & 0xffffffff);
	getFilePath();																// 初始化搜索的文件路径

#ifdef DEBUG
	wcout.imbue(std::locale("", LC_CTYPE));
	wcout << L"文件搜索路径为：" << stringTowstring(filePath) << endl;
#endif // DEBUG

	fstream file("music.mn", ios_base::in);
	if (!file.is_open())
	{
#ifdef DEBUG
		wcerr << L"文件 music.mn 打开失败，此文件可能不存在" << endl;
		wcerr << L"程序将尝试新建此文件，并初始化文件内容" << endl;
#endif // DEBUG

		file.open("music.mn", ios_base::out);			// 新建文件
		if (file.is_open())
		{
#ifdef DEBUG
			wcout << L"文件 music.mn 创建成功！" << endl;
			wcout << L"正在搜索音乐中..." << endl;
#endif // DEBUG

			findMusicName(filePath);

#ifdef DEBUG
			wcout << L"音乐搜索完毕！" << endl;
#endif // DEBUG

			wFileMusic(file);
		}
	}
	else
	{
		rFileMusic(file);								// 若文件存在，直接读取其内容
	}
	file.close();										// 关闭文件

	if (!musicPathName.empty() && musicPathName.back() == "")
		musicPathName.pop_back();						// 删除最后一个空白行

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