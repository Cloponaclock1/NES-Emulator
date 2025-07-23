#include "ppu.h"


uint8_t PPU::cpuRead(uint16_t addr) {
	switch (addr) {
	case 0: //PPUCTRL

		break;
	case 1: //PPUMASK

		break;
	case 2: //PPUSTATUS

		break;
	case 3: //OAMADDR

		break;
	case 4: // OAMDATA
		break;
	case 5: // PPUSCROLL
		break;
	case 6://PPUADDR
		break;
	case 7: //PPUDATA
		break;

	}
}

void PPU::cpuWrite(uint16_t addr, uint8_t data) {
	switch (addr) {
	case 0: //PPUCTRL

		break;
	case 1: //PPUMASK

		break;
	case 2: //PPUSTATUS

		break;
	case 3: //OAMADDR

		break;
	case 4: // OAMDATA
		break;
	case 5: // PPUSCROLL
		break;
	case 6://PPUADDR
		break;
	case 7: //PPUDATA
		break;

	}

}
uint8_t PPU::ppuRead(uint16_t addr) {
	addr &= 0b0000011111111111;
}
void PPU::ppuWrite(uint16_t addr, uint8_t data) {
	addr &= 0b0000011111111111;

}