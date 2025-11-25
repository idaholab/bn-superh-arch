// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef SRC_REGISTERS_H_
#define SRC_REGISTERS_H_

#include <array>
#include <cstdint>
#include <string>

namespace SuperH::Registers {
// 32-bit general registers R0-R15, used for data processing and address
// calculation
constexpr uint32_t R0 = 0;  // Index reg (sometimes as a fixed src/dst register)
constexpr uint32_t R1 = 1;
constexpr uint32_t R2 = 2;
constexpr uint32_t R3 = 3;
constexpr uint32_t R4 = 4;
constexpr uint32_t R5 = 5;
constexpr uint32_t R6 = 6;
constexpr uint32_t R7 = 7;
constexpr uint32_t R8 = 8;
constexpr uint32_t R9 = 9;
constexpr uint32_t R10 = 10;
constexpr uint32_t R11 = 11;
constexpr uint32_t R12 = 12;
constexpr uint32_t R13 = 13;
constexpr uint32_t R14 = 14;
constexpr uint32_t R15 = 15;  // Stack pointer

static constexpr std::array<uint32_t, 16> GeneralPurposeRegisters = {
    R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15};

// Control Registers
constexpr uint32_t SR = 16;   // Status register
constexpr uint32_t GBR = 17;  // Global base register
constexpr uint32_t VBR = 18;  // Vector base register

// System Registers
constexpr uint32_t MACH = 19;  // Multiply and accumulate register high
constexpr uint32_t MACL = 20;  // Multiply and accumulate register low
constexpr uint32_t PR = 21;    // Procedure register - stores return address
// calls
constexpr uint32_t PC = 22;  // Program counter

// Floating Point Registers
constexpr uint32_t FR0 = 23;
constexpr uint32_t FR1 = 24;
constexpr uint32_t FR2 = 25;
constexpr uint32_t FR3 = 26;
constexpr uint32_t FR4 = 27;
constexpr uint32_t FR5 = 28;
constexpr uint32_t FR6 = 29;
constexpr uint32_t FR7 = 30;
constexpr uint32_t FR8 = 31;
constexpr uint32_t FR9 = 32;
constexpr uint32_t FR10 = 33;
constexpr uint32_t FR11 = 34;
constexpr uint32_t FR12 = 35;
constexpr uint32_t FR13 = 36;
constexpr uint32_t FR14 = 37;
constexpr uint32_t FR15 = 38;

// Floating Point System Registers
constexpr uint32_t FPUL = 39;
constexpr uint32_t FPSCR = 40;

std::string to_string(uint32_t rid);
}  // namespace SuperH::Registers

#endif  // SRC_REGISTERS_H_
