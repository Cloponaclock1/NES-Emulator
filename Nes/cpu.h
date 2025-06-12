#pragma once
#include <cstdint>
#include "Bus.h"
#include <functional>


class cpu {
public:
    cpu();
    Bus* bus = nullptr;

    // Make gettr for opcode table varrables and registers, pc and cycles
    typedef void (cpu::* Opcodehandler)();  // Optional for naming clarity

    //Struct to hold opcode data
    struct Opcode {
        std::function<void()> handler;
        uint8_t cycles;
        uint8_t bytes;

    };
    Opcode opcodeTable[256];
    uint8_t totalCycles = 0;
    uint32_t clockCycles = 0;


    void Reset();
    void clock();
    void irq();
    void nmi();
    bool complete();

    void ConnectBus(Bus* bus);
    // Registers
    uint8_t A, X, Y, S;
    uint8_t P = 0;
    uint16_t PC;
    int cycles;


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
    uint16_t StoreZeroPage();
    uint16_t StoreZeroPageX();
    uint16_t StoreZeroPageY();

    uint16_t StoreAbsolute();
    uint16_t StoreAbsoluteX();
    uint16_t StoreAbsoluteY();
    uint16_t StoreIndirectX();
    uint16_t StoreIndirectY();














};
