#include"musicPlayer.h"

int main()
{
	srand(time(nullptr) & 0xffffffff);
	MusicPlayer mp0;
	mp0.chooseMusicPlay();									// ѡ�񲢲�������
	do
	{
	} while (mp0.chooseFunction());
	return 0;
}