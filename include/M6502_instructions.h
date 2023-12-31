#pragma once

// Instructions
void ADC(struct M6502* computer, uchar8_t mode);
void AND(struct M6502* computer, uchar8_t mode);
void ASL(struct M6502* computer, uchar8_t mode);
void BCC(struct M6502* computer);
void BCS(struct M6502* computer);
void BEQ(struct M6502* computer);
void BIT(struct M6502* computer, uchar8_t mode);
void BMI(struct M6502* computer); 
void BNE(struct M6502* computer);
void BPL(struct M6502* computer); 
void BRK(struct M6502* computer);
void BVC(struct M6502* computer);
void BVS(struct M6502* computer); 
void CLC(struct M6502* computer);
void CLD(struct M6502* computer);
void CLI(struct M6502* computer);
void CLV(struct M6502* computer);
void CMP(struct M6502* computer, uchar8_t mode);
void CPX(struct M6502* computer, uchar8_t mode);
void CPY(struct M6502* computer, uchar8_t mode);
void DEC(struct M6502* computer, uchar8_t mode);
void DEX(struct M6502* computer);
void DEY(struct M6502* computer);
void EOR(struct M6502* computer, uchar8_t mode);
void INC(struct M6502* computer, uchar8_t mode);
void INX(struct M6502* computer);
void INY(struct M6502* computer);
void JMP(struct M6502* computer, uchar8_t mode);
void JSR(struct M6502* computer, uchar8_t mode);
void LDA(struct M6502* computer, uchar8_t mode);
void LDX(struct M6502* computer, uchar8_t mode);
void LDY(struct M6502* computer, uchar8_t mode);
void LSR(struct M6502* computer, uchar8_t mode);
void NOP(struct M6502* computer);
void ORA(struct M6502* computer, uchar8_t mode);
void PHA(struct M6502* computer);
void PHP(struct M6502* computer);
void PLA(struct M6502* computer);
void PLP(struct M6502* computer);
void ROL(struct M6502* computer, uchar8_t mode);
void ROR(struct M6502* computer, uchar8_t mode);
void RTI(struct M6502* computer);
void RTS(struct M6502* computer);
void SBC(struct M6502* computer, uchar8_t mode);
void SEC(struct M6502* computer);
void SED(struct M6502* computer);
void SEI(struct M6502* computer);
void STA(struct M6502* computer, uchar8_t mode);
void STX(struct M6502* computer, uchar8_t mode);
void STY(struct M6502* computer, uchar8_t mode);
void TAX(struct M6502* computer);
void TAY(struct M6502* computer);
void TSX(struct M6502* computer);
void TXA(struct M6502* computer);
void TXS(struct M6502* computer);
void TYA(struct M6502* computer);

/*
a well-known bug in indirect jump instruction, JMP (address), where if the address is on a page boundary, 
it doesn't fetch the correct second byte of the target address. This is a specific quirk of the 6502
*/