#include "Bus.h"

#include <string>
#include <fstream>
#include <iostream>





Bus::Bus() {

}

uint8_t Bus::cpuRead(uint16_t addr) {
	uint8_t value = 0x00;
	if (cart->cpuRead(addr,value)) {

	}

	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		return cpuram[addr & 0b0000011111111111]; // keeping the address within the 2Kb range. Cheaper than using modulus, 07ff
	}
	else if (addr >= 0x2000 <= 0x3FFF) {
		// repeats every 8 bytes so same as 0x07 or mod 8 
		return ppu->cpuRead(addr & 0x0007);

	}
}

void Bus::cpuWrite(uint16_t addr, uint8_t value) {
	//Ram
	if (cart->cpuWrite(addr, value)) {

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		cpuram[addr & 0x07FF] = value;
	}else if (addr >= 0x2000 && addr <= 0x3FFF) {
		ppu->cpuWrite(addr & 0x0007, value);
	}
}
void Bus::Clock() {
	cpu->clock();
	systemClockCounter += 1;
}
void Bus::Reset() {
	cpu->Reset();
	systemClockCounter = 0;
}

void Bus::inputCart(const::std::shared_ptr<Cartridge>& cartidge) {
	this->cart = cartidge;
	ppu->inputCart(cartidge);

}