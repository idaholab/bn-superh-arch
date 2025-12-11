// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "architecture.h"

#include "instructions.h"
#include "registers.h"
#include "sizes.h"

namespace SuperH {
// Anything applicable to the SuperH in general should go here
Architecture::Architecture(const std::string &name)
    : BN::Architecture(name), isa_type() {}

BNRegisterInfo Architecture::RegisterInfo(const uint32_t fullWidthReg,
                                          const size_t offset,
                                          const size_t size,
                                          const bool zeroExtend) {
  BNRegisterInfo result{};
  result.fullWidthRegister = fullWidthReg;
  result.offset = offset;  // in bytes
  result.size = size;
  result.extend = zeroExtend ? ZeroExtendToFullWidth : NoExtend;
  return result;
}

size_t Architecture::GetAddressSize() const { return Sizes::LONG; }

[[nodiscard]] BNEndianness Architecture::GetEndianness() const {
  return BigEndian;
}

[[nodiscard]] size_t Architecture::GetDefaultIntegerSize() const {
  return Sizes::WORD;
}

[[nodiscard]] size_t Architecture::GetInstructionAlignment() const {
  return Sizes::WORD;
}

[[nodiscard]] size_t Architecture::GetMaxInstructionLength() const {
  return Sizes::WORD;
}

bool Architecture::GetInstructionInfo(const uint8_t *data, const uint64_t addr,
                                      const size_t maxLen,
                                      BN::InstructionInfo &result) {
  // Instructions are a minimum of 16 bits
  if (maxLen < GetMaxInstructionLength()) {
    return false;
  }

  // Swap bytes to Big Endian
  const uint16_t opcode = (static_cast<uint16_t>(data[0]) << 8) | data[1];

  if (const auto i = DecodeInstruction(isa_type, opcode)) {
    return i->get()->Info(opcode, addr, result);
  }

  return false;
}

bool Architecture::GetInstructionText(
    const uint8_t *data, const uint64_t addr, size_t &len,
    std::vector<BN::InstructionTextToken> &result) {
  const uint16_t opcode = (static_cast<uint16_t>(data[0]) << 8) | data[1];

  if (const auto i = DecodeInstruction(isa_type, opcode)) {
    len = Instruction::length;
    return i->get()->Text(opcode, addr, len, result);
  }

  return false;
}

bool Architecture::GetInstructionLowLevelIL(const uint8_t *data,
                                            const uint64_t addr, size_t &len,
                                            BN::LowLevelILFunction &il) {
  // Swap bytes to Big Endian
  const uint16_t opcode = (static_cast<uint16_t>(data[0]) << 8) | data[1];

  if (const auto i = DecodeInstruction(isa_type, opcode)) {
    len = Instruction::length;
    return i->get()->Lift(opcode, addr, len, il, this);
  }

  return false;
}

std::string Architecture::GetRegisterName(const uint32_t reg) {
  auto result = Registers::to_string(reg);
  if (result.empty()) {
    return "GetRegisterName: INVALID_REG_ID";
  }
  return result;
}

uint32_t Architecture::GetStackPointerRegister() { return Registers::R15; }

// The SH-1 implements at least one instruction differently than the SH-2
// (MAC/MAC.W)
SH1Architecture::SH1Architecture(const std::string &name) : Architecture(name) {
  this->isa_type = SH_1_ISA;
}

std::vector<uint32_t> SH1Architecture::GetAllRegisters() {
  return std::vector<uint32_t>{
      Registers::R0,   Registers::R1,  Registers::R2,  Registers::R3,
      Registers::R4,   Registers::R5,  Registers::R6,  Registers::R7,
      Registers::R8,   Registers::R9,  Registers::R10, Registers::R11,
      Registers::R12,  Registers::R13, Registers::R14, Registers::R15,
      Registers::SR,   Registers::GBR, Registers::VBR, Registers::MACH,
      Registers::MACL, Registers::PR,  Registers::PC};
}

BNRegisterInfo SH1Architecture::GetRegisterInfo(const uint32_t reg) {
  std::vector<uint32_t> regs = GetAllRegisters();
  if (reg <= Registers::PC) {
    // All registers are 32 bits
    return RegisterInfo(reg, 0, 4);
  }
  return RegisterInfo(0, 0, 0);
}

// The SH-2 is a subset of the SH-2E (adds FPU).
SH2EArchitecture::SH2EArchitecture(const std::string &name)
    : Architecture(name) {
  this->isa_type = SH_2E_ISA;
}

std::vector<uint32_t> SH2EArchitecture::GetAllRegisters() {
  return std::vector{
      Registers::R0,   Registers::R1,   Registers::R2,   Registers::R3,
      Registers::R4,   Registers::R5,   Registers::R6,   Registers::R7,
      Registers::R8,   Registers::R9,   Registers::R10,  Registers::R11,
      Registers::R12,  Registers::R13,  Registers::R14,  Registers::R15,
      Registers::SR,   Registers::GBR,  Registers::VBR,  Registers::MACH,
      Registers::MACL, Registers::PR,   Registers::PC,   Registers::FR0,
      Registers::FR1,  Registers::FR2,  Registers::FR3,  Registers::FR4,
      Registers::FR5,  Registers::FR6,  Registers::FR7,  Registers::FR8,
      Registers::FR9,  Registers::FR10, Registers::FR11, Registers::FR12,
      Registers::FR13, Registers::FR14, Registers::FR15, Registers::FPUL,
      Registers::FPSCR};
}

BNRegisterInfo SH2EArchitecture::GetRegisterInfo(const uint32_t reg) {
  if (reg <= Registers::FPSCR) {
    // All registers are 32 bits
    return RegisterInfo(reg, 0, 4);
  } else {
    return RegisterInfo(0, 0, 0);
  }
}

// Calling Convention
class SHCallingConvention final : public BN::CallingConvention {
 public:
  explicit SHCallingConvention(BN::Architecture *arch)
      : CallingConvention(arch, "SuperH") {}

  std::vector<uint32_t> GetIntegerArgumentRegisters() override {
    return std::vector{Registers::R4, Registers::R5, Registers::R6,
                       Registers::R7};
  }

  std::vector<uint32_t> GetFloatArgumentRegisters() override {
    return std::vector{Registers::FR4,  Registers::FR5, Registers::FR6,
                       Registers::FR7,  Registers::FR8, Registers::FR9,
                       Registers::FR10, Registers::FR11};
  }

  std::vector<uint32_t> GetCalleeSavedRegisters() override {
    return std::vector{Registers::R8,   Registers::R9,   Registers::R10,
                       Registers::R11,  Registers::R12,  Registers::R13,
                       Registers::R14,  Registers::R15,  Registers::MACH,
                       Registers::MACL, Registers::PR,   Registers::FR12,
                       Registers::FR13, Registers::FR14, Registers::FR15};
  }

  std::vector<uint32_t> GetCallerSavedRegisters() override {
    return std::vector{
        Registers::R0,   Registers::R1,   Registers::R2,   Registers::R3,
        Registers::R4,   Registers::R5,   Registers::R6,   Registers::R7,
        Registers::FR0,  Registers::FR1,  Registers::FR2,  Registers::FR3,
        Registers::FR4,  Registers::FR5,  Registers::FR6,  Registers::FR7,
        Registers::FR8,  Registers::FR9,  Registers::FR10, Registers::FR11,
        Registers::FPUL, Registers::FPSCR};
  }

  uint32_t GetIntegerReturnValueRegister() override { return Registers::R0; }

  uint32_t GetFloatReturnValueRegister() override { return Registers::FR0; }
};
}  // namespace SuperH

extern "C" {
BN_DECLARE_CORE_ABI_VERSION

BINARYNINJAPLUGIN bool CorePluginInit() {
  BN::Architecture *sh1 = new SuperH::SH1Architecture("superh-sh1");
  BN::Architecture::Register(sh1);
  const BN::Ref<BN::CallingConvention> sh1_cc =
      new SuperH::SHCallingConvention(sh1);
  sh1->RegisterCallingConvention(sh1_cc);
  sh1->SetDefaultCallingConvention(sh1_cc);

  BN::Architecture *sh2e = new SuperH::SH2EArchitecture("superh-sh2e");
  BN::Architecture::Register(sh2e);
  const BN::Ref<BN::CallingConvention> sh2e_cc =
      new SuperH::SHCallingConvention(sh2e);
  sh2e->RegisterCallingConvention(sh2e_cc);
  sh2e->SetDefaultCallingConvention(sh2e_cc);

  return true;
}
}
