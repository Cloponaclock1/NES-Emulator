#include <iostream>

class NESCPU {


private:
	uint8_t A, X, Y, S;
	uint8_t P = 0;
	uint16_t PC;



	static uint8_t memory[0x10000]; //memory size 2KB
	int cycles; // cycles counter
	typedef void (NESCPU::* OpcodeTable)(uint8_t*);
	OpcodeTable opcodeTable[256];






	//LDA
	//LDA cycle cost 

	void LDA_Immediate(uint8_t* memory) {
		//Opcode then operand 2 bytes and 2 cycles
		A = memory[PC + 1];
		CheckRegister(A);

		PC += 2;
		cycles += 2;
	}

	void LDA_Zero_Page(uint8_t* memory) {

		uint8_t location = memory[PC + 1]; // get location
		A = memory[location];// store A in it
		CheckRegister(A);

		PC += 2;
		cycles += 3;

	}

	void LDA_Zero_Page_X(uint8_t* memory) {
		//Add X to operand and store A

		uint8_t address = (memory[PC + 1] + X) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
		A = memory[address];
		if (A == 0) { SetZero(); }
		CheckRegister(A);

		PC += 2;
		cycles += 4;

	}

	void LDA_Absolute(uint8_t* memory) {
		//16bit address using pc+2 <<8 + pc+1 bitshift set it to A
		//Bytes 3 cycles 4
		uint16_t address = memory[PC + 2] << 8 | memory[PC + 1];
		A = memory[address];
		CheckRegister(A);

		PC += 3;
		cycles += 4;

	}
	void LDA_Absolute_X(uint8_t* memory) {
		//16bit address  using pc+2 << 8 pc+1 then +x
		//Bytes 3, cycles 5
		// if page crossed + 1 cycle
		uint16_t base = (memory[PC + 2] << 8 | memory[PC + 1]);
		uint16_t address = base + X;
		A = memory[address];
		CheckRegister(A);
		PC += 3;
		if ((base & 0xFF00) != (address & 0xFF00)) {
			cycles++;
		}
		cycles += 4;

	}

	void  LDA_Absolute_Y(uint8_t* memory) {
		//16bit address using pc+2 << 8 pc+1 then +y
		//Bytes 3, cycles 5
		// if page crossed + 1 cycle\

		uint16_t base = (memory[PC + 2] << 8 | memory[PC + 1]);
		uint16_t address = base + Y;
		A = memory[address];
		CheckRegister(A);

		PC += 3;
		if ((base & 0xFF00) != (address & 0xFF00)) {
			cycles++;
		}
		cycles += 4;

	}

	void  LDA_Indirect_X(uint8_t* memory) {
		//
		// Bytes 2, cycles 6
		uint8_t index = (memory[PC + 1] + X) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
		uint16_t address = (memory[(index + 1) & 0xFF] << 8) | memory[index];
		A = memory[address];
		CheckRegister(A);

		PC += 2;
		cycles += 6;
	}
	void  LDA_Indirect_Y(uint8_t* memory) {
		//
		// Bytes 2, cycles 6
			// if page crossed + 1 cycle

		uint8_t zptr = (memory[PC + 1]);
		uint16_t base = memory[zptr] | (memory[(zptr + 1) & 0xFF] << 8);

		uint16_t address = base + Y;
		if ((base & 0xFF00) != (address & 0xFF00)) {
			cycles++;
		}
		A = memory[address];
		CheckRegister(A);

		PC += 2;
		cycles += 5;
	}

	//STA
	void STA_Zero_Page(uint8_t* memory) {
		//opcode then a memory location to store A

		uint8_t location = memory[PC + 1]; // get location
		memory[location] = A;// store A in it
		PC += 2;
		cycles += 3;

	}
	void STA_Zero_Page_X(uint8_t* memory) {
		//Add X to operand and store A

		uint8_t address = (memory[PC + 1] + X) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
		memory[address] = A;
		PC += 2;
		cycles += 3;

	}
	void STA_Absolute(uint8_t* memory) {
		//16bit address using pc+2 <<8 + pc+1 bitshift set it to A
		//Bytes 3 cycles 4
		uint16_t address = memory[PC + 2] << 8 | memory[PC + 1];
		memory[address] = A;
		PC += 3;
		cycles += 4;

	}
	void STA_Absolute_X(uint8_t* memory) {
		//16bit address  using pc+2 << 8 pc+1 then +x
		//Bytes 3, cycles 5
		uint16_t address = (memory[PC + 2] << 8 | memory[PC + 1]) + X;
		memory[address] = A;
		PC += 3;
		cycles += 5;

	}
	void  STA_Absolute_Y(uint8_t* memory) {
		//16bit address using pc+2 << 8 pc+1 then +y
		//Bytes 3, cycles 5
		uint16_t address = (memory[PC + 2] << 8 | memory[PC + 1]) + Y;
		memory[address] = A;
		PC += 3;
		cycles += 5;

	}
	void  STA_Indirect_X(uint8_t* memory) {
		//
		// Bytes 2, cycles 6
		uint8_t index = (memory[PC + 1] + X) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
		uint16_t address = (memory[(index + 1) & 0xFF] << 8) | memory[index];
		memory[address] = A;
		PC += 2;
		cycles += 6;
	}
	void  STA_Indirect_Y(uint8_t* memory) {
		//
		// Bytes 2, cycles 6
		uint8_t index = (memory[PC + 1] + Y) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
		uint16_t address = (memory[(index + 1) & 0xFF] << 8) | memory[index];
		memory[address] = A;
		PC += 2;
		cycles += 6;
	}

	void STX_Zero_Page(uint8_t* memory) {

		//Store X into memory location
		uint8_t location = memory[PC + 1]; // get location
		memory[location] = X;
		PC += 2;
		cycles += 3;
	}
	void STX_Zero_Page_Y(uint8_t* memory) {
		uint8_t address = (memory[PC + 1] + Y) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
		memory[address] = X;
		PC += 2;
		cycles += 4;

	}
	void STX_Absolute(uint8_t* memory) {
		//Bytes 3 cycles 4
		uint16_t address = memory[PC + 2] << 8 | memory[PC + 1];
		memory[address] = X;
		PC += 3;
		cycles += 4;
	}

	void STY_Zero_Page(uint8_t* memory) {

		//Store Y into memory location
		uint8_t location = memory[PC + 1]; // get location
		memory[location] = Y;
		PC += 2;
		cycles += 3;
	}
	void STY_Zero_Page_X(uint8_t* memory) {
		uint8_t address = (memory[PC + 1] + X) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
		memory[address] = Y;
		PC += 2;
		cycles += 4;

	}
	void STY_Absolute(uint8_t* memory) {
		//Bytes 3 cycles 4
		uint16_t address = memory[PC + 2] << 8 | memory[PC + 1];
		memory[address] = Y;
		PC += 3;
		cycles += 4;
	}
	//LDX
	void LDX_Immediate(uint8_t* memory) {
		//Opcode then operand 2 bytes and 2 cycles
		X = memory[PC + 1];
		CheckRegister(X);

		PC += 2;
		cycles += 2;
	}

	void LDX_Zero_Page(uint8_t* memory) {

		uint8_t location = memory[PC + 1]; // get location
		X = memory[location];// store X in it
		CheckRegister(X);

		PC += 2;
		cycles += 3;

	}

	void LDX_Zero_Page_Y(uint8_t* memory) {
		//Add X to operand and store A

		uint8_t address = (memory[PC + 1] + Y) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
		X = memory[address];
		CheckRegister(X);

		PC += 2;
		cycles += 4;

	}

	void LDX_Absolute(uint8_t* memory) {
		//16bit address using pc+2 <<8 + pc+1 bitshift set it to A
		//Bytes 3 cycles 4
		uint16_t address = memory[PC + 2] << 8 | memory[PC + 1];
		X = memory[address];
		CheckRegister(X);

		PC += 3;
		cycles += 4;

	}
	void LDX_Absolute_Y(uint8_t* memory) {
		//16bit address  using pc+2 << 8 pc+1 then +x
		//Bytes 3, cycles 5
		// if page crossed + 1 cycle
		uint16_t base = (memory[PC + 2] << 8 | memory[PC + 1]);
		uint16_t address = base + Y;
		X = memory[address];
		CheckRegister(X);

		PC += 3;
		if ((base & 0xFF00) != (address & 0xFF00)) {
			cycles++;
		}
		cycles += 4;

	}

	void LDY_Immediate(uint8_t* memory) {
		//Opcode then operand 2 bytes and 2 cycles
		Y = memory[PC + 1];
		CheckRegister(Y);

		PC += 2;
		cycles += 2;
	}

	void LDY_Zero_Page(uint8_t* memory) {

		uint8_t location = memory[PC + 1]; // get location
		Y = memory[location];// store X in it
		CheckRegister(Y);

		PC += 2;
		cycles += 3;

	}

	void LDY_Zero_Page_X(uint8_t* memory) {
		//Add X to operand and store A

		uint8_t address = (memory[PC + 1] + X) & 0xFF; // 0xFF to wrap aroufnt the 256kb memory
		Y = memory[address];
		CheckRegister(Y);

		PC += 2;
		cycles += 4;

	}

	void LDY_Absolute(uint8_t* memory) {
		//16bit address using pc+2 <<8 + pc+1 bitshift set it to A
		//Bytes 3 cycles 4
		uint16_t address = memory[PC + 2] << 8 | memory[PC + 1];
		Y = memory[address];
		CheckRegister(Y);

		PC += 3;
		cycles += 4;

	}
	void LDY_Absolute_X(uint8_t* memory) {
		//16bit address  using pc+2 << 8 pc+1 then +x
		//Bytes 3, cycles 5
		// if page crossed + 1 cycle
		uint16_t base = (memory[PC + 2] << 8 | memory[PC + 1]);
		uint16_t address = base + X;
		Y = memory[address];
		CheckRegister(Y);

		PC += 3;
		if ((base & 0xFF00) != (address & 0xFF00)) {
			cycles++;
		}
		cycles += 4;

	}

	void PHA_Implied(uint8_t* memory ) {

		memory[0x0100 + S] = A;
		S--;
		PC += 1;
		cycles += 3;

	}
	void PLA_Implied(uint8_t* memory) {

		A = memory[0x0100 + S];
		S++;
		CheckRegister(A);
		PC += 1;
		cycles += 4;

	}
	void PHP_Implied(uint8_t* memory) {
		memory[0x0100 + S] = P;
		S--;
		PC += 1;
		cycles += 3;

	}
	void PLP_Implied(uint8_t* memory) {
		P = memory[0x0100 + S];
		S++;
		PC += 1;
		cycles += 4;
	}

	void TAX_Implied(uint8_t* memory) {
		// Dont need the memory access but itll make the loop simplier
		X = A;
		CheckRegister(X);
		PC += 1;
		cycles += 2;
	}

	void TAY_Implied(uint8_t* memory) {
		// Dont need the memory access but itll make the loop simplier

		Y = A;
		CheckRegister(Y);
		PC += 1;
		cycles += 2;
	}

	void TXA_Implied(uint8_t* memory) {
		// Dont need the memory access but itll make the loop simplier

		A = X;
		CheckRegister(A);
		PC += 1;
		cycles += 2;
	}

	void TYA_Implied(uint8_t* memory) {
		// Dont need the memory access but itll make the loop simplier

		A = Y;
		CheckRegister(A);
		PC += 1;
		cycles += 2;
	}
	void INX_Implied(uint8_t* memory) {
		// Dont need the memory access but itll make the loop simplier

		X++;
		CheckRegister(X);
		PC += 1;
		cycles += 2;
	}
	void INY_Implied(uint8_t* memory) {
		// Dont need the memory access but itll make the loop simplier

		Y++;
		CheckRegister(Y);
		PC += 1;
		cycles += 2;
	}

	void DEX_Implied(uint8_t* memory) {
		// Dont need the memory access but itll make the loop simplier

		X--;
		CheckRegister(X);
		PC += 1;
		cycles += 2;
	}
	void DEY_Implied(uint8_t* memory) {
		// Dont need the memory access but itll make the loop simplier

		Y--;
		CheckRegister(Y);
		PC += 1;
		cycles += 2;
	}
	void JSR_Implied(uint8_t* memory) {
		//Do not understand this one very well
		uint16_t target_address = (memory[PC + 2] << 8 | memory[PC + 1]);
		uint16_t return_address = PC + 2;
		memory[0x0100 + S] = (return_address >> 8);
		S--;
		memory[0x0100 + S] = (return_address & 0xFF);
		S--;
		PC = target_address;
		cycles += 6;
		
	}
	void RTS_Implied(uint8_t* memory) {
		//Do not understand this one very well

		S++;
		uint8_t low = memory[0x0100 + S];
		S++;
		uint8_t high = memory[0x0100 + S];
		PC = high << 8 | low;
		PC += 1;
		cycles += 6;

	}
	void BRK_Implied(uint8_t* memory) {
		//Do not understand this one very well

		PC++;
		memory[0x0100 +S] = (PC >>8);
		S--;
		memory[0x0100 + S] = (PC & 0xFF);
		S--;

		memory[0x0100 + S] = P | 0x30;
		S--;
		SetInterrupt();
		
		PC = (memory[0xFFFF] << 8 ) | memory[0xFFFE];
		cycles += 7;
	}


	//Register check Function for negaive and 0
	void CheckRegister(uint8_t reg) {
		if (reg == 0) { SetZero(); }
		else { ClearZero(); }
		if ((reg & 0b10000000) != 0) { SetNegative(); }
		else { ClearNegative(); }

	}
	
	


	void InitopcodeTable() {
		//LDA
// LDA – Load Accumulator
		opcodeTable[0xA9] = &NESCPU::LDA_Immediate;      // LDA #immediate
		opcodeTable[0xA5] = &NESCPU::LDA_Zero_Page;      // LDA zp
		opcodeTable[0xB5] = &NESCPU::LDA_Zero_Page_X;    // LDA zp,X
		opcodeTable[0xAD] = &NESCPU::LDA_Absolute;       // LDA abs
		opcodeTable[0xBD] = &NESCPU::LDA_Absolute_X;     // LDA abs,X
		opcodeTable[0xB9] = &NESCPU::LDA_Absolute_Y;     // LDA abs,Y
		opcodeTable[0xA1] = &NESCPU::LDA_Indirect_X;     // LDA (zp,X)
		opcodeTable[0xB1] = &NESCPU::LDA_Indirect_Y;     // LDA (zp),Y

		// STA – Store Accumulator
		opcodeTable[0x85] = &NESCPU::STA_Zero_Page;      // STA zp
		opcodeTable[0x95] = &NESCPU::STA_Zero_Page_X;    // STA zp,X
		opcodeTable[0x8D] = &NESCPU::STA_Absolute;       // STA abs
		opcodeTable[0x9D] = &NESCPU::STA_Absolute_X;     // STA abs,X
		opcodeTable[0x99] = &NESCPU::STA_Absolute_Y;     // STA abs,Y
		opcodeTable[0x81] = &NESCPU::STA_Indirect_X;     // STA (zp,X)
		opcodeTable[0x91] = &NESCPU::STA_Indirect_Y;     // STA (zp),Y

		// STX – Store X Register
		opcodeTable[0x86] = &NESCPU::STX_Zero_Page;      // STX zp
		opcodeTable[0x96] = &NESCPU::STX_Zero_Page_Y;    // STX zp,Y
		opcodeTable[0x8E] = &NESCPU::STX_Absolute;       // STX abs

		// STY – Store Y Register
		opcodeTable[0x84] = &NESCPU::STY_Zero_Page;      // STY zp
		opcodeTable[0x94] = &NESCPU::STY_Zero_Page_X;    // STY zp,X
		opcodeTable[0x8C] = &NESCPU::STY_Absolute;       // STY abs

		// LDX – Load X Register
		opcodeTable[0xA2] = &NESCPU::LDX_Immediate;      // LDX #imm
		opcodeTable[0xA6] = &NESCPU::LDX_Zero_Page;      // LDX zp
		opcodeTable[0xB6] = &NESCPU::LDX_Zero_Page_Y;    // LDX zp,Y
		opcodeTable[0xAE] = &NESCPU::LDX_Absolute;       // LDX abs
		opcodeTable[0xBE] = &NESCPU::LDX_Absolute_Y;     // LDX abs,Y

		// LDY – Load Y Register
		opcodeTable[0xA0] = &NESCPU::LDY_Immediate;      // LDY #imm
		opcodeTable[0xA4] = &NESCPU::LDY_Zero_Page;      // LDY zp
		opcodeTable[0xB4] = &NESCPU::LDY_Zero_Page_X;    // LDY zp,X
		opcodeTable[0xAC] = &NESCPU::LDY_Absolute;       // LDY abs
		opcodeTable[0xBC] = &NESCPU::LDY_Absolute_X;     // LDY abs,X

		// Stack Operations
		opcodeTable[0x48] = &NESCPU::PHA_Implied;        // PHA (push A)
		opcodeTable[0x68] = &NESCPU::PLA_Implied;        // PLA (pull A)
		opcodeTable[0x08] = &NESCPU::PHP_Implied;        // PHP (push P)
		opcodeTable[0x28] = &NESCPU::PLP_Implied;        // PLP (pull P)

		// Register Transfers
		opcodeTable[0xAA] = &NESCPU::TAX_Implied;        // TAX (A → X)
		opcodeTable[0x8A] = &NESCPU::TXA_Implied;        // TXA (X → A)
		opcodeTable[0xA8] = &NESCPU::TAY_Implied;        // TAY (A → Y)
		opcodeTable[0x98] = &NESCPU::TYA_Implied;        // TYA (Y → A)

		// Increments / Decrements
		opcodeTable[0xE8] = &NESCPU::INX_Implied;        // INX
		opcodeTable[0xCA] = &NESCPU::DEX_Implied;        // DEX
		opcodeTable[0xC8] = &NESCPU::INY_Implied;        // INY
		opcodeTable[0x88] = &NESCPU::DEY_Implied;        // DEY

		// Control Flow
		opcodeTable[0x20] = &NESCPU::JSR_Implied;        // JSR abs
		opcodeTable[0x60] = &NESCPU::RTS_Implied;        // RTS
		opcodeTable[0x00] = &NESCPU::BRK_Implied;        // BRK

	}
	
	public:


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
	void SetCarry() {
		P |= 0b00000001;
	}
	void SetZero() {
		P |= 0b00000010;
	}
	void SetInterrupt() {
		P |= 0b00000100;
	}
	void SetDecimal() {
		P |= 0b00001000;
	}
	void SetBreak() {
		P |= 0b00010000;
	}
	void SetOverflow() {
		P |= 0b01000000;
	}
	void SetNegative() {
		P |= 0b10000000;
	}
	void SetUnused() {
		P |= 0b00100000;
	}
	void ClearCarry() { P &= ~0b00000001; }
	void ClearZero() { P &= ~0b00000010; }
	void ClearInterrupt() { P &= ~0b00000100; }
	void ClearDecimal() { P &= ~0b00001000; }
	void ClearBreak() { P &= ~0b00010000; }
	void ClearUnused() { P &= ~0b00100000; }
	void ClearOverflow() { P &= ~0b01000000; }
	void ClearNegative() { P &= ~0b10000000; }

	bool GetCarry() {
		return (P & 0b00000001) !=0;
	}
	bool GetZero() {
		return (P & 0b00000010) != 0;
	}
	bool GetInterrupt() {
		return (P & 0b00000100) != 0;
	}
	bool GetDecimal() {
		return(P & 0b00001000) != 0;
	}
	bool GetBreak() {
		return (P & 0b00010000) != 0;
	}
	bool GetOverflow() {
		return (P & 0b01000000) != 0;
	}
	bool GetNegative() {
		return (P & 0b10000000) != 0;
	}
	bool GetUnused() {
		return (P & 0b00100000) !=0;
	}
	void Reset() {
		//nesdev.org/wiki/CPU_power_up_state
		//Interrupt set to 1, AXY set to 0, FFFC = 00 and FFFD = x80.S starts at xFD
		memory[0xFFFC] = 0x00;
		memory[0xFFFD] = 0x80;
		PC = memory[0xFFFD] << 8 | memory[0xFFFC];
		A = 0;
		Y = 0;
		X = 0;
		S = 0xFD;
		P = 0b00100000;
	}

	//void Connect

};

int main() {
	NESCPU cpu;
	//NESBUS bus;
	cpu.Reset();







	
}