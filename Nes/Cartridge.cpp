//https://www.nesdev.org/wiki/NES_2.0#Identification

#include "cartridge.h"
#include <fstream>
#include <iostream>
#include <iomanip>


Cartridge::Cartridge(const std::string& filename) {
	std::ifstream inputFile(filename, std::ios::in | std::ios::binary);

	if (inputFile.is_open()) {
		// header is 16 bytes
		inputFile.read((char*)header.data.data(), 16);
		if (header.data[6] & 0x04) {
			inputFile.seekg(512, std::ios_base::cur);
		}
		inputFile.close();
	}
	
	if (iNes20()) {
		PRGSize = ((header.data[9] & 0x0F) << 8) | header.data[4];
		CHRSize = ((header.data[9] & 0xF0) << 4) | header.data[5];
	}
	else {
		PRGSize = header.data[4];
		CHRSize = header.data[5];
	}
	prgMemory.resize(PRGSize * bankSizePRG);
	chrMemory.resize(CHRSize * bankSizeCHR);


	switch (mapperID) {
	case 0: Mapper = std::make_shared<Mapper_000>(PRGSize, CHRSize); break;
	}







}








bool Cartridge::INESFormat() {
	if (header.data[0] == 'N' && header.data[1] == 'E' && header.data[2] == 'S' && header.data[3] == 0x1A)
		iNESFormat = true;
	return iNESFormat;
}


int Cartridge::getMapper10() {
	//Flag 6 D3.D0 and Flag 7 D7.D4
	return (header.data[7] & 0xF0)| (header.data[6] >> 4) ;
}
int Cartridge::getMapper20() {
	//Byte 8 11.8 Byte 7 D7.D4 Byte 6 D3.D0
	return ((header.data[8] & 0x0F)<< 8) | ((header.data[7] & 0xF0)) | (header.data[6]>>4);

}
bool Cartridge::iNes20() {
		NES20Format = INESFormat() && ((header.data[7] & 0x0C) == 0x08);
	
	return NES20Format;
}
int Cartridge::getPRGRomSize20() {
	// Byte 9 LSB<<BYTE 4
	int bankAmount =  ( ((header.data[9] &0x0F)<<8 )|(header.data[4]));

	return bankAmount * bankSizePRG;
}
int Cartridge::getPRGRomSize10() {
	// BYTE 4
	return header.data[4] * bankSizePRG;
}

int Cartridge::getCHRRomSize10() {
	//BYTE  5
	return header.data[5] * bankSizeCHR;

}
int Cartridge::getCHRRomSize20() {
	//BYTE  9 MSB << byte 5 
	int bankAmount (((header.data[9] & 0xF0) << 4) | (header.data[5]));
	return bankAmount * bankSizeCHR;
}

int Cartridge::getCHRRamSizeVol() {
	//Need the LSB for volitile
	if (!iNes20()) return 0;
	int shift = (header.data[11] & 0x0F);

	if (shift == 0) { return 0; }
	return 64 << shift;

}
int Cartridge::getCHRRamSizeNonVol() {
	if (!iNes20()) return 0;
	//Need MSB and shift down for proper code number
	int shift = ((header.data[11] & 0xF0) >> 4);

	if (shift == 0) { return 0; }
	return 64 << shift;

}
int Cartridge::getPRGRamSizeVol() {
	if (!iNes20()) return 0;
	//Need the LSB for volitile
	int shift = (header.data[10] & 0x0F);
	if (shift == 0) { return 0; }
	return 64 << shift;

}
int Cartridge::getPRGRamSizeNonVol() {
	if (!iNes20()) return 0;
	//Need MSB and shift down for proper code number
	int shift = ((header.data[10] & 0xF0) >> 4);
	if (shift == 0) { return 0; }
	return 64<< shift;

}
int Cartridge::getTiming() {
	//0 NTSC, 1 PAL, 2 Multi, 3 Dendy
	if (!iNes20()) return 0;
	return (header.data[12] & 0x03);
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &value) {
	uint32_t mapped_addr = 0;
	if (Mapper->cpuMapRead(addr, mapped_addr)) {
		value = prgMemory[mapped_addr];
		return true;
	}
	else
	return false;
}
bool Cartridge::cpuWrite(uint16_t addr, uint8_t value) {
	uint32_t mapped_addr = 0;
	if (Mapper->cpuMapRead(addr, mapped_addr)) {
		value = prgMemory[mapped_addr];
		return true;
	}
	else
		return false;
}
bool Cartridge::ppuRead(uint16_t addr, uint8_t& value) {
	uint32_t mapped_addr = 0;
	if (Mapper->ppuMapRead(addr, mapped_addr)) {
		value = chrMemory[mapped_addr];
		return true;
	}
	else
		return false;
};
bool Cartridge::ppuWrite(uint16_t addr, uint8_t value) {
	uint32_t mapped_addr = 0;
	if (Mapper->ppuMapRead(addr, mapped_addr)) {
		value = chrMemory[mapped_addr];
		return true;
	}
	else
		return false;
}
int Cartridge::getPRGSize() {
	return PRGSize;
};
int Cartridge::getCHRSize() {
	return CHRSize;


};
