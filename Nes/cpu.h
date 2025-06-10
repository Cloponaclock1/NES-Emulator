#pragma once
#include <cstdint>
#include "CPU.cpp"


class cpu {
public:
    cpu();
    void Reset();
 

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
    // Registers
    uint8_t A, X, Y, S;
    uint8_t P = 0;
    uint16_t PC;
    int cycles;

    // Static memory (64KB)
    static uint8_t memory[0x10000];

    // Opcode Table
    typedef void (cpu::* OpcodeTable)(uint8_t*);
    OpcodeTable opcodeTable[256];

    // Internal helpers
    void InitopcodeTable();
    void CheckRegister(uint8_t reg);

    // Opcode functions (from your code)
    void LDA_Immediate(uint8_t*);
    void LDA_Zero_Page(uint8_t*);
    void LDA_Zero_Page_X(uint8_t*);
    void LDA_Absolute(uint8_t*);
    void LDA_Absolute_X(uint8_t*);
    void LDA_Absolute_Y(uint8_t*);
    void LDA_Indirect_X(uint8_t*);
    void LDA_Indirect_Y(uint8_t*);

    void STA_Zero_Page(uint8_t*);
    void STA_Zero_Page_X(uint8_t*);
    void STA_Absolute(uint8_t*);
    void STA_Absolute_X(uint8_t*);
    void STA_Absolute_Y(uint8_t*);
    void STA_Indirect_X(uint8_t*);
    void STA_Indirect_Y(uint8_t*);

    void STX_Zero_Page(uint8_t*);
    void STX_Zero_Page_Y(uint8_t*);
    void STX_Absolute(uint8_t*);

    void STY_Zero_Page(uint8_t*);
    void STY_Zero_Page_X(uint8_t*);
    void STY_Absolute(uint8_t*);

    void LDX_Immediate(uint8_t*);
    void LDX_Zero_Page(uint8_t*);
    void LDX_Zero_Page_Y(uint8_t*);
    void LDX_Absolute(uint8_t*);
    void LDX_Absolute_Y(uint8_t*);

    void LDY_Immediate(uint8_t*);
    void LDY_Zero_Page(uint8_t*);
    void LDY_Zero_Page_X(uint8_t*);
    void LDY_Absolute(uint8_t*);
    void LDY_Absolute_X(uint8_t*);

    void PHA_Implied(uint8_t*);
    void PLA_Implied(uint8_t*);
    void PHP_Implied(uint8_t*);
    void PLP_Implied(uint8_t*);

    void TAX_Implied(uint8_t*);
    void TAY_Implied(uint8_t*);
    void TXA_Implied(uint8_t*);
    void TYA_Implied(uint8_t*);

    void INX_Implied(uint8_t*);
    void INY_Implied(uint8_t*);
    void DEX_Implied(uint8_t*);
    void DEY_Implied(uint8_t*);

    void JSR_Implied(uint8_t*);
    void RTS_Implied(uint8_t*);
    void BRK_Implied(uint8_t*);
};
