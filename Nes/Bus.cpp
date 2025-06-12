#include "Bus.h"
#include "cpu.h"
#include "cartridge.h"
Cartridge* cartridge = nullptr;


Bus::Bus() {
	



}
uint8_t Bus::memory[0x0800] = {};

uint8_t Bus::Read(uint16_t addr) {
	//Ram
	if (addr <= 0x1FFF) {
		// memory is 2KB so wrap around
		
		return memory[addr % 0x0800];
	}
	//PPU registers
	//8 bytes in size repeating
	else if (addr <= 0x3FFF) {
		//return ppu->Read(addr % 8);
		return 0;

	}
	//APU and I/O registers
	//Not repeated only 1 byte
	else if (addr <= 0x4017) {
		//return apu->Read(addr);

	}
	//Disabled APU and I/O stuff
	else if (addr <= 0x401F) {
		return 0;
	}
	//Unampped for Cartidge
	//Not repeated
	else if (addr <= 0xFFFF) {
		//return memory[addr];
		return cartridge ? cartridge->Read(addr) : 0;

	}

}

void Bus::Write(uint16_t addr, uint8_t value) {
	//Ram
	if (addr <= 0x1FFF) {
		// memory is 2KB so wrap around

		memory[addr % 0x0800] = value;
	}
	//PPU registers
	//8 bytes in size repeating
	else if (addr <= 0x3FFF) {
		//ppu->Write((addr % 8), value);

	}
	//APU and I/O registers
	//Not repeated only 1 byte
	else if (addr <= 0x4017) {
		//apu->Write((addr), value);

	}
	//Disabled APU and I/O stuff
	else if (addr <= 0x401F) {
		
	}
	//Unampped for Cartidge
	//Not repeated
	else if (addr <= 0xFFFF) {
		//cartridge->Write((addr), value);
		if (cartridge) cartridge->Write(addr, value);

	}

}