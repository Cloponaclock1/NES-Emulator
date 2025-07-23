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
    std::map<int, int> sizeCodes = {
        {0,0},
        {1,128},
        {2,256},
        {3,512},
        {4,1024},
        {5,2048},
        {6,4096},
        {7,8192},
        {8,16384},
        {9,32768},
        {10,65536},
        {11,131072},
        {12,262144},
        {13,524288},
        {14,1048576}
    };


    std::ifstream inputFile;
    bool iNes20();
    int getMapper();
    int getPRGRomSize10();
    int getPRGRomSize20();
    int getCHRRamSize();
    int getPRGRamSize();
    int getCHRRomSize20();
    int getCHRRomSize10();
    int getCHRRamSizeVol();
    int getCHRRamSizeNonVol();

    bool INESFormat();



};