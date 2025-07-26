#pragma once
#include <cstdint>
#include <vector>
class Mapper
{
public:
	Mapper(uint16_t prgBank, uint16_t chrBank);
	virtual bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual ~Mapper() = default;


protected:
	uint16_t mprgBank ;
	uint16_t mchrBank ;
};

