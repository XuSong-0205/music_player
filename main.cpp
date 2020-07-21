#include"GuiMusicPlayer.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	GuiMusicPlayer& gmp = GuiMusicPlayer::singleton();
	gmp.run();
	return 0;
}