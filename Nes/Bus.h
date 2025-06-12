#pragma once
#include <cstdint>

// Forward declarations
class PPU;
class APU;
class Cartridge;

class Bus {
public:
    Bus();
    uint8_t Read(uint16_t addr);
    void Write(uint16_t addr, uint8_t value);

    Cartridge* cartridge = nullptr;
    PPU* ppu = nullptr;
    APU* apu = nullptr;

private:
    static uint8_t memory[0x0800];
};