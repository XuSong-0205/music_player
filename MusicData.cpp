#include "MusicData.h"


/****************************************************************************************************************
 *  class MusicData																								*
 ****************************************************************************************************************/

/****************************************************************
 * MusicData private method										*
 ****************************************************************/


#ifdef _DEBUG
FILE* STD_COUT_STREAM = nullptr;								// 指向标准输出流的文件指针
#endif // _DEBUG


// 打开并播放音乐
void MusicData::openMusic(size_t num)
{
	if (musicPathName.empty() || num > musicPathName.size() - 1)
	{
#ifdef _DEBUG
		std::wcout << L"参数不符合要求，请重试！" << std::endl;
#endif // _DEBUG

		status = 0;
		return;
	}

	nowMusicName = musicName.at(num);								// 设置正在操作的音乐名
	number = num;													// 设置正在操作的音乐名编号

	if (musicMci.open(musicPathName.at(num).c_str()))
	{
#ifdef _DEBUG
		std::wcout << L"音乐 " << nowMusicName << L" 打开成功！" << std::endl;
#endif // _DEBUG

		playMusic();
		musicMci.setVolume(volume);
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"音乐 " << nowMusicName << L" 打开失败！" << std::endl;
#endif // _DEBUG
	}
}

// 播放音乐
void MusicData::playMusic()
{
	if (musicMci.play())
	{
#ifdef _DEBUG
		std::wcout << L"音乐 " << nowMusicName << L" 播放成功！" << std::endl;
#endif // _DEBUG

		status = 1;
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"音乐 " << nowMusicName << L" 播放失败！" << std::endl;
#endif // _DEBUG

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
#ifdef _DEBUG
			std::wcout << L"音乐 " << nowMusicName << L" 已暂停！" << std::endl;
#endif // _DEBUG

			status = 2;
		}
		else
		{
#ifdef _DEBUG
			std::wcout << L"音乐 " << nowMusicName << L" 暂停失败！" << std::endl;
#endif // _DEBUG
		}
	}
}

// 停止播放
void MusicData::stopMusic()
{
	if (musicMci.stop())
	{
#ifdef _DEBUG
		std::wcout << L"音乐 " << nowMusicName << L" 已暂停！" << std::endl;
#endif // _DEBUG

		status = 0;
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"音乐 " << nowMusicName << L" 暂停失败！" << std::endl;
#endif // _DEBUG
	}
}

// 关闭音乐
void MusicData::closeMusic()
{
	if (musicMci.close())
	{
#ifdef _DEBUG
		std::wcout << L"音乐 " << nowMusicName << L" 已关闭！" << std::endl;
#endif // _DEBUG

		status = 0;
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"音乐 " << nowMusicName << L" 关闭失败！" << std::endl;
#endif // _DEBUG
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
#ifdef _DEBUG
		else
		{
			std::wcout << L"音量设置失败！" << std::endl;
		}
#endif // _DEBUG
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
#ifdef _DEBUG
			std::wcout << L"设置播放位置失败！" << std::endl;
#endif // _DEBUG
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
#ifdef _DEBUG
			std::wcout << L"获取播放时长失败！" << std::endl;
#endif // _DEBUG

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
#ifdef _DEBUG
			std::wcout << L"获取总时长失败！" << std::endl;
#endif // _DEBUG

			return 0;
		}
		return totalTime / 1000;
	}

	return 0;
}


// 从文件 filePath.ini 中读取搜索路径
void MusicData::getFilePath()
{
	FILE* fp = nullptr;
	auto err = _wfopen_s(&fp, L"filePath.ini", L"r");
	if (err != 0)
	{
#ifdef _DEBUG
		std::wcout << L"文件 filePath.ini 打开失败" << std::endl;
		std::wcout << L"程序将新建文件 filePath.ini" << std::endl;
#endif // _DEBUG

		err = _wfopen_s(&fp, L"filePath.ini", L"w, ccs=UTF-16LE");
		if (err == 0)
		{
#ifdef _DEBUG
			std::wcout << L"文件 filePath.ini 创建成功" << std::endl;
#endif // _DEBUG

			fputws(L"; 请在“path=”后写入合适的音乐搜索路径\n", fp);
			fputws(L"; 注意，请在修改后确保文件编码为 UTF-16LE，若不是，请将文件另存为 UTF-16LE 编码并保存\n", fp);
			// 写入配置文件注释
			fclose(fp);										// 关闭文件

			filePath = L".";								// 设置默认为当前路径
			wFilePath();									// 写入配置信息
		}
	}
	else
	{
		rFilePath();										// 读取配置信息
		fclose(fp);											// 关闭文件
	}
}

// 获取特定格式的文件名    
void MusicData::findMusicName(const wstring& path)
{
	WIN32_FIND_DATA fileinfo;														// 文件信息
	auto hFind = FindFirstFile((path + L"\\*." + musicFormat).c_str(), &fileinfo);	// 查找当前文件夹是否存在指定格式文件
	if (hFind != INVALID_HANDLE_VALUE)												// 找到指定格式文件
	{
		do
		{
			wstring str = fileinfo.cFileName;
			musicPathName.push_back(path + L"\\" + str);							// 写入音乐全路径名
			musicName.push_back(str.substr(0, str.size() - musicFormat.size() - 1));// 写入音乐名
		} while (FindNextFile(hFind, &fileinfo));
	}
	FindClose(hFind);

	hFind = FindFirstFile((path + L"\\*").c_str(), &fileinfo);						// 查找子文件夹，查找所有文件
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))				// 判断是否为文件夹
			{
				if (fileinfo.cFileName[0] != L'.')									// 过滤文件夹 "." 和 ".."
				{
					findMusicName(path + L"\\" + fileinfo.cFileName);				// 递归遍历文件夹
				}
			}
		} while (FindNextFile(hFind, &fileinfo));
	}
	FindClose(hFind);
}

// 将 filePath 写入文件 filePath.ini
void MusicData::wFilePath()
{
	// 在 .\\filePath.ini 中写入 节 [filePath] 键=值 path=filePath
	WritePrivateProfileString(L"filePath", L"path", filePath.c_str(), L".\\filePath.ini");
}

// 将文件 filePath.ini 读取到 filePath
void MusicData::rFilePath()
{
	// 读取对应节，键的值
	array<wchar_t, 256> wptr{ L'\0' };
	const DWORD d = GetPrivateProfileString(L"filePath", L"path", L".",
		&wptr.at(0), 256, L".\\filePath.ini");

	filePath = &wptr.at(0);
}

// 将 musicPathName 写入文件 music.mn 中
void MusicData::wFileMusic(FILE* fp)
{
	if (!musicPathName.empty())
	{
		const int len = static_cast<int>(musicPathName.size());
		fputws(musicPathName.at(0).c_str(), fp);

		for (int i = 1; i < len; ++i)
		{
			fputws(L"\n", fp);
			fputws(musicPathName.at(i).c_str(), fp);
		}
#ifdef _DEBUG
		std::wcout << L"文件 music.mn 写入完毕" << std::endl;
#endif // _DEBUG
	}
#ifdef _DEBUG
	else
	{
		std::wcout << L"musicPathName 为空" << std::endl;
	}
#endif // _DEBUG
}

// 从 music.mn 读取到 musicPathName 和 musicName 中
void MusicData::rFileMusic(FILE* fp)
{
	wstring s;
	array<wchar_t, 256> wMusic{ L'\0' };

	while (!feof(fp))
	{
		fgetws(&wMusic.at(0), 256, fp);
		s = &wMusic.at(0);
		if (s.empty())
		{
			continue;															// 若为空，跳过
		}
		if (s.back() == L'\n')
		{
			s.pop_back();														// 去掉末尾的换行符
			if (s.empty())
			{
				continue;
			}
		}
		musicPathName.push_back(s);												// 写入音乐全路径名
		const auto pos = s.rfind(L"\\");
		s = s.substr(pos + 1, s.size() - pos - 1 - musicFormat.size() - 1);		// 截取出音乐名
		musicName.push_back(s);													// 写入音乐名
	}
#ifdef _DEBUG
	std::wcout << L"文件 musci.mn 读取完毕" << std::endl;
	if (musicPathName.empty())
	{
		std::wcout << L"musicPathName 为空" << std::endl;
	}
#endif // _DEBUG
}


/****************************************************************
 *  MusicData public method										*
 ****************************************************************/

 // 默认构造函数
MusicData::MusicData()
{
	srand(time(nullptr) & 0xffffffff);

	AllocConsole();														// 添加控制台
#ifdef _DEBUG
	freopen_s(&STD_COUT_STREAM, "CON", "w", stdout);					// 重定向标准输出流
	std::wcout.imbue(std::locale("", LC_CTYPE));
#endif // _DEBUG


	FILE* fp = nullptr;													// 文件指针
	auto err = _wfopen_s(&fp, L"music.mn", L"r, ccs=UTF-16LE");			// 以 只读，UTF-16LE 编码格式打开文件
	if (err == 0)														// 打开成功
	{
		rFileMusic(fp);													// 若文件存在，直接读取其内容
		fclose(fp);														// 关闭文件
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"文件 music.mn 打开失败" << std::endl;
		std::wcout << L"程序将新建文件 music.mn，并进行音乐搜索" << std::endl;
#endif // _DEBUG

		err = _wfopen_s(&fp, L"music.mn", L"w, ccs=UTF-16LE");			// 创建以 UTF-16LE 编码的文件
		if (err == 0)													// 打开成功，创建成功
		{
#ifdef _DEBUG
			std::wcout << L"文件 music.mn 创建成功" << std::endl;
#endif // _DEBUG

			getFilePath();												// 获取音乐的搜索路径

#ifdef _DEBUG
			std::wcout << L"音乐搜索路径为：" << filePath << std::endl;
			std::wcout << L"音乐搜索中..." << std::endl;
#endif // _DEBUG

			findMusicName(filePath);

#ifdef _DEBUG
			std::wcout << L"音乐搜索完毕" << std::endl;
#endif // _DEBUG

			wFileMusic(fp);												// 写入
			fclose(fp);													// 关闭它
		}
	}
}

// 析构函数
MusicData::~MusicData()
{
	closeMusic();

#ifdef _DEBUG
	fclose(STD_COUT_STREAM);
#endif // _DEBUG
}