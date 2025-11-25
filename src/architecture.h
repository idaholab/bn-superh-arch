// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef SRC_ARCHITECTURE_H_
#define SRC_ARCHITECTURE_H_

#include <binaryninjaapi.h>

#include <cstdint>

#include "instructions.h"

namespace BN = BinaryNinja;

namespace SuperH {

class Architecture : public BN::Architecture {
 protected:
  static BNRegisterInfo RegisterInfo(uint32_t fullWidthReg, size_t offset,
                                     size_t size, bool zeroExtend = false);

 public:
  IsaType isa_type;

  explicit Architecture(const std::string& name);

  size_t GetAddressSize() const override;
  BNEndianness GetEndianness() const override;
  size_t GetDefaultIntegerSize() const override;
  size_t GetInstructionAlignment() const override;
  size_t GetMaxInstructionLength() const override;
  bool GetInstructionInfo(const uint8_t* data, uint64_t addr, size_t maxLen,
                          BN::InstructionInfo& result) override;
  bool GetInstructionText(
      const uint8_t* data, uint64_t addr, size_t& len,
      std::vector<BN::InstructionTextToken>& result) override;
  bool GetInstructionLowLevelIL(const uint8_t* data, uint64_t addr, size_t& len,
                                BN::LowLevelILFunction& il) override;
  std::string GetRegisterName(uint32_t reg) override;
  uint32_t GetStackPointerRegister() override;
};

class SH1Architecture final : public Architecture {
 public:
  explicit SH1Architecture(const std::string& name);

  std::vector<uint32_t> GetAllRegisters() override;
  BNRegisterInfo GetRegisterInfo(uint32_t reg) override;
};

class SH2EArchitecture final : public Architecture {
 public:
  explicit SH2EArchitecture(const std::string& name);

  std::vector<uint32_t> GetAllRegisters() override;
  BNRegisterInfo GetRegisterInfo(uint32_t reg) override;
};

}  // namespace SuperH

#endif  // SRC_ARCHITECTURE_H_
