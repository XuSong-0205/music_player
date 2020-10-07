#pragma once
#include "MusicData.h"


/************************************************
 *              class GuiMusicPlayer			*
 * ������										*
 * ���û����н���								*
 * ��������������͸�MusicDate���д���			*
 ************************************************/

class GuiMusicPlayer
{
private:
	MusicData musicData;									// MusicData �Ķ���
	IMAGE backgroundImage;									// ����ͼƬ
	bool haveBackgroundImage;								// �Ƿ���ڱ���ͼ
	bool playList = false;									// �����б��Ƿ�չ��
	bool quit = false;										// �Ƿ��˳�
	int position = 0;										// ÿһλ��Ӧһ��������״̬
															// 0 - 4 λ��ʾ�������б�ľ��Ը���λ��
															// 5, 10 λ��������������
															// 11��12��13 λ���ζ�Ӧλ�� ��һ������ʼ����ͣ������һ��
															// 14��15 λ������������ģʽ
	array<int, 2> numRange;									// �����б���ʾ�����ַ�Χ numRange[0] Ϊ musicData.musicName
															// ��ʾ�������б�ʼ���±�
															// numRange[1] Ϊ�ڲ����б�Ľ����±�
	const int WIDTH = 960;									// ���ڿ��
	const int HEIGHT = 640;									// ���ڸ߶�

	GuiMusicPlayer();										// ˽�е�Ĭ�Ϲ��캯��
	bool findBackgroundPicture()noexcept;					// �����Ƿ���ڱ���ͼƬ

	void event();											// �����¼������
	void mouseEvent(const MOUSEMSG& mouse);					// ����¼�
	void keyBoardEvent(int key);							// �����¼�
	void timerEvent();										// ��ʱ���¼�
	void drawEvent();										// ��ͼ�¼�

	bool mouseQuitEvent(const MOUSEMSG&);					// ����˳��¼�
	bool mousePlayListArrowEvent(const MOUSEMSG&);			// ��겥���б��ͷ�¼�
	bool mousePlayListEvent(const MOUSEMSG&);				// ��겥���б��¼�
	bool mousePlayListScrollEvent(const MOUSEMSG&);			// ��겥���б�����¼�
	bool mousePlayListScrollBarEvent(const MOUSEMSG&);		// ��겥���б�������¼�
	bool mouseMusicProgressBarEvent(const MOUSEMSG&);		// ������ֽ������¼�
	bool mousePlayPauseButtonEvent(const MOUSEMSG&);		// ��겥����ͣ��ť�¼�
	bool mousePreviousMusicButtonEvent(const MOUSEMSG&);	// �����һ����ť�¼�
	bool mouseNextMusicButonEvent(const MOUSEMSG&);			// �����һ����ť�¼�
	bool mouseVolumeBarEvent(const MOUSEMSG&);				// ����������¼�
	bool mousePlayModeEvent(const MOUSEMSG&);				// ��겥��ģʽ�¼�

	bool keyboardSpaceEvent(int key);						// ���̿ո���¼�
	bool keyboardEnterEvent(int key);						// ���̻س����¼�
	bool keyboardUpDownEvent(int key);						// �������¼��¼�
	bool keyboardLeftRightEvent(int key);					// �������Ҽ��¼�

	void timerPlayMusicEvent();								// ��ʱ�����������¼�

	void drawUiEvent();										// ����̬ ui
	void drawPlayListArrowEvent();							// �������б��ͷ�¼�
	void drawPlayListEvent();								// �������б��¼�
	void drawPlayListScrollBarEvent();						// �������б�������¼�
	void drawMusicProgressBarEvent();						// �����ֽ������¼�
	void drawMusicNameEvent();								// ���������¼�
	void drawMusicTimeEvent();								// ������ʱ���¼�
	void drawPlayPauseButtonEvent();						// ��������ͣ��ť�¼�
	void drawVolumeBarEvent();								// ���������¼�
	void drawPlayModeEvent();								// ������ģʽ�¼�

public:
	GuiMusicPlayer(const GuiMusicPlayer&) = delete;			// ���ƶ���������
	GuiMusicPlayer(GuiMusicPlayer&&) = default;
	GuiMusicPlayer& operator=(const GuiMusicPlayer&) = delete;
	GuiMusicPlayer& operator=(GuiMusicPlayer&&) = default;
	~GuiMusicPlayer();

	static GuiMusicPlayer& singleton();						// ���ظ���Ψһ��������ã�����ģʽ
	void run();												// �ⲿ���õĽӿ�
};