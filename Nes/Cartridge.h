#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <array>
#include <fstream>
#include <map>

class Cartridge {
public:
    Cartridge(const std::string& filename);

    uint8_t cpuRead(uint16_t addr);
    uint8_t cpuWrite(uint16_t addr);
    uint8_t ppuRead(uint16_t addr);
    uint8_t ppuWrite(uint16_t addr);

    bool valid = true;

private:
    int bankSize = 16384;
    std::vector<uint8_t> prgMemory;
    std::vector<uint8_t>chrMemory;
    bool iNESFormat = false;
    bool NES20Format = false;

    uint8_t mapperID =0;
    uint8_t PRGSize = 0;
    uint8_t CHRSize = 0;

    struct iNesHeader {
        //Header is 16 bytes
        std::array<uint8_t, 16> data;
    } header;


    std::ifstream inputFile;
    bool iNes20();
    int getMapper10();
    int getMapper20();

    int getPRGRomSize10();
    int getPRGRomSize20();

    int getCHRRomSize20();
    int getCHRRomSize10();
    int getCHRRamSizeVol();
    int getCHRRamSizeNonVol();
    int getPRGRamSizeVol();
    int getPRGRamSizeNonVol();
    bool INESFormat();
    int getTiming();
};