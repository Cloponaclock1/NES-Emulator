#include "ppu.h"


uint8_t PPU::cpuRead(uint16_t addr, bool read) {
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
	return 1;
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
uint8_t PPU::ppuRead(uint16_t addr, bool read) {
	uint8_t data = 0x00;
	addr &= 0x3FFF;

	if (cart->ppuRead(addr, data)) {

	}
	return data;
}
void PPU::ppuWrite(uint16_t addr, uint8_t data) {
	uint8_t value = 0x00;

	if (cart->ppuWrite(addr, data)){
		return;
	}

	addr &= 0x3FFF;
	return;
}
void PPU::inputCart(const std::shared_ptr<Cartridge>& cart) {
	this->cart = cart;
}