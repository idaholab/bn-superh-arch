// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "registers.h"

namespace SuperH::Registers {
// TODO: Should this be a hash table?
std::string to_string(const uint32_t rid) {
  switch (rid) {
    // General Registers
    case R0:
      return "R0";
    case R1:
      return "R1";
    case R2:
      return "R2";
    case R3:
      return "R3";
    case R4:
      return "R4";
    case R5:
      return "R5";
    case R6:
      return "R6";
    case R7:
      return "R7";
    case R8:
      return "R8";
    case R9:
      return "R9";
    case R10:
      return "R10";
    case R11:
      return "R11";
    case R12:
      return "R12";
    case R13:
      return "R13";
    case R14:
      return "R14";
    case R15:
      return "R15";
    // Control Registers
    case SR:
      return "SR";
    case GBR:
      return "GBR";
    case VBR:
      return "VBR";
    // System Registers
    case MACH:
      return "MACH";
    case MACL:
      return "MACL";
    case PR:
      return "PR";
    case PC:
      return "PC";
    // Floating Point Registers
    case FR0:
      return "FR0";
    case FR1:
      return "FR1";
    case FR2:
      return "FR2";
    case FR3:
      return "FR3";
    case FR4:
      return "FR4";
    case FR5:
      return "FR5";
    case FR6:
      return "FR6";
    case FR7:
      return "FR7";
    case FR8:
      return "FR8";
    case FR9:
      return "FR9";
    case FR10:
      return "FR10";
    case FR11:
      return "FR11";
    case FR12:
      return "FR12";
    case FR13:
      return "FR13";
    case FR14:
      return "FR14";
    case FR15:
      return "FR15";
    // Floating Point System Registers
    case FPUL:
      return "FPUL";
    case FPSCR:
      return "FPSCR";
    default:
      return "";
  }
}
}  // namespace SuperH::Registers
