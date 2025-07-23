#include "cartridge.h"
#include <fstream>
#include <iostream>
#include <iomanip>


Cartridge::Cartridge(const std::string& filename) {
	std::ifstream inputFile(filename, std::ios::in | std::ios::binary);

	if (inputFile.is_open()) {
		// header is 16 bytes
		inputFile.read((char*)header.data.data(), 16);
	}
}
//https://www.nesdev.org/wiki/NES_2.0#Identification
bool Cartridge::INESFormat() {
	if (header.data[0] == 'N' && header.data[1] == 'E' && header.data[2] == 'S' && header.data[3] == 0x1A)
		iNESFormat = true;
}


int Cartridge::getMapper() {
	//
}
bool Cartridge::iNes20() {
	if (iNESFormat == true && (header.data[7] & 0x0C) == 0x08)
		NES20Format = true;
}
int Cartridge::getPRGRomSize20() {
	// Byte 9 LSB<<BYTE 4
	return (  ((header.data[9] &0x0F)<<8 )|(header.data[4]));
}
int Cartridge::getPRGRomSize10() {
	// BYTE 4
	return header.data[4];
}

int Cartridge::getCHRRomSize10() {
	//BYTE  5
	return header.data[5];

}
int Cartridge::getCHRRomSize20() {
	//BYTE  9 MSB << byte 5 
	return (((header.data[9] & 0xF0) << 4) | (header.data[5]));
}

int Cartridge::getCHRRamSizeVol() {
	//Need the LSB for volitile
	return (sizeCodes[(header.data[11] & 0x0F)]);

}
int Cartridge::getCHRRamSizeNonVol() {
	//Need MSB and shift down for proper code
	return (sizeCodes[((header.data[11] & 0xF0) >>4)]);

}