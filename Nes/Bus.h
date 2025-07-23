#pragma once
#include <cstdint>
#include <array>
#include "ppu.h"
#include "cpu.h"



// Forward declarations
class CPU;

class PPU;
class APU;
class Cartridge;

class Bus {
public:
    Bus();
    void connectCPU(CPU* _cpu);
    void connectPPU(PPU* _ppu);
    void inputCart(const::std::shared_ptr<Cartridge>& cartidge);

    std::shared_ptr<Cartridge> cart  = nullptr;


    std::array<uint8_t, 0x2048> cpuram;
    

    uint8_t cpuRead(uint16_t addr);
    void cpuWrite(uint16_t addr, uint8_t value);
    void Reset();
    void Clock();



private:
    int systemClockCounter =0;
    CPU* cpu = nullptr;
    PPU* ppu = nullptr;

};