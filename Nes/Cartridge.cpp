#include "cartridge.h"
#include <fstream>

Cartridge::Cartridge(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) return;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> rom(size);
    if (!file.read((char*)rom.data(), size)) return;

    // Skip 16-byte iNES header
    const size_t headerSize = 16;
    const size_t prgSize = 16 * 1024;
    size_t prgCount = (rom.size() - headerSize) / prgSize;

    // Load PRG-ROM data
    if (prgCount >= 1) {
        PRG_ROM.insert(PRG_ROM.end(), rom.begin() + headerSize, rom.begin() + headerSize + prgSize);
        if (prgCount == 2) {
            PRG_ROM.insert(PRG_ROM.end(), rom.begin() + headerSize + prgSize, rom.begin() + headerSize + 2 * prgSize);
        }
        else {
            // Mirror 16KB to fill 32KB
            PRG_ROM.insert(PRG_ROM.end(), PRG_ROM.begin(), PRG_ROM.end());
        }
        valid = true;
    }
}

uint8_t Cartridge::Read(uint16_t addr) {
    if (addr >= 0x8000) {
        size_t index = addr - 0x8000;
        if (index < PRG_ROM.size())
            return PRG_ROM[index];
    }
    return 0;
}

void Cartridge::Write(uint16_t addr, uint8_t data) {
    // No-op: most cartridges are read-only
}