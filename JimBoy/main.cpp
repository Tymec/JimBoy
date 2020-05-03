#include "JimBoy.h"

int main(int argc, char* argv[]) {
	JimBoy jimBoy;
	jimBoy.LoadBootRom("..//dmg_boot.bin");
	jimBoy.Start("..//Roms//Tetris.gb");
	return 0;
}