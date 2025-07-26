#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <array>
#include <fstream>
#include <map>
#include "Mapper_000.h"

class Cartridge {
public:
    Cartridge(const std::string& filename);

    bool cpuRead(uint16_t addr, uint8_t &value);
    bool cpuWrite(uint16_t addr, uint8_t value);
    bool ppuRead(uint16_t addr, uint8_t &value);
    bool ppuWrite(uint16_t addr, uint8_t value);

    bool valid = true;

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
    int getPRGSize();
    int getCHRSize();





private:
    std::shared_ptr<Mapper> Mapper;
    int bankSize = 16384;
    std::vector<uint8_t> prgMemory;
    std::vector<uint8_t>chrMemory;
    bool iNESFormat = false;
    bool NES20Format = false;

    uint16_t mapperID =0;
    uint16_t PRGSize = 0;
    uint16_t CHRSize = 0;

    struct iNesHeader {
        //Header is 16 bytes
        std::array<uint8_t, 16> data;
    } header;


    std::ifstream inputFile;


};