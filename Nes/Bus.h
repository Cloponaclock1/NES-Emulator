#pragma once
#include <cstdint>


class PPU;
class APU;
class Cartridge;
class Bus
{
public:
	Bus();
	uint8_t Read(uint16_t addr);
	void Write(uint16_t addr, uint8_t value);


private:
	static uint8_t memory[0x0800];
	PPU* ppu = nullptr;
	APU* apu = nullptr;
	Cartridge* cartridge = nullptr;

};

