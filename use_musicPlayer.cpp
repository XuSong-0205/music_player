#include"musicPlayer.h"

int main()
{
	MusicPlayer mp0;
	mp0.showMusicName();									// 显示播放列表
	mp0.playMusic(mp0.chooseMusicPlay());					// 选择并播放音乐

	system("pause");
	return 0;
}