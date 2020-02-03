#include"musicPlayer.h"


// CMCI method
CMCI::CMCI(void)
{
	nDeviceID = -1;
}

CMCI::~CMCI(void)
{
}

// 打开文件
// 成功返回非零值，失败返回0
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

// 播放
// 成功返回非零值，失败返回0
BOOL CMCI::Play()
{
	MCI_PLAY_PARMS mciPP{};

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPP);
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// 暂停播放
// 成功返回非零值，失败返回0
BOOL CMCI::Pause()
{
	MCI_GENERIC_PARMS mciGP;

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_PAUSE, MCI_NOTIFY | MCI_WAIT, (DWORD)(LPVOID)&mciGP);
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// 停止播放并使进度返回到开头
// 成功返回非零值，失败返回0
BOOL CMCI::Stop()
{
	MCI_SEEK_PARMS mciSP;

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_SEEK, MCI_WAIT | MCI_NOTIFY | MCI_SEEK_TO_START, (DWORD)(LPVOID)&mciSP);
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// 关闭MCI设备
// 成功返回非零值，失败返回0
BOOL CMCI::Close()
{
	MCI_GENERIC_PARMS mciGP;

	const DWORD dwReturn = mciSendCommand(nDeviceID, MCI_CLOSE, MCI_NOTIFY | MCI_WAIT, (DWORD)(LPVOID)&mciGP);
	if (dwReturn == 0)
		return TRUE;
	else
		return FALSE;
}

// 获得当前播放进度，pos以ms为单位
// 成功返回非零值，失败返回0
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

// 音量设定，音量值范围在0到1000
// 成功返回非零值，失败返回0
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