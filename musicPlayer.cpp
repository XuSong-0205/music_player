#include"musicPlayer.h"


//  MusiMusicMCI method
MusicMCI::MusicMCI(void)noexcept
{
	nDeviceID = -1;
}

MusicMCI::~MusicMCI(void)noexcept
{
}

// ���ļ�
// �ɹ����ط���ֵ��ʧ�ܷ���0
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

// ����
// �ɹ����ط���ֵ��ʧ�ܷ���0
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

// ��ͣ����
// �ɹ����ط���ֵ��ʧ�ܷ���0
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

// ֹͣ���Ų�ʹ���ȷ��ص���ͷ
// �ɹ����ط���ֵ��ʧ�ܷ���0
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

// �ر�MCI�豸
// �ɹ����ط���ֵ��ʧ�ܷ���0
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

// ��õ�ǰ���Ž��ȣ�pos��msΪ��λ
// �ɹ����ط���ֵ��ʧ�ܷ���0
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

// �����趨������ֵ��Χ��0��1000
// �ɹ����ط���ֵ��ʧ�ܷ���0
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
		cerr << "�ļ�music.mn��ʧ�ܣ����ļ����ܲ�����" << endl;
		cerr << "���򽫳����½����ļ�������ʼ���ļ�����" << endl;

		file.open("music.mn", ios_base::out);
		if (!file.is_open())
		{
			cerr << "�ļ�music.mn����ʧ�ܣ�" << endl;
			cerr << "�������������һЩ���⣡" << endl;
			cerr << "���򼴽��˳���" << endl;
			system("pause");
			exit(0);
		}
		
		cout << "�ļ�music.mn�����ɹ���" << endl;
		this->findMusicName(filePath);
		this->wFile();
	}

	this->rFile();
	file.close();
	nowMusicName = "";

	if (!musicPathName.empty())
		musicPathName.pop_back();																		// ɾ�����һ���հ���

	for (const auto& x : musicPathName)
		musicName.push_back(x.substr(x.rfind("\\") + 1,
			x.size() - x.rfind("\\") - 1 - musicFormat.size() - 1));									// ��ȡ��������
}


// ��ȡ�ض���ʽ���ļ���    
void MusicPlayer::findMusicName(const string& path)
{
	long  hFile = 0;																					// �ļ����  64λ��long ��Ϊ intptr_t
	struct _finddata_t fileinfo;																		//	�ļ���Ϣ 
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)					// ���������ļ�
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))															// �ж��Ƿ�Ϊ�ļ���
			{
				if (strcmp(&fileinfo.name[0], ".") != 0 && strcmp(&fileinfo.name[0], "..") != 0)		// �ļ������в���"."��".."
				{
					this->findMusicName(p.assign(path).append("\\").append(&fileinfo.name[0]));			// �ݹ�����ļ���
				}
			}
			else
			{
				string suffix(&fileinfo.name[0]);
				if (suffix.size() > musicFormat.size())
					suffix = suffix.substr(suffix.size() - musicFormat.size());
				if (suffix == musicFormat)
					musicPathName.push_back(p.assign(path).append("\\").append(&fileinfo.name[0]));				// ��������ļ��У������ļ���
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

wstring MusicPlayer::stringTowstring(const string& str)
{
	wstring result;
	// ��ȡ��������С��������ռ䣬��������С���ַ�����
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), nullptr, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	// ���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';				// ����ַ�����β
	// ɾ��������������ֵ
	result.append(buffer);
	delete[] buffer;
	return result;
}

void MusicPlayer::playMusic(int num)
{
	if (num > musicPathName.size())
	{
		cout << "����������Ҫ�������ԣ�" << endl;
		return;
	}

	wstring musci_name = stringTowstring(musicPathName.at(num));
	if (musicMci.open(musci_name.c_str()))
	{
		nowMusicName.assign(musicName.at(num));				// �������ڲ��ŵ�����
		cout << "���ִ򿪳ɹ���" << endl;
		if (musicMci.play())
			cout << "����" << nowMusicName << "���ųɹ���" << endl;
		else
			cout << "����" << nowMusicName << "����ʧ�ܣ�" << endl;
	}
	else
		cout << "���ִ�ʧ�ܣ����Ժ����ԣ�" << endl;

	cout << musicPathName.at(num) << endl;
}

void MusicPlayer::pauseMusic()
{
	if (musicMci.pause())
		cout << "��������ͣ��" << endl;
	else
		cout << "������ͣʧ�ܣ�" << endl;
}

void MusicPlayer::stopMusic()
{
	if (musicMci.stop())
		cout << "������ֹͣ��" << endl;
	else
		cout << "����ֹͣʧ�ܣ�" << endl;
}

void MusicPlayer::closeMusic()
{
	if (musicMci.close())
		cout << "�����ѹرգ�" << endl;
	else
		cout << "���ֹر�ʧ�ܣ�" << endl;
}

void MusicPlayer::setVolumeMusic(int volume)
{
	if (musicMci.setVolume(volume))
		cout << "�������óɹ���" << endl;
	else
		cout << "��������ʧ�ܣ�" << endl;
}

int MusicPlayer::getPlayerBackTimeMusic()
{
	DWORD playTime = 0;
	if (musicMci.getPlayBackTime(playTime))
		cout << "��ȡ����ʱ���ɹ���" << endl;
	else
		cout << "��ȡ����ʱ��ʧ�ܣ�" << endl;

	return playTime / 1000;
}

void MusicPlayer::wFile()
{
	if (!musicPathName.empty())
		for (auto& x : musicPathName)
			file << x << endl;
	cout << "�ļ�music.mnд����ϣ�" << endl;
}

void MusicPlayer::rFile()
{
	string s;
	do
	{
		getline(file, s);
		musicPathName.push_back(s);
	} while (!file.eof());
	cout << "�ļ�musci.mn��ȡ��ϣ�" << endl;
}

void MusicPlayer::showMusicName()
{
	if (musicName.empty())
	{
		cout << "�޲����б�" << endl;
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
	cout << "��ѡ�����벥�ŵĸ�����";
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

		cout << "0.�˳�����" << endl;
		cout << "1.��ͣ����" << endl;
		cout << "2.��������" << endl;
		cout << "3.ѡ���������ֲ���" << endl;
		cout << "����������ѡ��Ĺ��ܣ�";
		Sleep(500);
		system("cls");
	} while (!_kbhit());
	n = _getch() - '0';
	switch (n)
	{
	case 0:cout << "���˳����ţ�" << endl; Sleep(1000); exit(0);
	case 1:this->pauseMusic(); break;
	case 2: {int vol = 300; cout << "����������ֵ��0-1000����";
		cin >> vol; this->setVolumeMusic(vol); break; }
	case 3:this->chooseMusicPlay(); break;
	default:cout << "ѡ�����" << endl; break;
	}
}

void MusicPlayer::showPlayTime()
{
	//static time_t start = time(nullptr);
	const int t = this->getPlayerBackTimeMusic();
	cout << "���ڲ��ţ�" << nowMusicName << endl;
	cout << "�Ѳ��ţ�" << t / 60 << "��" << t % 60 << "��" << endl;
	cout << "����������й���ѡ��" << endl;
}

MusicPlayer::~MusicPlayer()
{
	this->closeMusic();
}