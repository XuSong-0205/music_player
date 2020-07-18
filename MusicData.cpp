#include "MusicData.h"


/**************************************************************************************************************
 *  class MusicData                                                                                           *
 **************************************************************************************************************/

/**************************************************************
 * MusicData private method                                   *
 **************************************************************/

// �򿪲���������
void MusicData::openMusic(size_t num)
{
	if (musicPathName.empty() || num > musicPathName.size() - 1)
	{
#ifdef DEBUG
		wcout << L"����������Ҫ�������ԣ�" << endl;
#endif // DEBUG

		status = 0;
		return;
	}

	wstring musci_name = stringTowstring(musicPathName.at(num));
	nowMusicName = musicName.at(num);								// �������ڲ�����������
	number = num;													// �������ڲ��������������

	if (musicMci.open(musci_name.c_str()))
	{
#ifdef DEBUG
		wcout << L"���� " << nowMusicName << L" �򿪳ɹ���" << endl;
#endif // DEBUG

		playMusic();
		musicMci.setVolume(volume);
	}
	else
	{
#ifdef DEBUG
		wcout << L"���� " << nowMusicName << L" ��ʧ�ܣ�" << endl;
#endif // DEBUG
	}
}

// ��������
void MusicData::playMusic()
{
	if (musicMci.play())
	{
#ifdef DEBUG
		wcout << L"���� " << nowMusicName << L" ���ųɹ���" << endl;
#endif // DEBUG

		status = 1;
	}
	else
	{
#ifdef DEBUG
		wcout << L"���� " << nowMusicName << L" ����ʧ�ܣ�" << endl;
#endif // DEBUG

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
#ifdef DEBUG
			wcout << L"���� " << nowMusicName << L" ����ͣ��" << endl;
#endif // DEBUG

			status = 2;
		}
		else
		{
#ifdef DEBUG
			wcout << L"���� " << nowMusicName << L" ��ͣʧ�ܣ�" << endl;
#endif // DEBUG
		}
	}
}

// ֹͣ����
void MusicData::stopMusic()
{
	if (musicMci.stop())
	{
#ifdef DEBUG
		wcout << L"���� " << nowMusicName << L" ����ͣ��" << endl;
#endif // DEBUG

		status = 0;
	}
	else
	{
#ifdef DEBUG
		wcout << L"���� " << nowMusicName << L" ��ͣʧ�ܣ�" << endl;
#endif // DEBUG
	}
}

// �ر�����
void MusicData::closeMusic()
{
	if (musicMci.close())
	{
#ifdef DEBUG
		wcout << L"���� " << nowMusicName << L" �ѹرգ�" << endl;
#endif // DEBUG

		status = 0;
	}
	else
	{
#ifdef DEBUG
		wcout << L"���� " << nowMusicName << L" �ر�ʧ�ܣ�" << endl;
#endif // DEBUG
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
#ifdef DEBUG
		else
		{
			wcout << L"��������ʧ�ܣ�" << endl;
		}
#endif // DEBUG
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
#ifdef DEBUG
			wcout << L"���ò���λ��ʧ�ܣ�" << endl;
#endif // DEBUG
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
#ifdef DEBUG
			wcout << L"��ȡ����ʱ��ʧ�ܣ�" << endl;
#endif // DEBUG

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
#ifdef DEBUG
			wcout << L"��ȡ��ʱ��ʧ�ܣ�" << endl;
#endif // DEBUG

			return 0;
		}
		return totalTime / 1000;
	}

	return 0;
}


// ���ļ� filePath.ini �ж�ȡ����·��
void MusicData::getFilePath()
{
	fstream fPath("filePath.ini", ios_base::in);
	if (!fPath.is_open())
	{
#ifdef DEBUG
		wcout << L"�ļ� filePath.ini ��ʧ�ܣ�" << endl;
		wcerr << L"���򽫳����½����ļ�!" << endl;
#endif // DEBUG

		fPath.open("filePath.ini", ios_base::in | ios_base::out | ios_base::trunc);
		if (fPath.is_open())
		{
#ifdef DEBUG
			wcout << L"�ļ� filePath.ini �����ɹ���" << endl;
#endif // DEBUG

			filePath = "..";								// ����Ĭ��Ϊ��ǰ·��
			wFilePath(fPath);								// д��
		}
	}
	else
	{
		rFilePath(fPath);									// ��ȡ
	}
}

// ��ȡ�ض���ʽ���ļ���    
void MusicData::findMusicName(const string& path)
{
	_finddata_t fileinfo;															// �ļ���Ϣ
	auto hFile = _findfirst((path + "\\*." + musicFormat).c_str(), &fileinfo);		// ���ҵ�ǰ�ļ����Ƿ����ָ����ʽ�ļ�
	if (hFile != -1)																// ������ļ����´���ָ����ʽ�ļ�
	{
		do
		{
			string str = fileinfo.name;
			musicPathName.push_back(path + "\\" + str);								// д������ȫ·����
			musicName.push_back(stringTowstring(str.substr(0, str.size() - musicFormat.size() - 1)));	// д��������
		} while (_findnext(hFile, &fileinfo) == 0);
	}

	hFile = _findfirst((path + "\\*").c_str(), &fileinfo);							// �������ļ��У����������ļ�
	if (hFile != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))										// �ж��Ƿ�Ϊ�ļ���
			{
				if (fileinfo.name[0] != '.')										// �ļ������в���"."��".."
				{
					findMusicName(path + "\\" + fileinfo.name);						// �ݹ�����ļ���
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
}

// �� filePath д���ļ� filePath.ini
void MusicData::wFilePath(fstream& file)
{
	file << filePath;
}

// ���ļ� filePath.ini ��ȡ�� filePath
void MusicData::rFilePath(fstream& file)
{
	string temp;
	getline(file, temp);
	if (temp.empty())
	{
#ifdef DEBUG
		wcout << L"�ļ�����Ϊ�գ�" << endl;
		wcout << L"�ļ�·������ΪĬ�ϣ�" << endl;
#endif // DEBUG

		filePath = "..";							// Ĭ��Ϊ��ǰ·��
		wFilePath(file);							// д��
	}
	else
	{
		filePath = temp;
	}
}

// �� musicPathName д���ļ� music.mn ��
void MusicData::wFileMusic(fstream& file)
{
	if (!musicPathName.empty())
	{
		const int n = musicPathName.size();
		file << musicPathName.at(0);
		for (int i = 1; i < n; ++i)
			file << "\n" << musicPathName.at(i);
#ifdef DEBUG
		wcout << L"�ļ� music.mn д����ϣ�" << endl;
#endif // DEBUG
	}
#ifdef DEBUG
	else
	{
		wcout << L"musicPathName Ϊ�գ�" << endl;
	}
#endif // DEBUG
}

// �� music.mn ��ȡ�� musicPathName �� musicName ��
void MusicData::rFileMusic(fstream& file)
{
	string s;
	if (file.eof())
	{
#ifdef DEBUG
		wcout << L"�ļ� music.mn Ϊ�գ�" << endl;
#endif // DEBUG
	}

	while (!file.eof())
	{
		getline(file, s);
		musicPathName.push_back(s);												// д������ȫ·����
		const auto pos = s.rfind("\\");
		s = s.substr(pos + 1, s.size() - pos - 1 - musicFormat.size() - 1);		// ��ȡ��������
		musicName.push_back(stringTowstring(s));								// д��������
	}
#ifdef DEBUG
	wcout << L"�ļ� musci.mn ��ȡ��ϣ�" << endl;
#endif // DEBUG
}


/**************************************************************
 *  MusicData public method                                   *
 **************************************************************/

// Ĭ�Ϲ��캯��
MusicData::MusicData()
{
	srand(time(nullptr) & 0xffffffff);
	getFilePath();																// ��ʼ���������ļ�·��

#ifdef DEBUG
	wcout.imbue(std::locale("", LC_CTYPE));
	wcout << L"�ļ�����·��Ϊ��" << stringTowstring(filePath) << endl;
#endif // DEBUG

	fstream file("music.mn", ios_base::in);
	if (!file.is_open())
	{
#ifdef DEBUG
		wcerr << L"�ļ� music.mn ��ʧ�ܣ����ļ����ܲ�����" << endl;
		wcerr << L"���򽫳����½����ļ�������ʼ���ļ�����" << endl;
#endif // DEBUG

		file.open("music.mn", ios_base::out);			// �½��ļ�
		if (file.is_open())
		{
#ifdef DEBUG
			wcout << L"�ļ� music.mn �����ɹ���" << endl;
			wcout << L"��������������..." << endl;
#endif // DEBUG

			findMusicName(filePath);

#ifdef DEBUG
			wcout << L"����������ϣ�" << endl;
#endif // DEBUG

			wFileMusic(file);
		}
	}
	else
	{
		rFileMusic(file);								// ���ļ����ڣ�ֱ�Ӷ�ȡ������
	}
	file.close();										// �ر��ļ�

	if (!musicPathName.empty() && musicPathName.back() == "")
		musicPathName.pop_back();						// ɾ�����һ���հ���

	if (!musicName.empty() && musicName.back() == L"")
		musicName.pop_back();
}

// ��������
MusicData::~MusicData()
{
	closeMusic();
}


// �ǳ�Ա����
// stringתwstring
wstring stringTowstring(const string& str)
{
	wstring result;
	// ��ȡ��������С��������ռ䣬��������С���ַ�����
	const size_t len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), nullptr, 0);
	vector<TCHAR> buffer(len + 1u);
	// ���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), &buffer.at(0), len);
	buffer.at(len) = '\0';			// ����ַ�����β
	result.append(&buffer.at(0));
	return result;
}