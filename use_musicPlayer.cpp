#include"musicPlayer.h"

int main()
{
	GuiMusicPlayer& gmp = GuiMusicPlayer::singleton();
	gmp.run();
	return 0;
}