#pragma once
#include "Mapper.h"
class Mapper_000 : public Mapper
{
	bool cpuMapRead(uint16_t addr, uint32_t & mapped_addr) override;
	bool cpuMapWrite(uint16_t addr, uint32_t & mapped_addr) override;
	bool ppuMapRead(uint16_t addr, uint32_t & mapped_addr) override;
	bool ppuMapWrite(uint16_t addr, uint32_t & mapped_addr) override;
};

