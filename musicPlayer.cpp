#include"musicPlayer.h"


//  MusiMusicMCI method
MusicMCI::MusicMCI()noexcept
{
	nDeviceID = -1;
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

// ��ȡ������ʱ����time��msΪ��λ
// �ɹ�����TRUE��ʧ�ܷ���FALSE
BOOL MusicMCI::getMusicTime(DWORD& time)noexcept
{
	MCI_STATUS_PARMS mciSP{};

	mciSP.dwItem = MCI_STATUS_LENGTH;
	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_STATUS,
		MCI_WAIT | MCI_STATUS_ITEM, (DWORD)static_cast<LPVOID>(&mciSP));		// �ؼ�,ȡ�ó���
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
	getFilePath();																// ��ʼ���������ļ�·��

	cout << "�ļ�����·��Ϊ��"<<filePath << endl;
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
		findMusicName(filePath);
		wFile();
	}

	rFile();
	file.close();
	nowMusicName = "";

	if (!musicPathName.empty())
		musicPathName.pop_back();																		// ɾ�����һ���հ���

	if (!musicName.empty())
		musicName.pop_back();
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
					findMusicName(p.assign(path).append("\\").append(&fileinfo.name[0]));				// �ݹ�����ļ���
				}
			}
			else
			{
				string suffix(&fileinfo.name[0]);
				if (suffix.size() > musicFormat.size())
					suffix = suffix.substr(suffix.size() - musicFormat.size());
				if (suffix == musicFormat)
				{
					musicPathName.push_back(p.assign(path).append("\\").append(&fileinfo.name[0]));				// �Ǹĸ����ļ��������ļ���

					string temp(&fileinfo.name[0]);
					musicName.push_back(temp.substr(0, temp.size() - musicFormat.size() - 1));					// ��ȡ��������
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void MusicPlayer::getFilePath()
{
	fstream fPath("filePath.ad", ios_base::in);
	string temp;
	if (!fPath.is_open())
	{
		cerr << "�ļ�filePath.ad��ʧ�ܣ����ļ����ܲ����ڣ�" << endl;
		cerr << "���򽫳����½����ļ�!" << endl;

		fPath.open("filePath.ad", ios_base::in | ios_base::out | ios_base::trunc);
		if (!fPath.is_open())
		{
			cerr << "�ļ�filePath.ad����ʧ�ܣ����������������һЩ���⣡" << endl;
			cerr << "���Ժ����ԣ�" << endl;
		}
		else
		{
			cout << "�ļ�filePath.ad�����ɹ���" << endl;
		}
	}
	else
	{
		getline(fPath, temp);
		if (temp.empty() || temp.at(0) == '\n')
		{
			cout << "�ļ�����Ϊ�գ�" << endl;
			cout << "�ļ�·������ΪĬ�ϣ�" << endl;
		}
		else
			filePath = std::move(temp);
	}
}

void MusicPlayer::setFilePath()
{
	string temp;
	cout << "��ǰ����·��Ϊ��" << filePath << endl;
	cout << "�������µ�����·��(���������·����)��";
	cin >> temp;
	cout << "�µ�����·��Ϊ��" << temp << endl;
	cout << "�Ƿ��޸ģ���y/n��:";
	char ch = 0;
	cin >> ch;
	if ('y' == ch || 'Y' == ch)
	{
		filePath = temp;
		fstream fp("filePath.ad", ios_base::out);
		if (!fp.is_open())
		{
			cerr << "�ļ�filePath.ad��ʧ�ܣ����Ժ����ԣ�" << endl;
		}
		else
		{
			fp << filePath << endl;
			fp.close();
			cout << "����·��д����ϣ�" << endl;

			const size_t tnum = musicPathName.size();
			findMusicName(filePath);							// Ѱ�������ļ�
			file.open("music.mn", ios_base::out | ios_base::app);
			if (!file.is_open())
			{
				cerr << "�ļ�music.mn��ʧ�ܣ����Ժ����Դ˹��ܣ�" << endl;
				Sleep(1000);
			}
			else
			{
				for (size_t i = tnum; i < musicPathName.size(); ++i)
					file << musicPathName.at(i) << endl;

				cout << "д���¸������ɹ���" << endl;
			}
			cout << "�����������·���µĸ�����" << endl;
		}
	}
	else
	{
		cout << "��ȡ���޸ģ�" << endl;
	}
}

wstring MusicPlayer::stringTowstring(const string& str)
{
	wstring result;
	// ��ȡ��������С��������ռ䣬��������С���ַ�����
	const int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), nullptr, 0);
	vector<TCHAR> buffer(len + 1);
	// ���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), &buffer.at(0), len);
	buffer.at(len) = '\0';														// ����ַ�����β
	result.append(&buffer.at(0));
	return result;
}

void MusicPlayer::pos(short x, short y)noexcept									// ���ù��λ��
{
	COORD pos{ x,y };
	HANDLE hOutput = nullptr;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

void MusicPlayer::openMusic(size_t num)
{
	if (num >= musicPathName.size())
	{
		cout << "����������Ҫ�������ԣ�" << endl;
		return;
	}

	wstring musci_name = stringTowstring(musicPathName.at(num));
	if (musicMci.open(musci_name.c_str()))
	{
		nowMusicName.assign(musicName.at(num));				// �������ڲ��ŵ�����
		cout << "���ִ򿪳ɹ���" << endl;

		playMusic();
		setVolumeMusic(vole);				
	}
	else
		cout << "���ִ�ʧ�ܣ����Ժ����ԣ�" << endl;

	cout << musicPathName.at(num) << endl;
}

void MusicPlayer::playMusic()
{
	if (musicMci.play())
	{
		cout << "����" << nowMusicName << "���ųɹ���" << endl;
		status = 1;
	}
	else
		cout << "����" << nowMusicName << "����ʧ�ܣ�" << endl;
}

void MusicPlayer::pauseMusic()
{
	if (musicMci.pause())
	{
		cout << "��������ͣ��" << endl;
		status = 2;
	}
	else
		cout << "������ͣʧ�ܣ�" << endl;
}

void MusicPlayer::stopMusic()
{
	if (musicMci.stop())
	{
		cout << "������ֹͣ��" << endl;
		status = 0;
	}
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

void MusicPlayer::setVolumeMusic(size_t volume)
{
	if (musicMci.setVolume(volume))
	{
		cout << "�������óɹ���" << endl;
		vole = volume;
	}
	else
		cout << "��������ʧ�ܣ�" << endl;
}

int MusicPlayer::getPlayerBackTimeMusic()
{
	DWORD playTime = 0;
	if (!musicMci.getPlayBackTime(playTime))
	{
		cout << "��ȡ����ʱ��ʧ�ܣ�" << endl;
		return 0;
	}
	return playTime / 1000;
}

int MusicPlayer::getTotalTime()
{
	DWORD totalTime = 0;
	if (!musicMci.getMusicTime(totalTime))
	{
		cout << "��ȡ��ʱ��ʧ�ܣ�" << endl;
		return 0;
	}
	return totalTime / 1000;
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

		musicName.push_back(s.substr(s.rfind("\\") + 1,	s.size() - s.rfind("\\") - 1 - musicFormat.size() - 1));									// ��ȡ��������			// ��ȡ��������

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

	size_t i = 0;
	for (auto& x : musicName)
		cout << i++ << "  " << x << endl;
}

void MusicPlayer::chooseMusicPlay()
{
	size_t choose = 0;
	showMusicName();
	cout << "��ѡ�����벥�ŵĸ�����";
	cin >> choose;
	if (choose <= (musicName.size() - 1))
	{
		number = choose;
		stopMusic();
		closeMusic();
		openMusic(choose);
	}
	else
	{
		cout << "ѡ�����" << endl;
		Sleep(1000);
	}
	system("cls");
}

void MusicPlayer::setPlayMode()
{
	size_t cho = 0;
	cout << "0.����ѭ��" << endl;
	cout << "1.˳�򲥷�" << endl;
	cout << "2.�������" << endl;
	cout << "��ǰ����ģʽ��";
	if (mode == 0)
		cout << "����ѭ��" << endl;
	else if (mode == 1)
		cout << "˳�򲥷�" << endl;
	else if (mode == 2)
		cout << "�������" << endl;

	cout << "��ѡ����Ҫѡ��Ĳ���ģʽ��";
	cin >> cho;
	if (cho <= 2)
		mode = cho;
}

int MusicPlayer::chooseFunction()
{
	while (!status)
	{
		system("cls");
		cout << "���ֲ���ʧ�ܣ������½���ѡ��" << endl;
		chooseMusicPlay();
	}

	size_t n = 10;
	system("cls");
	do
	{
		showPlayTime();

		pos(0, 0);
		cout << "0.�˳�" << endl;
		cout << "1.��ͣ" << endl;
		cout << "2.����" << endl;
		cout << "3.��������" << endl;
		cout << "4.ѡ�����ֲ���" << endl;
		cout << "5.���ò���ģʽ" << endl;
		cout << "6.��������·��" << endl;
		cout << "����������ѡ��Ĺ��ܣ�";
	} while (!_kbhit());
	cin >> n;
	switch (n)
	{
	case 0:cout << "���˳����ţ�" << endl; Sleep(1000); return 0;
	case 1:pauseMusic(); break;
	case 2:playMusic(); break;
	case 3: {size_t vol = 0;
		cout << "��ǰ������" << vole << endl;
		cout << "����������ֵ��0-1000����";
		cin >> vol;
		if (vol <= 1000)
			setVolumeMusic(vol); break; }
	case 4:chooseMusicPlay(); break;
	case 5:setPlayMode(); break;
	case 6:setFilePath(); break;
	default:cout << "ѡ�����" << endl; break;
	}
	return 1;
}

void MusicPlayer::showPlayTime()
{
	const int t = getPlayerBackTimeMusic();
	const int t0 = getTotalTime();
	if (t == t0)
	{
		status = 3;
		Sleep(1000);
		if (mode == 1)
		{
			openMusic((number + 1) >= musicName.size() ? 0 : (number + 1));
			system("cls");
		}
		else if (mode == 2)
		{
			openMusic(rand() % musicName.size());
			system("cls");
		}
	}
	static int x = 120 / 2;
	if (status == 1)
		x = --x < 0 ? 120 : --x;
	pos(0, 27);
	cout << "                                                                                                                            ";
	pos(x, 27);
	if (status == 1)
		cout << "���ڲ��ţ�" << nowMusicName << endl;
	else if (status == 2)
		cout << "����ͣ���ţ�" << nowMusicName << endl;
	else if (status == 3)
		cout << "���Ž�����" << nowMusicName << endl;
	pos(0, 28);
	cout << "                                                                                                                            ";
	pos(0, 28);
	cout << "�Ѳ��ţ�" << t / 60 << "��" << t % 60 << "��"
		<< "		" << "��ʱ����" << t0 / 60 << "��"
		<< t0 % 60 << "��" << "		" << "������" << vole
		<< "		" << "����ģʽ��";
	if (mode == 0)
		cout << "����ѭ��" << endl;
	else if (mode == 1)
		cout << "˳�򲥷�" << endl;
	else if (mode == 2)
		cout << "�������" << endl;

	Sleep(500);
}

MusicPlayer::~MusicPlayer()
{
	closeMusic();
}