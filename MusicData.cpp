#include "MusicData.h"


/****************************************************************************************************************
 *  class MusicData																								*
 ****************************************************************************************************************/

/****************************************************************
 * MusicData private method										*
 ****************************************************************/


#ifdef _DEBUG
FILE* STD_COUT_STREAM = nullptr;								// ָ���׼��������ļ�ָ��
#endif // _DEBUG


// �򿪲���������
void MusicData::openMusic(size_t num)
{
	if (musicPathName.empty() || num > musicPathName.size() - 1)
	{
#ifdef _DEBUG
		std::wcout << L"����������Ҫ�������ԣ�" << std::endl;
#endif // _DEBUG

		status = 0;
		return;
	}

	nowMusicName = musicName.at(num);								// �������ڲ�����������
	number = num;													// �������ڲ��������������

	if (musicMci.open(musicPathName.at(num).c_str()))
	{
#ifdef _DEBUG
		std::wcout << L"���� " << nowMusicName << L" �򿪳ɹ���" << std::endl;
#endif // _DEBUG

		playMusic();
		musicMci.setVolume(volume);
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"���� " << nowMusicName << L" ��ʧ�ܣ�" << std::endl;
#endif // _DEBUG
	}
}

// ��������
void MusicData::playMusic()
{
	if (musicMci.play())
	{
#ifdef _DEBUG
		std::wcout << L"���� " << nowMusicName << L" ���ųɹ���" << std::endl;
#endif // _DEBUG

		status = 1;
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"���� " << nowMusicName << L" ����ʧ�ܣ�" << std::endl;
#endif // _DEBUG

		status = 0;
	}
}

// ��ͣ����
void MusicData::pauseMusic()
{
	if (status)
	{
		if (musicMci.pause())
		{
#ifdef _DEBUG
			std::wcout << L"���� " << nowMusicName << L" ����ͣ��" << std::endl;
#endif // _DEBUG

			status = 2;
		}
		else
		{
#ifdef _DEBUG
			std::wcout << L"���� " << nowMusicName << L" ��ͣʧ�ܣ�" << std::endl;
#endif // _DEBUG
		}
	}
}

// ֹͣ����
void MusicData::stopMusic()
{
	if (musicMci.stop())
	{
#ifdef _DEBUG
		std::wcout << L"���� " << nowMusicName << L" ����ͣ��" << std::endl;
#endif // _DEBUG

		status = 0;
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"���� " << nowMusicName << L" ��ͣʧ�ܣ�" << std::endl;
#endif // _DEBUG
	}
}

// �ر�����
void MusicData::closeMusic()
{
	if (musicMci.close())
	{
#ifdef _DEBUG
		std::wcout << L"���� " << nowMusicName << L" �ѹرգ�" << std::endl;
#endif // _DEBUG

		status = 0;
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"���� " << nowMusicName << L" �ر�ʧ�ܣ�" << std::endl;
#endif // _DEBUG
	}
}

// ������������
// ��������Χ��0 - 1000
void MusicData::setMusicVolume(size_t vol)
{
	if (status)
	{
		if (musicMci.setVolume(vol))
			volume = vol;
#ifdef _DEBUG
		else
		{
			std::wcout << L"��������ʧ�ܣ�" << std::endl;
		}
#endif // _DEBUG
	}
	else
	{
		volume = vol;
	}
}

// ���ò���λ��
// ������λ��ms
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
			std::wcout << L"���ò���λ��ʧ�ܣ�" << std::endl;
#endif // _DEBUG
			return false;
		}
	}

	return false;
}

// ��ȡ���ֵ�ǰ����ʱ�䣬��λ s
int MusicData::getMusicCurrentTime()
{
	if (status)
	{
		DWORD playTime = 0;
		if (!musicMci.getCurrentTime(playTime))
		{
#ifdef _DEBUG
			std::wcout << L"��ȡ����ʱ��ʧ�ܣ�" << std::endl;
#endif // _DEBUG

			return 0;
		}
		return playTime / 1000;
	}

	return 0;
}

// ��ȡ�����ܲ���ʱ�䣬��λ s
int MusicData::getMusicTotalTime()
{
	if (status)
	{
		DWORD totalTime = 0;
		if (!musicMci.getTotalTime(totalTime))
		{
#ifdef _DEBUG
			std::wcout << L"��ȡ��ʱ��ʧ�ܣ�" << std::endl;
#endif // _DEBUG

			return 0;
		}
		return totalTime / 1000;
	}

	return 0;
}

// ������һ��
void MusicData::prevMusic()
{
	if (status)													// �Ƿ���Ҫ�ر�����
	{
		closeMusic();
	}

	if (mode == 0 || mode == 1)									// ������������ţ���һ��
	{
		openMusic(number == 0 ? musicName.size() - 1 : number - 1);
	}
	else														// ��������ţ����һ��
	{
		openMusic(rand() % musicName.size());
	}
}

// ������һ��
void MusicData::nextMusic()
{
	if (status)
	{
		closeMusic();
	}

	if (mode == 0 || mode == 1)
	{
		openMusic(number + 1 > musicName.size() - 1 ? 0 : number + 1);
	}
	else
	{
		openMusic(rand() % musicName.size());
	}
}


// ���ļ� filePath.ini �ж�ȡ����·��
void MusicData::getFilePath()
{
	FILE* fp = nullptr;
	auto err = _wfopen_s(&fp, L"filePath.ini", L"r");
	if (err != 0)
	{
#ifdef _DEBUG
		std::wcout << L"�ļ� filePath.ini ��ʧ��" << std::endl;
		std::wcout << L"�����½��ļ� filePath.ini" << std::endl;
#endif // _DEBUG

		err = _wfopen_s(&fp, L"filePath.ini", L"w, ccs=UTF-16LE");
		if (err == 0)
		{
#ifdef _DEBUG
			std::wcout << L"�ļ� filePath.ini �����ɹ�" << std::endl;
#endif // _DEBUG

			fputws(L"; ���ڡ�path=����д����ʵ���������·��\n", fp);
			fputws(L"; ע�⣬�����޸ĺ�ȷ���ļ�����Ϊ UTF-16LE�������ǣ��뽫�ļ����Ϊ UTF-16LE ���벢����\n", fp);
			// д�������ļ�ע��
			fclose(fp);										// �ر��ļ�

			filePath = L".";								// ����Ĭ��Ϊ��ǰ·��
			wFilePath();									// д��������Ϣ
		}
	}
	else
	{
		rFilePath();										// ��ȡ������Ϣ
		fclose(fp);											// �ر��ļ�
	}
}

// ��ȡ�ض���ʽ���ļ���    
void MusicData::findMusicName(const wstring& path)
{
	WIN32_FIND_DATA fileinfo;														// �ļ���Ϣ
	auto hFind = FindFirstFile((path + L"\\*." + musicFormat).c_str(), &fileinfo);	// ���ҵ�ǰ�ļ����Ƿ����ָ����ʽ�ļ�
	if (hFind != INVALID_HANDLE_VALUE)												// �ҵ�ָ����ʽ�ļ�
	{
		do
		{
			wstring str = fileinfo.cFileName;
			musicPathName.push_back(path + L"\\" + str);							// д������ȫ·����
			musicName.push_back(str.substr(0, str.size() - musicFormat.size() - 1));// д��������
		} while (FindNextFile(hFind, &fileinfo));
	}
	FindClose(hFind);

	hFind = FindFirstFile((path + L"\\*").c_str(), &fileinfo);						// �������ļ��У����������ļ�
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))				// �ж��Ƿ�Ϊ�ļ���
			{
				if (fileinfo.cFileName[0] != L'.')									// �����ļ��� "." �� ".."
				{
					findMusicName(path + L"\\" + fileinfo.cFileName);				// �ݹ�����ļ���
				}
			}
		} while (FindNextFile(hFind, &fileinfo));
	}
	FindClose(hFind);
}

// �� filePath д���ļ� filePath.ini
void MusicData::wFilePath()
{
	// �� .\\filePath.ini ��д�� �� [filePath] ��=ֵ path=filePath
	WritePrivateProfileString(L"filePath", L"path", filePath.c_str(), L".\\filePath.ini");
}

// ���ļ� filePath.ini ��ȡ�� filePath
void MusicData::rFilePath()
{
	// ��ȡ��Ӧ�ڣ�����ֵ
	array<wchar_t, 256> wptr{ L'\0' };
	const DWORD d = GetPrivateProfileString(L"filePath", L"path", L".",
		&wptr.at(0), 256, L".\\filePath.ini");

	filePath = &wptr.at(0);
}

// �� musicPathName д���ļ� music.mn ��
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
		std::wcout << L"�ļ� music.mn д�����" << std::endl;
#endif // _DEBUG
	}
#ifdef _DEBUG
	else
	{
		std::wcout << L"musicPathName Ϊ��" << std::endl;
	}
#endif // _DEBUG
}

// �� music.mn ��ȡ�� musicPathName �� musicName ��
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
			continue;															// ��Ϊ�գ�����
		}
		if (s.back() == L'\n')
		{
			s.pop_back();														// ȥ��ĩβ�Ļ��з�
			if (s.empty())
			{
				continue;
			}
		}
		musicPathName.push_back(s);												// д������ȫ·����
		const auto pos = s.rfind(L"\\");
		s = s.substr(pos + 1, s.size() - pos - 1 - musicFormat.size() - 1);		// ��ȡ��������
		musicName.push_back(s);													// д��������
	}
#ifdef _DEBUG
	std::wcout << L"�ļ� musci.mn ��ȡ���" << std::endl;
	if (musicPathName.empty())
	{
		std::wcout << L"musicPathName Ϊ��" << std::endl;
	}
#endif // _DEBUG
}


/****************************************************************
 *  MusicData public method										*
 ****************************************************************/

 // Ĭ�Ϲ��캯��
MusicData::MusicData()
{
	srand(time(nullptr) & 0xffffffff);

	AllocConsole();														// ��ӿ���̨
#ifdef _DEBUG
	freopen_s(&STD_COUT_STREAM, "CON", "w", stdout);					// �ض����׼�����
	std::wcout.imbue(std::locale("", LC_CTYPE));
#endif // _DEBUG


	FILE* fp = nullptr;													// �ļ�ָ��
	auto err = _wfopen_s(&fp, L"music.mn", L"r, ccs=UTF-16LE");			// �� ֻ����UTF-16LE �����ʽ���ļ�
	if (err == 0)														// �򿪳ɹ�
	{
		rFileMusic(fp);													// ���ļ����ڣ�ֱ�Ӷ�ȡ������
		fclose(fp);														// �ر��ļ�
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"�ļ� music.mn ��ʧ��" << std::endl;
		std::wcout << L"�����½��ļ� music.mn����������������" << std::endl;
#endif // _DEBUG

		err = _wfopen_s(&fp, L"music.mn", L"w, ccs=UTF-16LE");			// ������ UTF-16LE ������ļ�
		if (err == 0)													// �򿪳ɹ��������ɹ�
		{
#ifdef _DEBUG
			std::wcout << L"�ļ� music.mn �����ɹ�" << std::endl;
#endif // _DEBUG

			getFilePath();												// ��ȡ���ֵ�����·��

#ifdef _DEBUG
			std::wcout << L"��������·��Ϊ��" << filePath << std::endl;
			std::wcout << L"����������..." << std::endl;
#endif // _DEBUG

			findMusicName(filePath);

#ifdef _DEBUG
			std::wcout << L"�����������" << std::endl;
#endif // _DEBUG

			wFileMusic(fp);												// д��
			fclose(fp);													// �ر���
		}
	}
}

// ��������
MusicData::~MusicData()
{
	closeMusic();

#ifdef _DEBUG
	fclose(STD_COUT_STREAM);
#endif // _DEBUG
}