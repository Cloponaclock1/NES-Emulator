#pragma once
#include <cstdint>
#include "Bus.h"
#include <functional>
#include <map>        // for std::map
#include <string>     // for std::string
#include <iomanip>    // optional if you're using stringstreams for hex formatting
#include <sstream>    // optional, only if you switch from manual hex formatting

class cpu {
public:
    cpu();
    Bus* bus = nullptr;

    // Make gettr for opcode table varrables and registers, pc and cycles
    typedef void (cpu::* Opcodehandler)();  // Optional for naming clarity

    //Struct to hold opcode data
    struct Opcode {
        std::function<void()> handler;
        uint8_t bytes;
        uint8_t cycles;

    };
    Opcode opcodeTable[256];
    uint32_t clockCycles = 0;


    void Reset();
    void clock();
    void irq();
    void nmi();
    bool complete();


    void ConnectBus(Bus* bus);
    // Registers
    uint8_t A =0, X = 0, Y = 0, S = 0;
    uint8_t P = 0;
    uint16_t PC = 0;
    int cycles = 0;


    // Flag setters
    void SetCarry();
    void SetZero();
    void SetInterrupt();
    void SetDecimal();
    void SetBreak();
    void SetOverflow();
    void SetNegative();
    void SetUnused();

    // Flag clearers
    void ClearCarry();
    void ClearZero();
    void ClearInterrupt();
    void ClearDecimal();
    void ClearBreak();
    void ClearUnused();
    void ClearOverflow();
    void ClearNegative();

    // Flag getters
    bool GetCarry();
    bool GetZero();
    bool GetInterrupt();
    bool GetDecimal();
    bool GetBreak();
    bool GetOverflow();
    bool GetNegative();
    bool GetUnused();
    void Push(uint8_t value);
    uint8_t Pop();
private:

    void InitopcodeTable();
    void CheckByte(uint8_t reg);

    // Opcode functions 
    void LDA(uint8_t value);
    void STA(uint16_t value);
    void STY(uint16_t value);
    void STX(uint16_t value);
    void ADC(uint8_t value);
    void LDX(uint8_t value);
   
    void LDY(uint8_t value);
    void PHA();
    void PLA();
    void PHP();
    void PLP();
    void TAX();
    void TAY();
    void TXA();
    void TYA();
    void INX();
    void INY();
    void DEX();
    void DEY();
    void RTS();
    void BRK();
    void RTI();
    void SBC(uint8_t value);
    void AND(uint8_t value);
    void ASL(uint16_t value);
    void ASL_A();
    void BCC();
    void BCS();
    void BIT(uint8_t value);
    void BEQ();
    void BMI();
    void BNE();
    void BPL();
    void BVC();
    void BVS();
    void CLC();
    void CLD();
    void CLI();
    void CLV();
    void CMP(uint8_t value);
    void CPX(uint8_t value);
    void CPY(uint8_t value);
    void DEC(uint16_t addr);
    void EOR(uint8_t value);
    void INC(uint16_t value);
    void JMP(uint16_t addr);
    void JSR(uint16_t value);
    void LSR_A();
    void LSR(uint16_t value);
    void NOP();
    void ORA(uint8_t value);
    void ROL(uint16_t value);
    void ROL_A();
    void ROR_A();
    void ROR(uint16_t value);
    void SEC();
    void SED();
    void SEI();
    void TSX();
    void TXS();
    void JMPIndirect();
    
    void NOP_Illegal();



    //adressing modes
    //Fetch
    uint8_t FetchImmediate();
    uint8_t FetchZeroPage();
    uint8_t FetchZeroPageX();
    uint8_t FetchZeroPageY();
    uint8_t FetchAbsolute();
    uint8_t FetchAbsoluteX();
    uint8_t FetchAbsoluteY();
    uint8_t FetchIndirectX();
    uint8_t FetchIndirectY();
    //Store
    uint16_t GetZeroPageAddress();
    uint16_t GetZeroPageAddressX();
    uint16_t GetZeroPageAddressY();

    uint16_t GetAddressAbsolute();
    uint16_t GetAddressAbsoluteX();
    uint16_t GetAddressAbsoluteY();
    uint16_t GetIndirectAddressX();
    uint16_t GetIndirectAddressY();














};
