#include"musicPlayer.h"


// CMCI method
CMCI::CMCI(void)
{
	nDeviceID = -1;
}

CMCI::~CMCI(void)
{
}

// ���ļ�
// �ɹ����ط���ֵ��ʧ�ܷ���0
BOOL CMCI::Open(LPCWSTR strSongPath)noexcept
{
	MCI_OPEN_PARMS mciOP;

	mciOP.lpstrDeviceType = nullptr;
	mciOP.lpstrElementName = strSongPath;
	const DWORD dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE, (DWORD)(LPVOID)&mciOP);
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
BOOL CMCI::Play()
{
	MCI_PLAY_PARMS mciPP{};

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPP);
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// ��ͣ����
// �ɹ����ط���ֵ��ʧ�ܷ���0
BOOL CMCI::Pause()
{
	MCI_GENERIC_PARMS mciGP;

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_PAUSE, MCI_NOTIFY | MCI_WAIT, (DWORD)(LPVOID)&mciGP);
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// ֹͣ���Ų�ʹ���ȷ��ص���ͷ
// �ɹ����ط���ֵ��ʧ�ܷ���0
BOOL CMCI::Stop()
{
	MCI_SEEK_PARMS mciSP;

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_SEEK, MCI_WAIT | MCI_NOTIFY | MCI_SEEK_TO_START, (DWORD)(LPVOID)&mciSP);
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// �ر�MCI�豸
// �ɹ����ط���ֵ��ʧ�ܷ���0
BOOL CMCI::Close()
{
	MCI_GENERIC_PARMS mciGP;

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_CLOSE, MCI_NOTIFY | MCI_WAIT, (DWORD)(LPVOID)&mciGP);
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// ��õ�ǰ���Ž��ȣ�pos��msΪ��λ
// �ɹ����ط���ֵ��ʧ�ܷ���0
BOOL CMCI::GetCurrentPos(DWORD& pos)
{
	MCI_STATUS_PARMS mciSP;

	mciSP.dwItem = MCI_STATUS_POSITION;
	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciSP);
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
BOOL CMCI::SetVolume(int nVolumeValue)
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
	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&mciDSP);
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}