#pragma once
#include <cstdint>



class cpu {
public:
    cpu();
    void Reset();
    void ConnectBus(Bus* bus);
 

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
    Bus* bus = nullptr;


    // Opcode Table
    typedef void (cpu::* OpcodeTable)();
    OpcodeTable opcodeTable[256];

    // Internal helpers
    void InitopcodeTable();
    void CheckRegister(uint8_t reg);

    // Opcode functions (from your code)
    void LDA_Immediate();
    void LDA_Zero_Page();
    void LDA_Zero_Page_X();
    void LDA_Absolute();
    void LDA_Absolute_X();
    void LDA_Absolute_Y();
    void LDA_Indirect_X();
    void LDA_Indirect_Y();

    void STA_Zero_Page();
    void STA_Zero_Page_X();
    void STA_Absolute();
    void STA_Absolute_X();
    void STA_Absolute_Y();
    void STA_Indirect_X();
    void STA_Indirect_Y();

    void STX_Zero_Page();
    void STX_Zero_Page_Y();
    void STX_Absolute();

    void STY_Zero_Page();
    void STY_Zero_Page_X();
    void STY_Absolute();

    void LDX_Immediate();
    void LDX_Zero_Page();
    void LDX_Zero_Page_Y();
    void LDX_Absolute();
    void LDX_Absolute_Y();

    void LDY_Immediate();
    void LDY_Zero_Page();
    void LDY_Zero_Page_X();
    void LDY_Absolute();
    void LDY_Absolute_X();

    void PHA_Implied();
    void PLA_Implied();
    void PHP_Implied();
    void PLP_Implied();

    void TAX_Implied();
    void TAY_Implied();
    void TXA_Implied();
    void TYA_Implied();

    void INX_Implied();
    void INY_Implied();
    void DEX_Implied();
    void DEY_Implied();

    void JSR_Implied();
    void RTS_Implied();
    void BRK_Implied();
};
