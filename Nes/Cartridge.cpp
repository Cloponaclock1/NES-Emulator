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
}
bool Cartridge::INESFormat() {
	if (header.data[0] == 'N' && header.data[1] == 'E' && header.data[2] == 'S' && header.data[3] == 0x1A)
		iNESFormat = true;
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
	INESFormat();
	if (iNESFormat == true && (header.data[7] & 0x0C) == 0x08)
		NES20Format = true;
	return NES20Format;
}
int Cartridge::getPRGRomSize20() {
	// Byte 9 LSB<<BYTE 4
	int bankAmount =  ( ((header.data[9] &0x0F)<<8 )|(header.data[4]));

	return bankAmount * bankSize;
}
int Cartridge::getPRGRomSize10() {
	// BYTE 4
	return header.data[4] * bankSize;
}

int Cartridge::getCHRRomSize10() {
	//BYTE  5
	return header.data[5] * bankSize;

}
int Cartridge::getCHRRomSize20() {
	//BYTE  9 MSB << byte 5 
	int bankAmount (((header.data[9] & 0xF0) << 4) | (header.data[5]));
	return bankAmount * bankSize;
}

int Cartridge::getCHRRamSizeVol() {
	//Need the LSB for volitile
	return 64 << (header.data[11] & 0x0F);

}
int Cartridge::getCHRRamSizeNonVol() {
	//Need MSB and shift down for proper code number
	return 64 << ((header.data[11] & 0xF0) >>4);

}
int Cartridge::getPRGRamSizeVol() {
	//Need the LSB for volitile
	return 64 << (header.data[10] & 0x0F);

}
int Cartridge::getPRGRamSizeNonVol() {
	//Need MSB and shift down for proper code number
	return 64<< ((header.data[10] & 0xF0) >> 4);

}
int Cartridge::getTiming() {
	//0 NTSC, 1 PAL, 2 Multi, 3 Dendy
	return (header.data[12] & 0x03);
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &value) {

}
bool Cartridge::cpuWrite(uint16_t addr, uint8_t value) {

}