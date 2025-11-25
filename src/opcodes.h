// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef SRC_OPCODES_H_
#define SRC_OPCODES_H_

#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace SuperH {

namespace Opcodes {
// 0b0000 Prefixes
// MUL.L Rm,Rn          0000nnnnmmmm0111
constexpr uint16_t MullRmRn = 0b0111;
// CLRT                 0000000000001000
constexpr uint16_t Clrt = 0b1000;
// NOP                  0000000000001001
constexpr uint16_t Nop = 0b1001;
// RTS                  0000000000001011
constexpr uint16_t Rts = 0b1011;
// SETT                 0000000000011000
constexpr uint16_t Sett = 0b11000;
// DIV0U                0000000000011001
constexpr uint16_t Div0u = 0b11001;
// SLEEP                0000000000011011
constexpr uint16_t Sleep = 0b11011;
// CLRMAC               0000000000101000
constexpr uint16_t Clrmac = 0b101000;
// MOVT Rn              0000nnnn00101001
constexpr uint16_t MovtRn = 0b00101001;
// RTE                  0000000000101011
constexpr uint16_t Rte = 0b101011;

// 0b0010 Prefixes
// DIV0S Rm,Rn          0010nnnnmmmm0111
constexpr uint16_t Div0sRmRn = 0b0010 << 12 | 0b0111;
// TST Rm,Rn            0010nnnnmmmm1000
constexpr uint16_t TstRmRn = 0b0010 << 12 | 0b1000;
// AND Rm,Rn            0010nnnnmmmm1001
constexpr uint16_t AndRmRn = 0b0010 << 12 | 0b1001;
// XOR Rm,Rn            0010nnnnmmmm1010
constexpr uint16_t XorRmRn = 0b0010 << 12 | 0b1010;
// OR Rm,Rn             0010nnnnmmmm1011
constexpr uint16_t OrRmRn = 0b0010 << 12 | 0b1011;
// CMP/STR Rm,Rn        0010nnnnmmmm1100
constexpr uint16_t CmpStrRmRn = 0b0010 << 12 | 0b1100;
// XTRCT Rm,Rn          0010nnnnmmmm1101
constexpr uint16_t XtrctRmRn = 0b0010 << 12 | 0b1101;
// MULU.W Rm,Rn         0010nnnnmmmm1110
constexpr uint16_t MuluwRmRn = 0b0010 << 12 | 0b1110;
// MULS.W Rm,Rn         0010nnnnmmmm1111
constexpr uint16_t MulswRmRn = 0b0010 << 12 | 0b1111;

// 0b0011 Prefixes
// CMP/EQ Rm,Rn         0011nnnnmmmm0000
constexpr uint16_t CmpEqRmRn = 0b0011 << 12;
// CMP/HS Rm,Rn         0011nnnnmmmm0010
constexpr uint16_t CmpHsRmRn = 0b0011 << 12 | 0b0010;
// CMP/GE Rm,Rn         0011nnnnmmmm0011
constexpr uint16_t CmpGeRmRn = 0b0011 << 12 | 0b0011;
// DIV1 Rm,Rn           0011nnnnmmmm0100
constexpr uint16_t Div1RmRn = 0b0011 << 12 | 0b0100;
// DMULU.L Rm,Rn        0011nnnnmmmm0101
constexpr uint16_t DmululRmRn = 0b0011 << 12 | 0b0101;
// CMP/HI Rm,Rn         0011nnnnmmmm0110
constexpr uint16_t CmpHiRmRn = 0b0011 << 12 | 0b0110;
// CMP/GT Rm,Rn         0011nnnnmmmm0111
constexpr uint16_t CmpGtRmRn = 0b0011 << 12 | 0b0111;
// SUB Rm,Rn            0011nnnnmmmm1000
constexpr uint16_t SubRmRn = 0b0011 << 12 | 0b1000;
// SUBC Rm,Rn           0011nnnnmmmm1010
constexpr uint16_t SubcRmRn = 0b0011 << 12 | 0b1010;
// SUBV Rm,Rn           0011nnnnmmmm1011
constexpr uint16_t SubvRmRn = 0b0011 << 12 | 0b1011;
// ADD Rm,Rn            0011nnnnmmmm1100
constexpr uint16_t AddRmRn = 0b0011 << 12 | 0b1100;
// DMULS.L Rm,Rn        0011nnnnmmmm1101
constexpr uint16_t DmulslRmRn = 0b0011 << 12 | 0b1101;
// ADDC Rm,Rn           0011nnnnmmmm11101
constexpr uint16_t AddcRmRn = 0b0011 << 12 | 0b1110;
// ADDV Rm,Rn           0011nnnnmmmm1111
constexpr uint16_t AddvRmRn = 0b0011 << 12 | 0b1111;

// 0b0100 Prefixes
// SHLL Rn              0100nnnn00000000
constexpr uint16_t ShllRn = 0b0100 << 12;
// SHLR Rn              0100nnnn00000001
constexpr uint16_t ShlrRn = 0b0100 << 12 | 0b00000001;
// ROTL Rn              0100nnnn00000100
constexpr uint16_t RotlRn = 0b0100 << 12 | 0b00000100;
// ROTR Rn              0100nnnn00000101
constexpr uint16_t RotrRn = 0b0100 << 12 | 0b00000101;
// DT Rn                0100nnnn00010000
constexpr uint16_t DtRn = 0b0100 << 12 | 0b00010000;
// CMP/PZ Rn            0100nnnn00010001
constexpr uint16_t CmpPzRn = 0b0100 << 12 | 0b00010001;
// CMP/PL Rn            0100nnnn00010101
constexpr uint16_t CmpPlRn = 0b0100 << 12 | 0b00010101;
// SHAL Rn              0100nnnn00100000
constexpr uint16_t ShalRn = 0b0100 << 12 | 0b00100000;
// SHAR Rn              0100nnnn00100001
constexpr uint16_t SharRn = 0b0100 << 12 | 0b00100001;
// ROTCR Rn             0100nnnn00100101
constexpr uint16_t RotcrRn = 0b0100 << 12 | 0b00100101;

// 0b0110 Prefixes
// MOV Rm,Rn            0110nnnnmmmm0011
constexpr uint16_t MovRmRn = 0b0110 << 12 | 0b0011;
// NOT Rm,Rn            0110nnnnmmmm0111
constexpr uint16_t NotRmRn = 0b0110 << 12 | 0b0111;
// SWAP.B Rm,Rn         0110nnnnmmmm1000
constexpr uint16_t SwapbRmRn = 0b0110 << 12 | 0b01000;
// SWAP.W Rm,Rn         0110nnnnmmmm1001
constexpr uint16_t SwapwRmRn = 0b0110 << 12 | 0b1001;
// NEGC Rm,Rn           0110nnnnmmmm1010
constexpr uint16_t NegcRmRn = 0b0110 << 12 | 0b1010;
// NEG Rm,Rn            0110nnnnmmmm1011
constexpr uint16_t NegRmRn = 0b0110 << 12 | 0b1011;
// EXTU.B Rm,Rn         0110nnnnmmmm1100
constexpr uint16_t ExtubRmRn = 0b0110 << 12 | 0b1100;
// EXTU.W Rm,Rn         0110nnnnmmmm1101
constexpr uint16_t ExtuwRmRn = 0b0110 << 12 | 0b1101;
// EXTS.B Rm,Rn         0110nnnnmmmm1110
constexpr uint16_t ExtsbRmRn = 0b0110 << 12 | 0b1110;
// EXTS.W Rm,Rn         0110nnnnmmmm1111
constexpr uint16_t ExtswRmRn = 0b0110 << 12 | 0b1111;

static const std::unordered_map<uint16_t, std::string> NAMES = {
    {AddRmRn, "ADD"},
    {AddcRmRn, "ADDC"},
    {AddvRmRn, "ADDV"},
    {AndRmRn, "AND"},
    {Clrmac, "CLRMAC"},
    {Clrt, "CLRT"},
    {CmpEqRmRn, "CMP/EQ"},
    {CmpGeRmRn, "CMP/GE"},
    {CmpGtRmRn, "CMP/GT"},
    {CmpHiRmRn, "CMP/HI"},
    {CmpHsRmRn, "CMP/HS"},
    {CmpPlRn, "CMP/PL"},
    {CmpPzRn, "CMP/PZ"},
    {CmpStrRmRn, "CMP/STR"},
    {Div0sRmRn, "DIV0S"},
    {Div0u, "DIV0U"},
    {Div1RmRn, "DIV1"},
    {DmululRmRn, "DMULU.L"},
    {DmulslRmRn, "DMULS.L"},
    {DtRn, "DT"},
    {ExtsbRmRn, "EXTS.B"},
    {ExtswRmRn, "EXTS.W"},
    {ExtubRmRn, "EXTU.B"},
    {ExtuwRmRn, "EXTU.W"},
    {MovRmRn, "MOV"},
    {MovtRn, "MOVT"},
    {MullRmRn, "MUL.L"},
    {MulswRmRn, "MULS.W"},
    {MuluwRmRn, "MULU.W"},
    {NegRmRn, "NEG"},
    {NegcRmRn, "NEGC"},
    {Nop, "NOP"},
    {NotRmRn, "NOT"},
    {OrRmRn, "OR"},
    {RotlRn, "ROTL"},
    {RotcrRn, "ROTCR"},
    {RotrRn, "ROTR"},
    {Rte, "RTE"},
    {Rts, "RTS"},
    {Sett, "SETT"},
    {ShalRn, "SHAL"},
    {SharRn, "SHAR"},
    {ShllRn, "SHLL"},
    {ShlrRn, "SHLR"},
    {SubRmRn, "SUB"},
    {Sleep, "SLEEP"},
    {SubcRmRn, "SUBC"},
    {SubvRmRn, "SUBV"},
    {SwapbRmRn, "SWAP.B"},
    {SwapwRmRn, "SWAP.W"},
    {TstRmRn, "TST"},
    {XorRmRn, "XOR"},
    {XtrctRmRn, "XTRCT"}};

static const std::set BRANCHING = {Rte, Rts};

// Construct a list of all instructions
std::vector<int> GetAllOpcodes();
std::vector<int> GetSimpleOpcodes();

}  // namespace Opcodes

typedef uint8_t N;
typedef uint8_t M;
typedef uint8_t D;
typedef uint16_t D12;
typedef uint8_t I;

// NM Format (Dis)assembly
std::pair<N, M> GetNMFormatOpcodeFields(uint16_t opcode);
uint16_t SetNMFormatOpcodeFields(uint16_t opcode, N, M);

// N Format (Dis)assembly
N GetNFormatOpcodeField(uint16_t opcode);
uint16_t SetNFormatOpcodeField(uint16_t opcode, N);

// M Format (Dis)assembly
M GetMFormatOpcodeField(uint16_t opcode);
uint16_t SetMFormatOpcodeField(uint16_t opcode, M);

// I Format (Dis)assembly
I GetIFormatOpcodeField(uint16_t opcode);
uint16_t SetIFormatOpcodeField(uint16_t opcode, I);

std::pair<M, D> ExtractMDFormatOpcodeFields(uint16_t opcode);
std::pair<N, D> ExtractND4FormatOpcodeFields(uint16_t opcode);
std::tuple<N, M, D> ExtractNMDFormatOpcodeFields(uint16_t opcode);
D ExtractDFormatOpcodeFields(uint16_t opcode);
D12 ExtractD12FormatOpcodeFields(uint16_t opcode);
std::pair<N, D> ExtractND8FormatOpcodeFields(uint16_t opcode);
std::pair<N, I> ExtractNIFormatOpcodeFields(uint16_t opcode);
}  // namespace SuperH

#endif  // SRC_OPCODES_H_
