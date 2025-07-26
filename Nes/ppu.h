#pragma once

#include <cstdint>
#include <array>
#include <memory>
#include "Cartridge.h"

class Cartridge;


class PPU
{
private:
	uint8_t PPUCTRL = 0b00000000;
	uint8_t PPUMASK = 0b00000000;
	uint8_t PPUSTATUS = 0b00000000;
	uint8_t OAMADDR = 0b00000000;
	uint8_t PPSCROLL= 0b00000000;
	uint8_t PPUADDR = 0b00000000;
	uint8_t PPUDATA = 0b00000000;
	uint8_t OAMAMA = 0b00000000;
	std::shared_ptr<Cartridge> cart;
	// Table is actually 1kb and theres 2, table pattern would be 2 4kb 
	std::array<uint8_t, 2048>nameTable;
	std::array<uint8_t, 32>tablePalette;
	// two 4kb's that are mirrored depnding on mapping type
	std::array<uint8_t, 8192>tablePattern;

public:

	uint8_t cpuRead(uint16_t addr, bool read);
	void cpuWrite(uint16_t addr, uint8_t value);
	uint8_t ppuRead(uint16_t addr, bool read);
	void ppuWrite(uint16_t addr, uint8_t value);
	void Clock();
	void inputCart(const std::shared_ptr<Cartridge>& cartridge);






};

