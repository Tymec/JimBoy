#include "JimBoy.h"

int main(int argc, char* argv[]) {
	JimBoy jimBoy;
	//jimBoy.LoadBootRom("..//dmg_boot.bin");
	jimBoy.LoadBootRom("none");
	//jimBoy.Start("..//Roms//gb_test_roms//cpu_instrs//cpu_instrs.gb");
	jimBoy.Start("..//Roms//Dr. Mario.gb");
	return 0;
}