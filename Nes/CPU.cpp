#include <iostream>
#include <fstream>
#include <iomanip>
#include "cpu.h"

CPU::CPU() {
	InitopcodeTable();
}


void CPU::ConnectBus(Bus* b) {
	bus = b;
	}



//Adressing modes

uint8_t CPU::FetchImmediate() {
	return (bus->cpuRead(PC+ 1));

}

uint8_t CPU::FetchZeroPage() {
	uint8_t addr = bus->cpuRead(PC + 1);
	return bus->cpuRead(addr);
}

uint8_t CPU::FetchZeroPageX() {
	uint8_t base = bus->cpuRead(PC + 1);//get operand
	uint8_t address = (base + X) & 0xFF; // get nex address +X
	return bus->cpuRead(address); // Store in A

}

uint8_t CPU::FetchZeroPageY() {
	uint8_t base = bus->cpuRead(PC + 1);//get operand
	uint8_t address = (base + Y) & 0xFF; // get nex address +X
	return bus->cpuRead(address); // Store in A

}
uint8_t CPU::FetchAbsolute() {
	uint16_t address = bus->cpuRead(PC + 1) | (bus->cpuRead(PC + 2) << 8);
	return bus->cpuRead(address);
}
uint8_t CPU::FetchAbsoluteX() {

	uint16_t base = (bus->cpuRead(PC + 1) | (bus->cpuRead(PC + 2) << 8));
	uint16_t address = base + X;
	uint8_t value = bus->cpuRead(address);

	std::cout << std::hex << "PC=$" << PC
		<< " X=$" << (int)X
		<< " LDA $" << base << ",X @ $" << address
		<< " = $" << (int)value << "\n";
	if ((base & 0xFF00) != (address & 0xFF00)) {
		cycles++;
	}

	return bus->cpuRead(address);
}
uint8_t CPU::FetchAbsoluteY() {
	uint16_t base = (bus->cpuRead(PC + 1) | (bus->cpuRead(PC + 2) << 8));
	uint16_t address = base + Y;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		cycles++;
	}
	return bus->cpuRead(address);
}
uint8_t CPU::FetchIndirectX() {
	uint8_t zp_addr = (bus->cpuRead(PC + 1) + X) & 0xFF; // wrap around
	uint8_t low = bus->cpuRead(zp_addr);
	uint8_t high = bus->cpuRead((zp_addr + 1) & 0xFF); // wrap again
	uint16_t address = (high << 8) | low;
	return bus->cpuRead(address);
}
uint8_t CPU::FetchIndirectY() {
	uint8_t zptr = bus->cpuRead(PC + 1);

	uint16_t base = bus->cpuRead(zptr) | (bus->cpuRead((zptr + 1) & 0xFF) << 8);

	uint16_t address = base + Y;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		cycles++;
	}
	return bus->cpuRead(address);
}

uint16_t CPU::GetZeroPageAddress() {
	return bus->cpuRead(PC + 1);
}

uint16_t CPU::GetZeroPageAddressX() {
	return (bus->cpuRead(PC + 1) + X) & 0xFF;
}
uint16_t CPU::GetZeroPageAddressY() {
	return (bus->cpuRead(PC + 1) + Y) & 0xFF;
}
uint16_t CPU::GetAddressAbsolute() {
	uint8_t lo = bus->cpuRead(PC + 1);
	uint8_t hi = bus->cpuRead(PC + 2);
	uint16_t addr = (hi << 8) | lo;



	return addr;
}
uint16_t CPU::GetAddressAbsoluteX() {
	return ((bus->cpuRead(PC + 1) | (bus->cpuRead(PC + 2) << 8)) +X);

}
uint16_t CPU::GetAddressAbsoluteY() {
	return ((bus->cpuRead(PC + 1) | (bus->cpuRead(PC + 2) << 8)) + Y);

}
uint16_t CPU::GetIndirectAddressX() {
	uint8_t index = (bus->cpuRead(PC + 1) + X) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
	uint16_t addr = (bus->cpuRead((index + 1) & 0xFF) << 8) | bus->cpuRead(index);
	return addr;

}
uint16_t CPU::GetIndirectAddressY() {
	uint8_t index = (bus->cpuRead(PC + 1) + Y) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
	uint16_t addr = (bus->cpuRead((index + 1) & 0xFF) << 8) | bus->cpuRead(index);
	return addr;

}
//LDA
//LDA cycle cost 

void CPU::LDA(uint8_t value) {
	//Opcode then operand 2 bytes and 2 cycles

	A = value;
	CheckByte(A);
}


//STA
void CPU::STA(uint16_t value) {
	//opcode then a memory location to store A

	bus->cpuWrite(value,A);// store A in it

}


void CPU::STX(uint16_t value) {

	//Store X into memory location
	bus->cpuWrite(value,X);
}


void CPU::STY(uint16_t value) {

	//Store Y into memory location
	bus->cpuWrite(value,Y);
}

//LDX
void CPU::LDX(uint8_t value) {
	//Opcode then operand 2 bytes and 2 cycles
	X = value;
	CheckByte(X);
}


void CPU::LDY(uint8_t value) {
	//Opcode then operand 2 bytes and 2 cycles
	Y = value;
	CheckByte(Y);


}

void CPU::PHA() {

	bus->cpuWrite(0x0100 + S, A);
	S--;

}
void CPU::PLA() {
	S++;


	A = bus->cpuRead(0x0100 + S);
	CheckByte(A);


}
void CPU::PHP() {
	bus->cpuWrite(0x0100 + S,  P| 0b00110000);
	S--;


}
void CPU::PLP() {
	S++;

	uint8_t value = bus->cpuRead(0x0100 + S);
	P = (value & 0b11001111) | 0b00100000;
}

void CPU::TAX() {
	// Dont need the memory access but itll make the loop simplier
	X = A;
	CheckByte(X);

}

void CPU::TAY() {
	// Dont need the memory access but itll make the loop simplier

	Y = A;
	CheckByte(Y);

}

void CPU::TXA() {
	// Dont need the memory access but itll make the loop simplier

	A = X;
	CheckByte(A);

}

void CPU::TYA() {
	// Dont need the memory access but itll make the loop simplier

	A = Y;
	CheckByte(A);

}
void CPU::INX() {
	// Dont need the memory access but itll make the loop simplier

	X++;
	CheckByte(X);

}
void CPU::INY() {
	// Dont need the memory access but itll make the loop simplier

	Y++;
	CheckByte(Y);

}

void CPU::DEX() {
	// Dont need the memory access but itll make the loop simplier

	X--;
	CheckByte(X);

}
void CPU::DEY() {
	// Dont need the memory access but itll make the loop simplier

	Y--;
	CheckByte(Y);

}
void CPU::JSR(uint16_t addr) {

    // Correct return address: PC of the instruction AFTER JSR, minus 1
    uint16_t return_address = PC + 2;

    bus->cpuWrite(0x0100 + S, (return_address >> 8) & 0xFF); // high byte
    S--;
    bus->cpuWrite(0x0100 + S, return_address & 0xFF);        // low byte
    S--;

    PC = addr;


}
void CPU::RTS() {

	S++;
	uint8_t low = bus->cpuRead(0x0100 + S);
	S++;
	uint8_t high = bus->cpuRead(0x0100 + S);
	PC = (high << 8 | low ) + 1;


}
void CPU::BRK() {


	PC++; // BRK skips next byte (acts like a 2-byte instruction)

	// Compute values to push
	uint8_t pch = (PC >> 8) & 0xFF;
	uint8_t pcl = PC & 0xFF;
	uint8_t status = P | 0x30; // Set B and U flags



	// Push PC and status to stack
	bus->cpuWrite(0x0100 + S--, pch);  // PCH
	bus->cpuWrite(0x0100 + S--, pcl);  // PCL
	bus->cpuWrite(0x0100 + S--, status); // Status byte

	// Set I flag
	SetInterrupt();

	// cpuRead IRQ vector
	uint8_t low = bus->cpuRead(0xFFFE);
	uint8_t high = bus->cpuRead(0xFFFF);
	PC = (high << 8) | low;


}
void CPU::RTI() {

	S++;
	P = bus->cpuRead(0x0100 + S);
	ClearBreak();
	SetUnused();

	S++;
	uint8_t pcl = bus->cpuRead(0x0100 + S);
	S++;
	uint8_t pch = bus->cpuRead(0x0100 + S);
	PC = (pch << 8) | pcl;

}
void CPU::ADC(uint8_t value) {
	// Add contents of memory to A with the carry bit of P
	uint8_t base = value;
	uint16_t result = base + A + (P & 0b00000001);

	// If carry, past FF, the carry flag needs to be set
	if ((result >> 8) != 0) { SetCarry(); }
	else { ClearCarry(); }

	// Check for signed overflow
	if ((~(base ^ A) & (result ^ base)) & 0x80) {
		SetOverflow();
	}
	else {
		ClearOverflow();
	}

	// Wrap result to 8 bits
	A = result & 0xFF;

	// Set or clear Zero flag
	CheckByte(A);


}

void CPU::SBC(uint8_t value) {
	// Add contents of memory to A with the carry bit of P
	uint16_t result = (~value) + A + (P & 0b00000001);

	// If carry, past FF, the carry flag needs to be set
	if ((result >> 8) == 0) { SetCarry(); }
	else { ClearCarry(); }

	// Check for signed overflow
	//https://www.nesdev.org/wiki/Instruction_reference#SBC
	if (((result ^ A) & (result ^ ~value)) & 0x80) {
		SetOverflow();
	}
	else {
		ClearOverflow();
	}

	// Wrap result to 8 bits
	A = result & 0xFF;

	// Set or clear Zero flag
	CheckByte(A);

}
void CPU::AND(uint8_t value) {
	A = A & value;

	CheckByte(A);
}
void CPU::ASL(uint16_t addr) {
	//Check bit 7 before bit shift to see  value for carry

	uint8_t value = bus->cpuRead(addr);
	if (value & 0b10000000) {
		SetCarry();
	}
	else { ClearCarry(); }
	bus->cpuWrite(addr, value <<= 1);
	CheckByte(value);


}
void CPU::ASL_A() {
	//Check bit 7 before bit shift to see  value for carry
	if (A & 0b10000000) {
		SetCarry();
	}
	else { ClearCarry(); }
	A = A << 1;
	CheckByte(A);
}

void CPU::BCC() {
	int8_t offset = (int8_t)(bus->cpuRead(PC + 1));



	if (!GetCarry()) {
		uint16_t nonOffestPC = PC + 2;
		uint16_t newPC = PC + 2 + offset;
		// Do we go to a new page if so +1 cycle
		if ((newPC & 0xFF00) != (nonOffestPC & 0xFF00)) {
			
			cycles += 1;
		}
		// If we branch we have added 2 cycles if not jsut added 1. So 3 if branch taken and 4 if page crossed works
		PC = newPC;
		cycles += 1;
	}
	else {
		//Branch wasnt taken advance 2 bytes
		PC += 2;
	}

}
void CPU::BCS() {
	int8_t offset = (int8_t)(bus->cpuRead(PC + 1));



	if (GetCarry()) {
		uint16_t nonOffestPC = PC + 2;
		uint16_t newPC = PC + 2 + offset;
		// Do we go to a new page if so +1 cycle
		if ((newPC & 0xFF00) != (nonOffestPC & 0xFF00)) {

			cycles += 1;
		}
		// If we branch we have added 2 cycles if not jsut added 1. So 3 if branch taken and 4 if page crossed works
		PC = newPC;
		cycles += 1;
	}
	else {
		//Branch wasnt taken advance 2 bytes
		PC += 2;
	}

}
void CPU::BEQ() {
	int8_t offset = (int8_t)(bus->cpuRead(PC + 1));

	if (GetZero()) {
		uint16_t nonOffestPC = PC + 2;
		uint16_t newPC = PC + 2 + offset;
		// Do we go to a new page if so +1 cycle
		if ((newPC & 0xFF00) != (nonOffestPC & 0xFF00)) {

			cycles += 1;
		}
		// If we branch we have added 2 cycles if not jsut added 1. So 3 if branch taken and 4 if page crossed works
		PC = newPC;
		cycles += 1;
	}
	else {
		//Branch wasnt taken advance 2 bytes
		PC += 2;
	}

}
void CPU::BIT(uint8_t value) {
	if ((A & value) ==0) {
		SetZero();
	}
	else { ClearZero(); }
	if (value & 0b10000000) {
		SetNegative();
	}
	else {
		ClearNegative();
	}if (value & 0b01000000) {
		SetOverflow();
	}
	else {
		ClearOverflow();
	}
}

void CPU::BMI() {
	int8_t offset = (int8_t)(bus->cpuRead(PC + 1));

	if (GetNegative()) {
		uint16_t nonOffestPC = PC + 2;
		uint16_t newPC = PC + 2 + offset;
		// Do we go to a new page if so +1 cycle
		if ((newPC & 0xFF00) != (nonOffestPC & 0xFF00)) {

			cycles += 1;
		}
		// If we branch we have added 2 cycles if not jsut added 1. So 3 if branch taken and 4 if page crossed works
		PC = newPC;
		cycles += 1;
	}
	else {
		//Branch wasnt taken advance 2 bytes
		PC += 2;
	}


}
void CPU::BNE() {
	int8_t offset = (int8_t)(bus->cpuRead(PC + 1));

	if (!GetZero()) {
		uint16_t nonOffestPC = PC + 2;
		uint16_t newPC = PC + 2 + offset;
		// Do we go to a new page if so +1 cycle
		if ((newPC & 0xFF00) != (nonOffestPC & 0xFF00)) {

			cycles += 1;
		}
		// If we branch we have added 2 cycles if not jsut added 1. So 3 if branch taken and 4 if page crossed works
		PC = newPC;
		cycles += 1;
	}
	else {
		//Branch wasnt taken advance 2 bytes
		PC += 2;
	}

}
void CPU::BPL() {
	int8_t offset = (int8_t)(bus->cpuRead(PC + 1));

	if (!GetNegative()) {
		uint16_t nonOffestPC = PC + 2;
		uint16_t newPC = PC + 2 + offset;
		// Do we go to a new page if so +1 cycle
		if ((newPC & 0xFF00) != (nonOffestPC & 0xFF00)) {

			cycles += 1;
		}
		// If we branch we have added 2 cycles if not jsut added 1. So 3 if branch taken and 4 if page crossed works
		PC = newPC;
		cycles += 1;
	}
	else {
		//Branch wasnt taken advance 2 bytes
		PC += 2;
	}

}
void CPU::BVC() {
	int8_t offset = (int8_t)(bus->cpuRead(PC + 1));

	if (!GetOverflow()) {
		uint16_t nonOffestPC = PC + 2;
		uint16_t newPC = PC + 2 + offset;
		// Do we go to a new page if so +1 cycle
		if ((newPC & 0xFF00) != (nonOffestPC & 0xFF00)) {

			cycles += 1;
		}
		// If we branch we have added 2 cycles if not jsut added 1. So 3 if branch taken and 4 if page crossed works
		PC = newPC;
		cycles += 1;
	}
	else {
		//Branch wasnt taken advance 2 bytes
		PC += 2;
	}

}
void CPU::BVS() {
	int8_t offset = (int8_t)(bus->cpuRead(PC + 1));

	if (GetOverflow()) {
		uint16_t nonOffestPC = PC + 2;
		uint16_t newPC = PC + 2 + offset;
		// Do we go to a new page if so +1 cycle
		if ((newPC & 0xFF00) != (nonOffestPC & 0xFF00)) {

			cycles += 1;
		}
		// If we branch we have added 2 cycles if not jsut added 1. So 3 if branch taken and 4 if page crossed works
		PC = newPC;
		cycles += 1;
	}
	else {
		//Branch wasnt taken advance 2 bytes
		PC += 2;
	}

}


void CPU::CLC() {
	ClearCarry();
}

void CPU::CLD() {
	ClearDecimal();
}

void CPU::CLI() {
	ClearInterrupt();
}
void CPU::CLV() {
	ClearOverflow();
}

void CPU::CMP(uint8_t value) {
	uint16_t temp = A - value;
	if (A >= value) {
		SetCarry();
	}
	else { ClearCarry(); }
	if (A == value) {
		SetZero();
	}
	else { ClearZero(); }
	// Even though its 16 bits the 7th bit will still hold the negative 
	if (temp & 0b10000000) {
		SetNegative();
	}
	else { ClearNegative(); }
}

void CPU::CPX(uint8_t value) {
	uint16_t temp = X - value;
	if (X >= value) {
		SetCarry();
	}
	else { ClearCarry(); }
	if (X == value) {
		SetZero();
	}
	else { ClearZero(); }
	// Even though its 16 bits the 7th bit will still hold the negative 
	if (temp & 0b10000000) {
		SetNegative();
	}
	else { ClearNegative(); }
}

void CPU::CPY(uint8_t value) {
	uint16_t temp = Y - value;
	if (Y >= value) {
		SetCarry();
	}
	else { ClearCarry(); }
	if (Y == value) {
		SetZero();
	}
	else { ClearZero(); }
	// Even though its 16 bits the 7th bit will still hold the negative 
	if (temp & 0b10000000) {
		SetNegative();
	}
	else { ClearNegative(); }
}

void CPU::DEC(uint16_t addr) {
	uint8_t value = bus->cpuRead(addr);
	value -= 1;
	bus->cpuWrite(addr, value);
	CheckByte(value);

}


void CPU::EOR(uint8_t value) {
	A = A ^ value;

	CheckByte(A);
}

void CPU::INC(uint16_t addr) {
	uint8_t value = bus->cpuRead(addr);
	value += 1;
	bus->cpuWrite(addr, value);
	CheckByte(value);

}
void CPU::JMP(uint16_t addr) {

	PC = addr;

}
void CPU::JMPIndirect() {
	uint8_t low = bus->cpuRead(PC + 1);
	uint8_t high = bus->cpuRead(PC + 2);
	uint16_t pointer = (high << 8) | low;

	uint8_t target_low = bus->cpuRead(pointer);
	uint8_t target_high;

	// Buggy wrap around
	if ((pointer & 0x00FF) == 0x00FF)
		target_high = bus->cpuRead(pointer & 0xFF00);  // wrap around to start of page
	else
		target_high = bus->cpuRead(pointer + 1);

	PC = (target_high << 8) | target_low;

}
void CPU::LSR_A() {
	if (A & 0b00000001) { SetCarry(); }
	else
	{
		ClearCarry();
	}
	A >>= 1;

	CheckByte(A);
}
void CPU::LSR(uint16_t addr) {
	uint8_t value= bus->cpuRead(addr);
	if (value & 0b00000001) { SetCarry(); }
	else
	{
		ClearCarry();
	}
	value >>= 1;
	bus->cpuWrite(addr, value);
	CheckByte(value);

}
void CPU::NOP() {
	//Do nothing
}
void CPU::ORA(uint8_t value) {
	A = A | value;

	CheckByte(A);
}
void CPU::ROL(uint16_t addr) {
	uint8_t value = bus->cpuRead(addr);
	bool oldCarry = GetCarry();
	if (value & 0b10000000) SetCarry(); else ClearCarry();
	value <<= 1;
	if (oldCarry) {
		value |= 0b00000001;
	};

	CheckByte(value);
	bus->cpuWrite(addr, value);

}
void CPU::ROL_A() {
	bool oldCarry = GetCarry();

	if (A & 0b10000000) SetCarry(); else ClearCarry();
	A <<= 1;
	if (oldCarry) {
		A |= 0b00000001;
	}
	CheckByte(A);
}

void CPU::ROR_A() {

	bool oldCarry = GetCarry();
	if (A & 0b00000001) SetCarry(); else ClearCarry();
	A >>= 1;
	if (oldCarry) {
		A |= 0b10000000;
	};
	CheckByte(A);
}

void CPU::ROR(uint16_t addr) {
	uint8_t value = bus->cpuRead(addr);
	bool oldCarry = GetCarry();
	if (value & 0x01) SetCarry(); else ClearCarry();
	value >>= 1;
	if (oldCarry) value |= 0x80;
	CheckByte(value);
	bus->cpuWrite(addr, value);

}

void CPU::SEC() {
	SetCarry();
}
void CPU::SED() {
	SetDecimal();
}
void CPU::SEI() {
	SetInterrupt();
}
void CPU::TSX() {
	X = S;
	CheckByte(X);
}
void CPU::TXS() {
	S = X;
}
//Register check Function for negaive and 0
void CPU::CheckByte(uint8_t reg) {
	if (reg == 0) { SetZero(); }
	else { ClearZero(); }
	if ((reg & 0b10000000) != 0) { SetNegative(); }
	else { ClearNegative(); }

}
void CPU::NOP_Illegal() {

}

void CPU::InitopcodeTable() {
	for (int i = 0; i < 256; ++i) {
		if (!opcodeTable[i].handler) {
			opcodeTable[i] = { [this]() { NOP_Illegal(); }, 2, 3 };
		}
	}

	//LDA
// LDA – Load Accumulator
	opcodeTable[0xA9] = { [this]() { LDA(FetchImmediate()); }, 2, 2 };
	opcodeTable[0xA5] = { [this]() { LDA(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0xB5] = { [this]() { LDA(FetchZeroPageX()); }, 2, 4 };
	opcodeTable[0xAD] = { [this]() { LDA(FetchAbsolute()); }, 3, 4 };
	opcodeTable[0xBD] = { [this]() { LDA(FetchAbsoluteX()); }, 3, 4 };
	opcodeTable[0xB9] = { [this]() { LDA(FetchAbsoluteY()); }, 3, 4 };
	opcodeTable[0xA1] = { [this]() { LDA(FetchIndirectX()); }, 2, 6 };
	opcodeTable[0xB1] = { [this]() { LDA(FetchIndirectY()); }, 2, 5 };

	// STA – Store Accumulator
	opcodeTable[0x85] = { [this]() { STA(GetZeroPageAddress()); }, 2, 3 };
	opcodeTable[0x95] = { [this]() { STA(GetZeroPageAddressX()); }, 2, 4 };
	opcodeTable[0x8D] = { [this]() { STA(GetAddressAbsolute()); }, 3, 4 };
	opcodeTable[0x9D] = { [this]() { STA(GetAddressAbsoluteX()); }, 3, 5 };
	opcodeTable[0x99] = { [this]() { STA(GetAddressAbsoluteY()); }, 3, 5 };
	opcodeTable[0x81] = { [this]() { STA(GetIndirectAddressX()); }, 2, 6 };
	opcodeTable[0x91] = { [this]() { STA(GetIndirectAddressY()); }, 2, 6 };

	// STX – Store X Register
	opcodeTable[0x86] = { [this]() { STX(GetZeroPageAddress()); }, 2, 3 };
	opcodeTable[0x96] = { [this]() { STX(GetZeroPageAddressY()); }, 2, 4 };
	opcodeTable[0x8E] = { [this]() { STX(GetAddressAbsolute()); }, 3, 4 };

	// STY – Store Y Register
	opcodeTable[0x84] = { [this]() { STY(GetZeroPageAddress()); }, 2, 3 };
	opcodeTable[0x94] = { [this]() { STY(GetZeroPageAddressX()); }, 2, 4 };
	opcodeTable[0x8C] = { [this]() { STY(GetAddressAbsolute()); }, 3, 4 };

	// LDX – Load X Register
	opcodeTable[0xA2] = { [this]() { LDX(FetchImmediate()); }, 2, 2 };
	opcodeTable[0xA6] = { [this]() { LDX(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0xB6] = { [this]() { LDX(FetchZeroPageY()); }, 2, 4 };
	opcodeTable[0xAE] = { [this]() { LDX(FetchAbsolute()); }, 3, 4 };
	opcodeTable[0xBE] = { [this]() { LDX(FetchAbsoluteY()); }, 3, 4 };

	// LDY – Load Y Register
	opcodeTable[0xA0] = { [this]() { LDY(FetchImmediate()); }, 2, 2 };
	opcodeTable[0xA4] = { [this]() { LDY(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0xB4] = { [this]() { LDY(FetchZeroPageX()); }, 2, 4 };
	opcodeTable[0xAC] = { [this]() { LDY(FetchAbsolute()); }, 3, 4 };
	opcodeTable[0xBC] = { [this]() { LDY(FetchAbsoluteX()); }, 3, 4 };

	// Transfer
	opcodeTable[0xAA] = { [this]() { TAX(); }, 1, 2 };
	opcodeTable[0x8A] = { [this]() { TXA(); }, 1, 2 };
	opcodeTable[0xA8] = { [this]() { TAY(); }, 1, 2 };
	opcodeTable[0x98] = { [this]() { TYA(); }, 1, 2 };
	opcodeTable[0xBA] = { [this]() { TSX(); }, 1, 2 };
	opcodeTable[0x9A] = { [this]() { TXS(); }, 1, 2 };

	// Stack Operations
	opcodeTable[0x48] = { [this]() { PHA(); }, 1, 3 };
	opcodeTable[0x68] = { [this]() { PLA(); }, 1, 4 };
	opcodeTable[0x08] = { [this]() { PHP(); }, 1, 3 };
	opcodeTable[0x28] = { [this]() { PLP(); }, 1, 4 };

	// Control Flow
	opcodeTable[0x20] = { [this]() { JSR(GetAddressAbsolute()); }, 0, 6 };
	opcodeTable[0x60] = { [this]() { RTS(); }, 0, 6 };
	opcodeTable[0x00] = { [this]() { BRK(); }, 0, 7 };
	opcodeTable[0x40] = { [this]() { RTI(); }, 0, 6 };
	opcodeTable[0x4C] = { [this]() { JMP(GetAddressAbsolute()); }, 0, 3 };
	opcodeTable[0x6C] = { [this]() { JMPIndirect(); }, 0, 5 };

	// Branches
	opcodeTable[0x90] = { [this]() { BCC(); }, 0, 2 };
	opcodeTable[0xB0] = { [this]() { BCS(); }, 0, 2 };
	opcodeTable[0xF0] = { [this]() { BEQ(); }, 0, 2 };
	opcodeTable[0x30] = { [this]() { BMI(); }, 0, 2 };
	opcodeTable[0xD0] = { [this]() { BNE(); }, 0, 2 };
	opcodeTable[0x10] = { [this]() { BPL(); }, 0, 2 };
	opcodeTable[0x50] = { [this]() { BVC(); }, 0, 2 };
	opcodeTable[0x70] = { [this]() { BVS(); }, 0, 2 };

	// Set Flags
	opcodeTable[0x38] = { [this]() { SEC(); }, 1, 2 };
	opcodeTable[0xF8] = { [this]() { SED(); }, 1, 2 };
	opcodeTable[0x78] = { [this]() { SEI(); }, 1, 2 };

	// Clear Flags
	opcodeTable[0x18] = { [this]() { CLC(); }, 1, 2 };
	opcodeTable[0xD8] = { [this]() { CLD(); }, 1, 2 };
	opcodeTable[0x58] = { [this]() { CLI(); }, 1, 2 };
	opcodeTable[0xB8] = { [this]() { CLV(); }, 1, 2 };

	// Compare
	opcodeTable[0xC9] = { [this]() { CMP(FetchImmediate()); }, 2, 2 };
	opcodeTable[0xC5] = { [this]() { CMP(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0xD5] = { [this]() { CMP(FetchZeroPageX()); }, 2, 4 };
	opcodeTable[0xCD] = { [this]() { CMP(FetchAbsolute()); }, 3, 4 };
	opcodeTable[0xDD] = { [this]() { CMP(FetchAbsoluteX()); }, 3, 4 };
	opcodeTable[0xD9] = { [this]() { CMP(FetchAbsoluteY()); }, 3, 4 };
	opcodeTable[0xC1] = { [this]() { CMP(FetchIndirectX()); }, 2, 6 };
	opcodeTable[0xD1] = { [this]() { CMP(FetchIndirectY()); }, 2, 5 };
	opcodeTable[0xE0] = { [this]() { CPX(FetchImmediate()); }, 2, 2 };
	opcodeTable[0xE4] = { [this]() { CPX(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0xEC] = { [this]() { CPX(FetchAbsolute()); }, 3, 4 };
	opcodeTable[0xC0] = { [this]() { CPY(FetchImmediate()); }, 2, 2 };
	opcodeTable[0xC4] = { [this]() { CPY(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0xCC] = { [this]() { CPY(FetchAbsolute()); }, 3, 4 };

	// Arithmetic
	opcodeTable[0x69] = { [this]() { ADC(FetchImmediate()); }, 2, 2 };
	opcodeTable[0x65] = { [this]() { ADC(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0x75] = { [this]() { ADC(FetchZeroPageX()); }, 2, 4 };
	opcodeTable[0x6D] = { [this]() { ADC(FetchAbsolute()); }, 3, 4 };
	opcodeTable[0x7D] = { [this]() { ADC(FetchAbsoluteX()); }, 3, 4 };
	opcodeTable[0x79] = { [this]() { ADC(FetchAbsoluteY()); }, 3, 4 };
	opcodeTable[0x61] = { [this]() { ADC(FetchIndirectX()); }, 2, 6 };
	opcodeTable[0x71] = { [this]() { ADC(FetchIndirectY()); }, 2, 5 };
	opcodeTable[0xE9] = { [this]() { SBC(FetchImmediate()); }, 2, 2 };
	opcodeTable[0xE5] = { [this]() { SBC(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0xF5] = { [this]() { SBC(FetchZeroPageX()); }, 2, 4 };
	opcodeTable[0xED] = { [this]() { SBC(FetchAbsolute()); }, 3, 4 };
	opcodeTable[0xFD] = { [this]() { SBC(FetchAbsoluteX()); }, 3, 4 };
	opcodeTable[0xF9] = { [this]() { SBC(FetchAbsoluteY()); }, 3, 4 };
	opcodeTable[0xE1] = { [this]() { SBC(FetchIndirectX()); }, 2, 6 };
	opcodeTable[0xF1] = { [this]() { SBC(FetchIndirectY()); }, 2, 5 };

	// Logic
	opcodeTable[0x29] = { [this]() { AND(FetchImmediate()); }, 2, 2 };
	opcodeTable[0x25] = { [this]() { AND(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0x35] = { [this]() { AND(FetchZeroPageX()); }, 2, 4 };
	opcodeTable[0x2D] = { [this]() { AND(FetchAbsolute()); }, 3, 4 };
	opcodeTable[0x3D] = { [this]() { AND(FetchAbsoluteX()); }, 3, 4 };
	opcodeTable[0x39] = { [this]() { AND(FetchAbsoluteY()); }, 3, 4 };
	opcodeTable[0x21] = { [this]() { AND(FetchIndirectX()); }, 2, 6 };
	opcodeTable[0x31] = { [this]() { AND(FetchIndirectY()); }, 2, 5 };
	opcodeTable[0x49] = { [this]() { EOR(FetchImmediate()); }, 2, 2 };
	opcodeTable[0x45] = { [this]() { EOR(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0x55] = { [this]() { EOR(FetchZeroPageX()); }, 2, 4 };
	opcodeTable[0x4D] = { [this]() { EOR(FetchAbsolute()); }, 3, 4 };
	opcodeTable[0x5D] = { [this]() { EOR(FetchAbsoluteX()); }, 3, 4 };
	opcodeTable[0x59] = { [this]() { EOR(FetchAbsoluteY()); }, 3, 4 };
	opcodeTable[0x41] = { [this]() { EOR(FetchIndirectX()); }, 2, 6 };
	opcodeTable[0x51] = { [this]() { EOR(FetchIndirectY()); }, 2, 5 };
	opcodeTable[0x09] = { [this]() { ORA(FetchImmediate()); }, 2, 2 };
	opcodeTable[0x05] = { [this]() { ORA(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0x15] = { [this]() { ORA(FetchZeroPageX()); }, 2, 4 };
	opcodeTable[0x0D] = { [this]() { ORA(FetchAbsolute()); }, 3, 4 };
	opcodeTable[0x1D] = { [this]() { ORA(FetchAbsoluteX()); }, 3, 4 };
	opcodeTable[0x19] = { [this]() { ORA(FetchAbsoluteY()); }, 3, 4 };
	opcodeTable[0x01] = { [this]() { ORA(FetchIndirectX()); }, 2, 6 };
	opcodeTable[0x11] = { [this]() { ORA(FetchIndirectY()); }, 2, 5 };

	// Bit Test
	opcodeTable[0x24] = { [this]() { BIT(FetchZeroPage()); }, 2, 3 };
	opcodeTable[0x2C] = { [this]() { BIT(FetchAbsolute()); }, 3, 4 };

	// Increments / Decrements
	opcodeTable[0xE8] = { [this]() { INX(); }, 1, 2 };
	opcodeTable[0xCA] = { [this]() { DEX(); }, 1, 2 };
	opcodeTable[0xC8] = { [this]() { INY(); }, 1, 2 };
	opcodeTable[0x88] = { [this]() { DEY(); }, 1, 2 };
	opcodeTable[0xE6] = { [this]() { INC(GetZeroPageAddress()); }, 2, 5 };
	opcodeTable[0xF6] = { [this]() { INC(GetZeroPageAddressX()); }, 2, 6 };
	opcodeTable[0xEE] = { [this]() { INC(GetAddressAbsolute()); }, 3, 6 };
	opcodeTable[0xFE] = { [this]() { INC(GetAddressAbsoluteX()); }, 3, 7 };
	opcodeTable[0xC6] = { [this]() { DEC(GetZeroPageAddress()); }, 2, 5 };
	opcodeTable[0xD6] = { [this]() { DEC(GetZeroPageAddressX()); }, 2, 6 };
	opcodeTable[0xCE] = { [this]() { DEC(GetAddressAbsolute()); }, 3, 6 };
	opcodeTable[0xDE] = { [this]() { DEC(GetAddressAbsoluteX()); }, 3, 7 };

	// Shifts & Rotates
	opcodeTable[0x0A] = { [this]() { ASL_A(); }, 1, 2 };
	opcodeTable[0x06] = { [this]() { ASL(GetZeroPageAddress()); }, 2, 5 };
	opcodeTable[0x16] = { [this]() { ASL(GetZeroPageAddressX()); }, 2, 6 };
	opcodeTable[0x0E] = { [this]() { ASL(GetAddressAbsolute()); }, 3, 6 };
	opcodeTable[0x1E] = { [this]() { ASL(GetAddressAbsoluteX()); }, 3, 7 };
	opcodeTable[0x4A] = { [this]() { LSR_A(); }, 1, 2 };
	opcodeTable[0x46] = { [this]() { LSR(GetZeroPageAddress()); }, 2, 5 };
	opcodeTable[0x56] = { [this]() { LSR(GetZeroPageAddressX()); }, 2, 6 };
	opcodeTable[0x4E] = { [this]() { LSR(GetAddressAbsolute()); }, 3, 6 };
	opcodeTable[0x5E] = { [this]() { LSR(GetAddressAbsoluteX()); }, 3, 7 };
	opcodeTable[0x2A] = { [this]() { ROL_A(); }, 1, 2 };
	opcodeTable[0x26] = { [this]() { ROL(GetZeroPageAddress()); }, 2, 5 };
	opcodeTable[0x36] = { [this]() { ROL(GetZeroPageAddressX()); }, 2, 6 };
	opcodeTable[0x2E] = { [this]() { ROL(GetAddressAbsolute()); }, 3, 6 };
	opcodeTable[0x3E] = { [this]() { ROL(GetAddressAbsoluteX()); }, 3, 7 };
	opcodeTable[0x6A] = { [this]() { ROR_A(); }, 1, 2 };
	opcodeTable[0x66] = { [this]() { ROR(GetZeroPageAddress()); }, 2, 5 };
	opcodeTable[0x76] = { [this]() { ROR(GetZeroPageAddressX()); }, 2, 6 };
	opcodeTable[0x6E] = { [this]() { ROR(GetAddressAbsolute()); }, 3, 6 };
	opcodeTable[0x7E] = { [this]() { ROR(GetAddressAbsoluteX()); }, 3, 7 };
	opcodeTable[0xEA] = { [this]() { NOP(); },1,2 };


}
	


//Carry
//Zero
//Interrupt
//Decimal
//Break
//Unused
//Overflow
//Negative
//N V - B D I Z C
//7 6 5 4 3 2 1 0
//Set P flag bits
// Register Check Functions
void CPU::SetCarry() {
	P |= 0b00000001;
}
void CPU::SetZero() {
	P |= 0b00000010;
}
void CPU::SetInterrupt() {
	P |= 0b00000100;
}
void CPU::SetDecimal() {
	P |= 0b00001000;
}
void CPU::SetBreak() {
	P |= 0b00010000;
}
void CPU::SetOverflow() {
	P |= 0b01000000;
}
void CPU::SetNegative() {
	P |= 0b10000000;
}
void CPU::SetUnused() {
	P |= 0b00100000;
}
void CPU::ClearCarry() { P &= ~0b00000001; }
void CPU::ClearZero() { P &= ~0b00000010; }
void CPU::ClearInterrupt() { P &= ~0b00000100; }
void CPU::ClearDecimal() { P &= ~0b00001000; }
void CPU::ClearBreak() { P &= ~0b00010000; }
void CPU::ClearUnused() { P &= ~0b00100000; }
void CPU::ClearOverflow() { P &= ~0b01000000; }
void CPU::ClearNegative() { P &= ~0b10000000; }
//True Set /False Clear
bool CPU::GetCarry() {
	return (P & 0b00000001) !=0;
}
bool CPU::GetZero() {
	return (P & 0b00000010) != 0;
}
bool CPU::GetInterrupt() {
	return (P & 0b00000100) != 0;
}
bool CPU::GetDecimal() {
	return(P & 0b00001000) != 0;
}
bool CPU::GetBreak() {
	return (P & 0b00010000) != 0;
}
bool CPU::GetOverflow() {
	return (P & 0b01000000) != 0;
}
bool CPU::GetNegative() {
	return (P & 0b10000000) != 0;
}
bool CPU::GetUnused() {
	return (P & 0b00100000) !=0;
}
void CPU::Reset() {
	//nesdev.org/wiki/CPU_power_up_state
	//Interrupt set to 1, AXY set to 0, FFFC = 00 and FFFD = x80.S starts at xFD
	//bus->cpuWrite(0xFFFC,0x00);
	//bus->cpuWrite(0xFFFD, 0x80);
	uint8_t lo = bus->cpuRead(0xFFFC);
	uint8_t hi = bus->cpuRead(0xFFFD);
	PC = (hi << 8) | lo;
	A = 0;
	Y = 0;
	X = 0;
	S = 0xFD;
	P = 0b00100100;
	std::printf("cpuRead reset vector: $%04X (lo=$%02X hi=$%02X)\n", PC, lo, hi);
	std::printf("A=$%02X X=$%02X Y=$%02X S=$%02X P=$%02X\n", A, X, Y, S, P);
	cycles = 7;
}
void CPU::clock() {
	if (cycles == 0) {
		uint8_t opcode = bus->cpuRead(PC);
		//std::cout << "PC: $" << std::hex << std::uppercase << (int)PC
		//	<< "  OPCODE: $" << std::setw(2) << std::setfill('0') << (int)opcode << std::endl;
		//std::cout << "PC=$" << std::setw(4) << PC << " OPCODE=$" << std::setw(2) << (int)opcode << "\n";


		const auto& op = opcodeTable[opcode];

		cycles = op.cycles;

		op.handler(); 
		PC += op.bytes;

	}
	clockCycles++;
	cycles--;
}
void CPU::irq() {
	if (!GetInterrupt()) {
		bus->cpuWrite(0x0100 + S,  (PC >> 8) & 0x00FF);
		S--;
		bus->cpuWrite(0x0100 + S, PC & 0x00FF);
		S--;
		ClearBreak();
		SetUnused();
		SetInterrupt();
		bus->cpuWrite(0x0100 + S, P);
		S--;
		uint16_t lo = bus->cpuRead(0xFFFA);      // returns 0x34
		uint16_t hi = bus->cpuRead(0xFFFB);      // returns 0x12
		PC = (hi << 8) | lo;                  // PC = 0x1234
		PC = (hi << 8) | lo;
		cycles = 7;
	}
}
	
void CPU::nmi() {
	bus->cpuWrite(0x0100 + S, (PC >> 8) & 0x00FF);
	S--;
	bus->cpuWrite(0x0100 + S, PC & 0x00FF);
	S--;
	ClearBreak();
	SetUnused();
	SetInterrupt();
	bus->cpuWrite(0x0100 + S, P);
	S--;
	uint16_t lo = bus->cpuRead(0xFFFA);      // e.g. returns 0x34
	uint16_t hi = bus->cpuRead(0xFFFB);      // e.g. returns 0x12
	PC = (hi << 8) | lo;                  // PC = 0x1234
	PC = (hi << 8) | lo;
	cycles = 8;
	}

void CPU::Push(uint8_t value) {
	bus->cpuWrite(0x0100 + S, value);
	S--;
}

uint8_t CPU::Pop() {
	S++;
	return bus->cpuRead(0x0100 + S);
}

bool CPU::complete() {
    return cycles == 0;
}

int main() {

}