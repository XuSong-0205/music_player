#pragma once
#include "MusicMCI.h"


/************************************************
 *              class MusicData					*
 * ���ݲ�����									*
 * ����Ӧ�����ݽ��в���							*
 * ����MusicMCI��GuiMusicPlayer���н���			*
 ************************************************/

class MusicData
{
private:
	MusicMCI musicMci;										// ִ�����ֲ��Ų����Ķ���
	wstring filePath;										// Ҫ���ҵ��ļ�·��
	wstring musicFormat = L"mp3";							// Ҫ���ҵ��ļ���ʽ
	wstring nowMusicName;									// ���ڲ�����������
	vector<wstring> musicPathName;							// �洢ȫ·�������ļ���
	vector<wstring> musicName;								// �洢�����ļ���
	size_t status = 0;										// ����״̬ 0δ���� 1���ڲ��� 2��ͣ���� 3�������
	size_t volume = 400;									// ��ǰ������С
	size_t number = 0;										// ���ڲ��������ֱ��
	size_t mode = 2;										// ���ֲ���ģʽ 0����ѭ�� 1˳�򲥷�(�б�ѭ��) 2�������

	void openMusic(size_t num);								// �򿪲���������
	void playMusic();										// ��������
	void pauseMusic();										// ��ͣ����
	void stopMusic();										// ֹͣ����
	void closeMusic();										// �ر�����
	void setMusicVolume(size_t vol);						// ����������С
	bool setMusicStartTime(size_t start);					// ���ò���λ��
	int getMusicCurrentTime();								// ��ȡ���ֵĵ�ǰ����ʱ��
	int getMusicTotalTime();								// ��ȡ���ֵ���ʱ��

	void prevMusic();										// ��һ��
	void nextMusic();										// ��һ��

	void getFilePath();										// ��ȡ����·��
	void findMusicName(const wstring& path);				// Ѱ�������ļ�
	void wFilePath();										// �� filePath д���ļ� filePath.ini
	void rFilePath();										// ���ļ� filePath.ini ��ȡ�� filePath
	void wFileMusic(FILE* fp);								// �� musicPathName д���ļ� music.mn
	void rFileMusic(FILE* fp);								// ���ļ� music.mn ��ȡ�� musicPathName

public:
	MusicData();											// Ĭ�Ϲ��캯��
	~MusicData();											// ��������

	friend class GuiMusicPlayer;
};