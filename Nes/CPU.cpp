#include <iostream>
#include "cpu.h"
#include "Bus.h"
#include "cartridge.h"

cpu::cpu() {
	InitopcodeTable();
}


void cpu::ConnectBus(Bus* b) {
	bus = b;
	}



//Adressing modes

uint8_t cpu::FetchImmediate() {
	return bus->Read(PC+ 1);

}

uint8_t cpu::FetchZeroPage() {
	uint8_t addr = bus->Read(PC + 1);
	return bus->Read(addr);
}

uint8_t cpu::FetchZeroPageX() {
	uint8_t base = bus->Read(PC + 1);//get operand
	uint8_t address = (base + X) & 0xFF; // get nex address +X
	return bus->Read(address); // Store in A

}

uint8_t cpu::FetchZeroPageY() {
	uint8_t base = bus->Read(PC + 1);//get operand
	uint8_t address = (base + Y) & 0xFF; // get nex address +X
	return bus->Read(address); // Store in A

}
uint8_t cpu::FetchAbsolute() {
	uint16_t address = bus->Read(PC + 1) | (bus->Read(PC + 2) << 8);
	return bus->Read(address);
}
uint8_t cpu::FetchAbsoluteX() {
	uint16_t base = (bus->Read(PC + 1) | (bus->Read(PC + 2) << 8));
	uint16_t address = base + X;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		cycles++;
	}
	return bus->Read(address);
}
uint8_t cpu::FetchAbsoluteY() {
	uint16_t base = (bus->Read(PC + 1) | (bus->Read(PC + 2) << 8));
	uint16_t address = base + Y;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		cycles++;
	}
	return bus->Read(address);
}
uint8_t cpu::FetchIndirectX() {
	uint8_t zp_addr = (bus->Read(PC + 1) + X) & 0xFF; // wrap around
	uint8_t low = bus->Read(zp_addr);
	uint8_t high = bus->Read((zp_addr + 1) & 0xFF); // wrap again
	uint16_t address = (high << 8) | low;
	return bus->Read(address);
}
uint8_t cpu::FetchIndirectY() {
	uint8_t zptr = bus->Read(PC + 1);

	uint16_t base = bus->Read(zptr) | (bus->Read((zptr + 1) & 0xFF) << 8);

	uint16_t address = base + Y;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		cycles++;
	}
	return bus->Read(address);
}

uint16_t cpu::StoreZeroPage() {
	return bus->Read(PC + 1);
}

uint16_t cpu::StoreZeroPageX() {
	return (bus->Read(PC + 1) + X) & 0xFF;
}
uint16_t cpu::StoreZeroPageY() {
	return (bus->Read(PC + 1) + Y) & 0xFF;
}
uint16_t cpu::StoreAbsolute() {
	return (bus->Read(PC + 1) | (bus->Read(PC + 2) << 8));

}
uint16_t cpu::StoreAbsoluteX() {
	return ((bus->Read(PC + 1) | (bus->Read(PC + 2) << 8)) +X);

}
uint16_t cpu::StoreAbsoluteY() {
	return ((bus->Read(PC + 1) | (bus->Read(PC + 2) << 8)) + Y);

}
uint16_t cpu::StoreIndirectX() {
	uint8_t index = (bus->Read(PC + 1) + X) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
	uint16_t addr = (bus->Read((index + 1) & 0xFF) << 8) | bus->Read(index);
	return addr;

}
uint16_t cpu::StoreIndirectY() {
	uint8_t index = (bus->Read(PC + 1) + Y) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
	uint16_t addr = (bus->Read((index + 1) & 0xFF) << 8) | bus->Read(index);
	return addr;

}
//LDA
//LDA cycle cost 

void cpu::LDA(uint8_t value) {
	//Opcode then operand 2 bytes and 2 cycles
	A = value;
	CheckByte(A);
}


//STA
void cpu::STA(uint16_t value) {
	//opcode then a memory location to store A

	bus->Write(value,A);// store A in it


}


void cpu::STX(uint16_t value) {

	//Store X into memory location
	bus->Write(value,X);

}


void cpu::STY(uint16_t value) {

	//Store Y into memory location
	bus->Write(value,Y);

}

//LDX
void cpu::LDX(uint8_t value) {
	//Opcode then operand 2 bytes and 2 cycles
	X = value;
	CheckByte(X);

}


void cpu::LDY(uint8_t value) {
	//Opcode then operand 2 bytes and 2 cycles
	Y = value;
	CheckByte(Y);


}

void cpu::PHA() {

	bus->Write(0x0100 + S, A);
	S--;
	PC += 1;
	cycles += 3;

}
void cpu::PLA() {

	A = bus->Read(0x0100 + S);
	S++;
	CheckByte(A);
	PC += 1;
	cycles += 4;

}
void cpu::PHP() {
	bus->Write(0x0100 + S,  P);
	S--;


}
void cpu::PLP() {
	P = bus->Read(0x0100 + S);
	S++;

}

void cpu::TAX() {
	// Dont need the memory access but itll make the loop simplier
	X = A;
	CheckByte(X);

}

void cpu::TAY() {
	// Dont need the memory access but itll make the loop simplier

	Y = A;
	CheckByte(Y);

}

void cpu::TXA() {
	// Dont need the memory access but itll make the loop simplier

	A = X;
	CheckByte(A);

}

void cpu::TYA() {
	// Dont need the memory access but itll make the loop simplier

	A = Y;
	CheckByte(A);

}
void cpu::INX() {
	// Dont need the memory access but itll make the loop simplier

	X++;
	CheckByte(X);

}
void cpu::INY() {
	// Dont need the memory access but itll make the loop simplier

	Y++;
	CheckByte(Y);

}

void cpu::DEX() {
	// Dont need the memory access but itll make the loop simplier

	X--;
	CheckByte(X);

}
void cpu::DEY() {
	// Dont need the memory access but itll make the loop simplier

	Y--;
	CheckByte(Y);

}
void cpu::JSR(uint16_t addr) {

    // Correct return address: PC of the instruction AFTER JSR, minus 1
    uint16_t return_address = PC + 2;

    bus->Write(0x0100 + S, (return_address >> 8) & 0xFF); // high byte
    S--;
    bus->Write(0x0100 + S, return_address & 0xFF);        // low byte
    S--;

    PC = addr;


}
void cpu::RTS() {
	//Do not understand this one very well

	S++;
	uint8_t low = bus->Read(0x0100 + S);
	S++;
	uint8_t high = bus->Read(0x0100 + S);
	S++;
	PC = high << 8 | low;


}
void cpu::BRK() {
	//Do not understand this one very well

	PC++;
	bus->Write((0x0100 +S) , (PC >>8));
	S--;
	bus->Write((0x0100 + S), (PC & 0xFF));
	S--;

	bus->Write((0x0100 + S),( P | 0x30));
	S--;
	SetInterrupt(); //Flag
		
	PC = (bus->Read(0xFFFF) << 8 ) | bus->Read(0xFFFE);
	cycles += 7;
}
void cpu::RTI() {
	// Set P to current stack value
	P = bus->Read(0x0100 + S);
	S++; // Push
	uint8_t low = bus->Read((0x0100 + S) ); // get low byte
	S++;
	uint8_t high = bus->Read(0x0100 + S ); // get high byte
	S++;
	PC = high << 8 | low; //Bit shift to get 16bit
	cycles += 6;

}
void cpu::ADC(uint8_t value) {
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

void cpu::SBC(uint8_t value) {
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
void cpu::AND(uint8_t value) {
	A = A & value;

	CheckByte(A);
}
void cpu::ASL(uint16_t addr) {
	//Check bit 7 before bit shift to see  value for carry

	uint8_t value = bus->Read(addr);
	if (value & 0b10000000) {
		SetCarry();
	}
	else { ClearCarry(); }
	bus->Write(addr, value <<= 1);
	CheckByte(value);


}
void cpu::ASL_A() {
	//Check bit 7 before bit shift to see  value for carry
	if (A & 0b10000000) {
		SetCarry();
	}
	else { ClearCarry(); }
	A = A << 1;
	CheckByte(A);
}

void cpu::BCC() {
	int8_t offset = (int8_t)(bus->Read(PC + 1));



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
void cpu::BCS() {
	int8_t offset = (int8_t)(bus->Read(PC + 1));



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
void cpu::BEQ() {
	int8_t offset = (int8_t)(bus->Read(PC + 1));

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
void cpu::BIT(uint8_t value) {
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

void cpu::BMI() {
	int8_t offset = (int8_t)(bus->Read(PC + 1));

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
void cpu::BNE() {
	int8_t offset = (int8_t)(bus->Read(PC + 1));

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
void cpu::BPL() {
	int8_t offset = (int8_t)(bus->Read(PC + 1));

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
void cpu::BVC() {
	int8_t offset = (int8_t)(bus->Read(PC + 1));

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
void cpu::BVS() {
	int8_t offset = (int8_t)(bus->Read(PC + 1));

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


void cpu::CLC() {
	ClearCarry();
}

void cpu::CLD() {
	ClearDecimal();
}

void cpu::CLI() {
	ClearInterrupt();
}
void cpu::CLV() {
	ClearOverflow();
}

void cpu::CMP(uint8_t value) {
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

void cpu::CPX(uint8_t value) {
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

void cpu::CPY(uint8_t value) {
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

void cpu::DEC(uint16_t addr) {
	uint8_t value = bus->Read(addr);
	value -= 1;
	bus->Write(addr, value);
	CheckByte(value);

}


void cpu::EOR(uint8_t value) {
	A = A ^ value;

	CheckByte(A);
}

void cpu::INC(uint16_t addr) {
	uint8_t value = bus->Read(addr);
	value += 1;
	bus->Write(addr, value);
	CheckByte(value);

}
void cpu::JMP(uint16_t addr) {
	PC = addr;

}
void cpu::JMPIndirect() {
	uint8_t low = bus->Read(PC + 1);
	uint8_t high = bus->Read(PC + 2);
	uint16_t pointer = (high << 8) | low;

	uint8_t target_low = bus->Read(pointer);
	uint8_t target_high;

	// Buggy wrap around
	if ((pointer & 0x00FF) == 0x00FF)
		target_high = bus->Read(pointer & 0xFF00);  // wrap around to start of page
	else
		target_high = bus->Read(pointer + 1);

	PC = (target_high << 8) | target_low;

}
void cpu::LSR_A() {
	if (A & 0b00000001) { SetCarry(); }
	else
	{
		ClearCarry();
	}
	A >>= 1;

	CheckByte(A);
}
void cpu::LSR(uint16_t addr) {
	uint8_t value= bus->Read(addr);
	if (value & 0b00000001) { SetCarry(); }
	else
	{
		ClearCarry();
	}
	value >>= 1;
	bus->Write(addr, value);
	CheckByte(value);

}
void cpu::NOP() {
	//Do nothing
}
void cpu::ORA(uint8_t value) {
	A = A | value;

	CheckByte(A);
}
void cpu::ROL(uint16_t addr) {
	uint8_t value = bus->Read(addr);
	bool oldCarry = GetCarry();
	if (value & 0b10000000) SetCarry(); else ClearCarry();
	value <<= 1;
	if (oldCarry) {
		value |= 0b00000001;
	};

	CheckByte(value);
	bus->Write(addr, value);

}
void cpu::ROL_A() {
	bool oldCarry = GetCarry();

	if (A & 0b10000000) SetCarry(); else ClearCarry();
	A <<= 1;
	if (oldCarry) {
		A |= 0b00000001;
	}
	CheckByte(A);
}

void cpu::ROR_A() {

	bool oldCarry = GetCarry();
	if (A & 0b00000001) SetCarry(); else ClearCarry();
	A >>= 1;
	if (oldCarry) {
		A |= 0b10000000;
	};
	CheckByte(A);
}

void cpu::ROR(uint16_t addr) {
	uint8_t value = bus->Read(addr);
	bool oldCarry = GetCarry();
	if (value & 0b00000001) SetCarry(); else ClearCarry();
	value >>= 1;
	if (oldCarry) {
		value |= 0b10000000;
	};

	CheckByte(value);
	bus->Write(addr, value);
}
void cpu::SEC() {
	SetCarry();
}
void cpu::SED() {
	SetDecimal();
}
void cpu::SEI() {
	SetInterrupt();
}
void cpu::TSX() {
	X = S;
}
void cpu::TXS() {
	S = X;
}
//Register check Function for negaive and 0
void cpu::CheckByte(uint8_t reg) {
	if (reg == 0) { SetZero(); }
	else { ClearZero(); }
	if ((reg & 0b10000000) != 0) { SetNegative(); }
	else { ClearNegative(); }

}

void cpu::InitopcodeTable() {

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
	opcodeTable[0x85] = { [this]() { STA(StoreZeroPage()); }, 2, 3 };
	opcodeTable[0x95] = { [this]() { STA(StoreZeroPageX()); }, 2, 4 };
	opcodeTable[0x8D] = { [this]() { STA(StoreAbsolute()); }, 3, 4 };
	opcodeTable[0x9D] = { [this]() { STA(StoreAbsoluteX()); }, 3, 5 };
	opcodeTable[0x99] = { [this]() { STA(StoreAbsoluteY()); }, 3, 5 };
	opcodeTable[0x81] = { [this]() { STA(StoreIndirectX()); }, 2, 6 };
	opcodeTable[0x91] = { [this]() { STA(StoreIndirectY()); }, 2, 6 };

	// STX – Store X Register
	opcodeTable[0x86] = { [this]() { STX(StoreZeroPage()); }, 2, 3 };
	opcodeTable[0x96] = { [this]() { STX(StoreZeroPageY()); }, 2, 4 };
	opcodeTable[0x8E] = { [this]() { STX(StoreAbsolute()); }, 3, 4 };

	// STY – Store Y Register
	opcodeTable[0x84] = { [this]() { STY(StoreZeroPage()); }, 2, 3 };
	opcodeTable[0x94] = { [this]() { STY(StoreZeroPageX()); }, 2, 4 };
	opcodeTable[0x8C] = { [this]() { STY(StoreAbsolute()); }, 3, 4 };

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
	opcodeTable[0x28] = { [this]() { PLP(); }, 1, 3 };

	// Control Flow
	opcodeTable[0x20] = { [this]() { JSR(FetchAbsolute()); }, 3, 6 };
	opcodeTable[0x60] = { [this]() { RTS(); }, 1, 6 };
	opcodeTable[0x00] = { [this]() { BRK(); }, 1, 7 };
	opcodeTable[0x40] = { [this]() { RTI(); }, 1, 6 };
	opcodeTable[0x4C] = { [this]() { JMP(FetchAbsolute()); }, 3, 3 };
	opcodeTable[0x6C] = { [this]() { JMPIndirect(); }, 3, 5 };

	// Branches
	opcodeTable[0x90] = { [this]() { BCC(); }, 2, 2 };
	opcodeTable[0xB0] = { [this]() { BCS(); }, 2, 2 };
	opcodeTable[0xF0] = { [this]() { BEQ(); }, 2, 2 };
	opcodeTable[0x30] = { [this]() { BMI(); }, 2, 2 };
	opcodeTable[0xD0] = { [this]() { BNE(); }, 2, 2 };
	opcodeTable[0x10] = { [this]() { BPL(); }, 2, 2 };
	opcodeTable[0x50] = { [this]() { BVC(); }, 2, 2 };
	opcodeTable[0x70] = { [this]() { BVS(); }, 2, 2 };

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
	opcodeTable[0xE6] = { [this]() { INC(FetchZeroPage()); }, 2, 5 };
	opcodeTable[0xF6] = { [this]() { INC(FetchZeroPageX()); }, 2, 6 };
	opcodeTable[0xEE] = { [this]() { INC(FetchAbsolute()); }, 3, 6 };
	opcodeTable[0xFE] = { [this]() { INC(FetchAbsoluteX()); }, 3, 7 };
	opcodeTable[0xC6] = { [this]() { DEC(FetchZeroPage()); }, 2, 5 };
	opcodeTable[0xD6] = { [this]() { DEC(FetchZeroPageX()); }, 2, 6 };
	opcodeTable[0xCE] = { [this]() { DEC(FetchAbsolute()); }, 3, 6 };
	opcodeTable[0xDE] = { [this]() { DEC(FetchAbsoluteX()); }, 3, 7 };

	// Shifts & Rotates
	opcodeTable[0x0A] = { [this]() { ASL_A(); }, 1, 2 };
	opcodeTable[0x06] = { [this]() { ASL(StoreZeroPage()); }, 2, 5 };
	opcodeTable[0x16] = { [this]() { ASL(StoreZeroPageX()); }, 2, 6 };
	opcodeTable[0x0E] = { [this]() { ASL(StoreAbsolute()); }, 3, 6 };
	opcodeTable[0x1E] = { [this]() { ASL(StoreAbsoluteX()); }, 3, 7 };
	opcodeTable[0x4A] = { [this]() { LSR_A(); }, 1, 2 };
	opcodeTable[0x46] = { [this]() { LSR(StoreZeroPage()); }, 2, 5 };
	opcodeTable[0x56] = { [this]() { LSR(StoreZeroPageX()); }, 2, 6 };
	opcodeTable[0x4E] = { [this]() { LSR(StoreAbsolute()); }, 3, 6 };
	opcodeTable[0x5E] = { [this]() { LSR(StoreAbsoluteX()); }, 3, 7 };
	opcodeTable[0x2A] = { [this]() { ROL_A(); }, 1, 2 };
	opcodeTable[0x26] = { [this]() { ROL(StoreZeroPage()); }, 2, 5 };


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
void cpu::SetCarry() {
	P |= 0b00000001;
}
void cpu::SetZero() {
	P |= 0b00000010;
}
void cpu::SetInterrupt() {
	P |= 0b00000100;
}
void cpu::SetDecimal() {
	P |= 0b00001000;
}
void cpu::SetBreak() {
	P |= 0b00010000;
}
void cpu::SetOverflow() {
	P |= 0b01000000;
}
void cpu::SetNegative() {
	P |= 0b10000000;
}
void cpu::SetUnused() {
	P |= 0b00100000;
}
void cpu::ClearCarry() { P &= ~0b00000001; }
void cpu::ClearZero() { P &= ~0b00000010; }
void cpu::ClearInterrupt() { P &= ~0b00000100; }
void cpu::ClearDecimal() { P &= ~0b00001000; }
void cpu::ClearBreak() { P &= ~0b00010000; }
void cpu::ClearUnused() { P &= ~0b00100000; }
void cpu::ClearOverflow() { P &= ~0b01000000; }
void cpu::ClearNegative() { P &= ~0b10000000; }
//True Set /False Clear
bool cpu::GetCarry() {
	return (P & 0b00000001) !=0;
}
bool cpu::GetZero() {
	return (P & 0b00000010) != 0;
}
bool cpu::GetInterrupt() {
	return (P & 0b00000100) != 0;
}
bool cpu::GetDecimal() {
	return(P & 0b00001000) != 0;
}
bool cpu::GetBreak() {
	return (P & 0b00010000) != 0;
}
bool cpu::GetOverflow() {
	return (P & 0b01000000) != 0;
}
bool cpu::GetNegative() {
	return (P & 0b10000000) != 0;
}
bool cpu::GetUnused() {
	return (P & 0b00100000) !=0;
}
void cpu::Reset() {
	//nesdev.org/wiki/CPU_power_up_state
	//Interrupt set to 1, AXY set to 0, FFFC = 00 and FFFD = x80.S starts at xFD
	//bus->Write(0xFFFC,0x00);
	//bus->Write(0xFFFD, 0x80);
	PC = bus->Read(0xFFFC)  | bus->Read(0xFFFD) <<8;
	A = 0;
	Y = 0;
	X = 0;
	S = 0xFD;
	P = 0b00100100;
	cycles = 7;
}
void cpu::clock();
void cpu::irq();
void cpu::nmi();




#include <fstream>
#include <iomanip>
#include <sstream>



int main() {
	cpu cpu;
	Bus bus;

	Cartridge cart("nestest.nes");
	if (!cart.valid) {
		std::cerr << "Failed to load cartridge: nestest.nes\n";
		return -1;
	}


}