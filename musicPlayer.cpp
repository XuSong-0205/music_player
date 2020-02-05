#include"musicPlayer.h"

/**************************************************************************************************************
*  MusiMusicMCI method                                                                                        *
***************************************************************************************************************/

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
* MusicPlayer method                                                                                          *
**************************************************************************************************************/

/**************************************************************
* MusicPalyer private method                                  *
**************************************************************/

// �򿪲���������
void MusicPlayer::openMusic(size_t num)
{
	if (num >= musicPathName.size())
	{
		cout << "����������Ҫ�������ԣ�" << endl;
		status = 0;
		return;
	}

	wstring musci_name = stringTowstring(musicPathName.at(num));
	nowMusicName.assign(musicName.at(num));							// �������ڲ�����������
	number = num;													// �������ڲ��������������
	if (musicMci.open(musci_name.c_str()))
	{
		cout << "���ִ򿪳ɹ���" << endl;

		playMusic();
		setMusicVolume(vole);
	}
	else
		cout << "���ִ�ʧ�ܣ����Ժ����ԣ�" << endl;
}

// ��������
void MusicPlayer::playMusic()
{
	if (musicMci.play())
	{
		cout << "����" << nowMusicName << "���ųɹ���" << endl;
		status = 1;
	}
	else
	{
		cout << "����" << nowMusicName << "����ʧ�ܣ�" << endl;
		status = 0;
	}
}

// ��ͣ����
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

// ֹͣ����
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

// �ر�����
void MusicPlayer::closeMusic()
{
	if (musicMci.close())
	{
		cout << "�����ѹرգ�" << endl;
		status = 0;
	}
	else
		cout << "���ֹر�ʧ�ܣ�" << endl;
}

// ��ȡ���ֵ�ǰ����ʱ��
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

// ��ȡ�����ܲ���ʱ��
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


// stringתwstring
wstring MusicPlayer::stringTowstring(const string& str)
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

// ���ù��λ��
void MusicPlayer::pos(short x, short y)noexcept
{
	COORD pos{ x,y };
	HANDLE hOutput = nullptr;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

// ���ļ�filePath.ad�ж�ȡ����·��
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

// ��musicPathNameд���ļ�music.mn��
void MusicPlayer::wFile(fstream& file)
{
	if (!musicPathName.empty())
		for (auto& x : musicPathName)
			file << x << endl;
	cout << "�ļ�music.mnд����ϣ�" << endl;
}

// ��music.mn��ȡ��musicPathName��musicName��
void MusicPlayer::rFile(fstream& file)
{
	string s;
	do
	{
		getline(file, s);
		musicPathName.push_back(s);

		musicName.push_back(s.substr(s.rfind("\\") + 1, s.size() - s.rfind("\\") - 1 - musicFormat.size() - 1));									// ��ȡ��������			// ��ȡ��������

	} while (!file.eof());
	cout << "�ļ�musci.mn��ȡ��ϣ�" << endl;
}

// ������������
void MusicPlayer::setMusicVolume(size_t volume)
{
	if (musicMci.setVolume(volume))
	{
		cout << "�������óɹ���" << endl;
		vole = volume;
	}
	else
		cout << "��������ʧ�ܣ�" << endl;
}

// ��������·����д��filePath.ad��
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
			fstream file("music.mn", ios_base::out | ios_base::app);
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

// ���ò���ģʽ
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

// ��ʾ���ֲ����б�
void MusicPlayer::showMusicName()
{
	if (musicName.empty())
	{
		cout << "�޲����б�" << endl;
		return;
	}

	size_t i = 0;
	cout << "�����б�" << endl;
	for (auto& x : musicName)
		cout << i++ << "  " << x << endl;
}

// ��ʾ������Ϣ
void MusicPlayer::showPlayInformation()
{
	const int t = getPlayerBackTimeMusic();
	const int t0 = getTotalTime();
	auto chonum = [](size_t a, size_t b)noexcept {return a > b ? 0 : ++a; };	// lamdom���ʽ
	if (t == t0)																// �����Ƿ����
	{
		status = 3;
		Sleep(1000);
		if (mode == 0)
		{
			stopMusic();
			closeMusic();
			openMusic(number);
			system("cls");
		}
		else if (mode == 1)
		{
			openMusic(chonum(number + 1, musicName.size()));
			system("cls");
			if (!status)
			{
				cout << "����" << nowMusicName << "����ʧ�ܣ�" << endl;
				cout << "��������һ�����֣�" << musicName.at(chonum(number + 1, musicName.size())) << endl;
				Sleep(2000);
				openMusic(chonum(number + 1, musicName.size()));
			}
			system("cls");
		}
		else if (mode == 2)
		{
			openMusic(rand() % musicName.size());
			system("cls");
			if (!status)
			{
				const size_t stemp = rand() % musicName.size();
				cout << "����" << nowMusicName << "����ʧ�ܣ�" << endl;
				cout << "��������һ�����֣�" << musicName.at(stemp) << endl;
				Sleep(2000);
				openMusic(stemp);
			}
			system("cls");
		}
	}
	static short x = 120 / 2;
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
}

// ɾ������
void MusicPlayer::deleteMusic()
{
	system("cls");
	showMusicName();
	cout << "����ʾ��������ɾ�����ֵ���Ų��س�����������ϣ���������ּ��ɣ���" << endl;
	cout << "����������ɾ�������֣�";
	vector<int> tvec;
	int it = 0;
	while (cin >> it)
	{
		tvec.push_back(it);
		cout << "����������ɾ�������֣�";
	}
	cin.clear();
	while (cin.get() != '\n');											// ������뻺����
	cout << "��Ҫɾ�����¸�����" << endl;
	for (auto x : tvec)
		cout << x << "  " << musicName.at(x) << endl;
	cout << "�Ƿ�ɾ��?��y/n��:";
	char ch = 0;
	cin >> ch;
	if (ch == 'y' || ch == 'Y')
	{
		cout << "ɾ�������У����Ժ�..." << endl;
		for (auto x : tvec)												// ɾ����Ӧ�±��������
		{
			musicName.erase(musicName.begin() + x);
			musicPathName.erase(musicPathName.begin() + x);
		}
		cout << "�����б�ɾ����ɣ�" << endl;
		cout << "�ļ�music.mn���ݸ�����..." << endl;

		fstream ft("music.mn", ios_base::out);
		if (!ft.is_open())
		{
			cerr << "�ļ�music.mn��ʧ���ˣ����Ժ����ԣ�" << endl;
		}
		else
		{
			for (auto& x : musicPathName)
				ft << x << endl;
			ft.close();
			cout << "�ļ�music.mn���ݸ�����ɣ�" << endl;
		}
	}
	else
	{
		cout << "��ȡ������ɾ����" << endl;
	}
	Sleep(2000);
}


/**************************************************************
*  MusicPlayer public method                                  *
**************************************************************/

// Ĭ�Ϲ��캯��
MusicPlayer::MusicPlayer()
{
	srand(time(nullptr) & 0xffffffff);
	getFilePath();																// ��ʼ���������ļ�·��

	cout << "�ļ�����·��Ϊ��" << filePath << endl;
	fstream file("music.mn", ios_base::in);
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
		wFile(file);
	}

	rFile(file);
	file.close();
	nowMusicName = "";

	if (!musicPathName.empty())
		musicPathName.pop_back();																		// ɾ�����һ���հ���

	if (!musicName.empty())
		musicName.pop_back();
}

// ��������
MusicPlayer::~MusicPlayer()
{
	closeMusic();
}

// ѡ�����ֲ���
void MusicPlayer::chooseMusicPlay()
{
	system("cls");
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

// ����ѡ��
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
		showPlayInformation();

		pos(0, 0);
		cout << "0.�˳�" << endl;
		cout << "1.��ͣ" << endl;
		cout << "2.����" << endl;
		cout << "3.��������" << endl;
		cout << "4.ɾ������" << endl;
		cout << "5.ѡ�����ֲ���" << endl;
		cout << "6.���ò���ģʽ" << endl;
		cout << "7.��������·��" << endl;
		cout << "����������ѡ��Ĺ��ܣ�";
		Sleep(500);
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
			setMusicVolume(vol); break; }
	case 4:deleteMusic(); break;
	case 5:chooseMusicPlay(); break;
	case 6:setPlayMode(); break;
	case 7:setFilePath(); break;
	default:cout << "ѡ�����" << endl; break;
	}
	return 1;
}