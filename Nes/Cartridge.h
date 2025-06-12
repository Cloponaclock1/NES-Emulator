#pragma once
#include <vector>
#include <string>
#include <cstdint>

class Cartridge {
public:
    bool valid = false;
    std::vector<uint8_t> PRG_ROM;

    Cartridge(const std::string& filename);
    uint8_t Read(uint16_t addr);
    void Write(uint16_t addr, uint8_t data);
};