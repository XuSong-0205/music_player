#include"musicPlayer.h"

int main()
{
	MusicPlayer mp0;
	mp0.showMusicName();									// ��ʾ�����б�
	mp0.playMusic(mp0.chooseMusicPlay());					// ѡ�񲢲�������

	system("pause");
	return 0;
}