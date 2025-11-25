// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include <binaryninjaapi.h>

#include <cstdint>

#include "instructions.h"
#include "opcodes.h"

namespace SuperH {
// Default Info -- applies to all instructions except SH-DSP
bool Instruction::Info(uint16_t opcode, uint64_t addr,
                       BN::InstructionInfo &result) {
  result.length = length;
  return true;
}

// Branch if false
bool BfDisp::Info(const uint16_t opcode, const uint64_t addr,
                  BN::InstructionInfo &result) {
  result.length = length;
  result.AddBranch(TrueBranch, GetTarget(opcode, addr));
  result.AddBranch(FalseBranch, addr + result.length);
  return true;
}

uint32_t BfDisp::GetTarget(const uint16_t opcode, const uint64_t addr) {
  int64_t disp;

  if (const auto d = ExtractDFormatOpcodeFields(opcode); (d & 0x80) == 0) {
    // positive displacement -- jump forward
    disp = (0x000000FF & static_cast<int64_t>(d));
  } else {
    // negative displacement -- jump back
    disp = (0xFFFFFF00 | static_cast<int64_t>(d));
  }
  return (addr + 2 * INSTRUCTION_SIZE + (disp << 1));
}

// Branch if false with delay slot
bool BfsDisp::Info(const uint16_t opcode, const uint64_t addr,
                   BN::InstructionInfo &result) {
  result.length = length;

  switch (this->GetIsaType()) {
    case SH_1_ISA:
      // unimplemented on SH-1
      return false;
    default:
      result.AddBranch(TrueBranch, GetTarget(opcode, addr), nullptr, true);
      result.AddBranch(FalseBranch, addr + (2 * result.length), nullptr, true);
      return true;
  }
}

uint32_t BfsDisp::GetTarget(const uint16_t opcode, const uint64_t addr) {
  int64_t disp;

  if (const auto d = ExtractDFormatOpcodeFields(opcode); (d & 0x80) == 0) {
    // positive displacement -- jump forward
    disp = (0x000000FF & static_cast<int64_t>(d));
  } else {
    // negative displacement -- jump back
    disp = (0xFFFFFF00 | static_cast<int64_t>(d));
  }
  return (addr + 2 * INSTRUCTION_SIZE + (disp << 1));
}

// Branch (unconditional)
bool BraDisp::Info(const uint16_t opcode, const uint64_t addr,
                   BN::InstructionInfo &result) {
  result.length = length;
  result.AddBranch(UnconditionalBranch, GetTarget(opcode, addr), nullptr, true);
  return true;
}

uint32_t BraDisp::GetTarget(const uint16_t opcode, const uint64_t addr) {
  int64_t disp;

  if (const auto d = ExtractD12FormatOpcodeFields(opcode); (d & 0x800) == 0) {
    // positive displacement -- jump forward
    disp = (0x00000FFF & static_cast<int64_t>(d));
  } else {
    // negative displacement -- jump back
    disp = (0xFFFFF000 | static_cast<int64_t>(d));
  }
  return (addr + 2 * INSTRUCTION_SIZE + (disp << 1));
}

// Branch far
bool BrafRm::Info(const uint16_t opcode, const uint64_t addr,
                  BN::InstructionInfo &result) {
  result.length = length;
  switch (this->GetIsaType()) {
    case SH_1_ISA:
      // unimplemented on SH-1
      return false;
    default:
      result.AddBranch(UnconditionalBranch, GetTarget(opcode, addr), nullptr,
                       true);
      return true;
  }
}

uint32_t BrafRm::GetTarget(const uint16_t opcode, const uint64_t addr) {
  const auto m = GetMFormatOpcodeField(opcode);
  return (addr + 2 * INSTRUCTION_SIZE + m);
}

// Branch to subroutine
bool BsrDisp::Info(const uint16_t opcode, const uint64_t addr,
                   BN::InstructionInfo &result) {
  result.length = length;
  result.AddBranch(CallDestination, GetTarget(opcode, addr), nullptr, true);
  return true;
}

uint32_t BsrDisp::GetTarget(const uint16_t opcode, const uint64_t addr) {
  int64_t disp;

  if (const auto d = ExtractD12FormatOpcodeFields(opcode); (d & 0x800) == 0) {
    disp = 0x00000FFF & static_cast<int64_t>(d);
  } else {
    disp = 0xFFFFF000 | static_cast<int64_t>(d);
  }
  return (addr + 2 * INSTRUCTION_SIZE + (disp << 1));
}

// Branch to subroutine far
bool BsrfRm::Info(const uint16_t opcode, const uint64_t addr,
                  BN::InstructionInfo &result) {
  result.length = length;
  switch (this->GetIsaType()) {
    case SH_1_ISA:
      // unimplemented on SH-1
      return false;
    default:
      result.AddBranch(CallDestination, GetTarget(opcode, addr), nullptr, true);
      return true;
  }
}

uint32_t BsrfRm::GetTarget(const uint16_t opcode, const uint64_t addr) {
  const auto m = GetMFormatOpcodeField(opcode);
  return (addr + 2 * INSTRUCTION_SIZE + m);
}

// Branch if true
bool BtDisp::Info(const uint16_t opcode, const uint64_t addr,
                  BN::InstructionInfo &result) {
  result.length = length;
  result.AddBranch(TrueBranch, GetTarget(opcode, addr));
  result.AddBranch(FalseBranch, addr + result.length);
  return true;
}

uint32_t BtDisp::GetTarget(const uint16_t opcode, const uint64_t addr) {
  int64_t disp;

  if (const auto d = ExtractDFormatOpcodeFields(opcode); (d & 0x80) == 0) {
    // positive displacement -- jump forward
    disp = (0x000000FF & static_cast<int64_t>(d));
  } else {
    // negative displacement -- jump back
    disp = (0xFFFFFF00 | static_cast<int64_t>(d));
  }
  return (addr + 2 * INSTRUCTION_SIZE + (disp << 1));
}

// Branch if true with delay slot
bool BtsDisp::Info(const uint16_t opcode, const uint64_t addr,
                   BN::InstructionInfo &result) {
  result.length = length;
  switch (this->GetIsaType()) {
    case SH_1_ISA:
      // unimplemented on SH-1
      return false;
    default:
      result.AddBranch(TrueBranch, GetTarget(opcode, addr), nullptr, true);
      result.AddBranch(FalseBranch, addr + (2 * result.length), nullptr, true);
      return true;
  }
}

uint32_t BtsDisp::GetTarget(const uint16_t opcode, const uint64_t addr) {
  int64_t disp;

  if (const auto d = ExtractDFormatOpcodeFields(opcode); (d & 0x80) == 0) {
    // positive displacement -- jump forward
    disp = (0x000000FF & static_cast<int64_t>(d));
  } else {
    // negative displacement -- jump back
    disp = (0xFFFFFF00 | static_cast<int64_t>(d));
  }
  return (addr + 2 * INSTRUCTION_SIZE + (disp << 1));
}

bool JmpIndrRm::Info(const uint16_t opcode, const uint64_t addr,
                     BN::InstructionInfo &result) {
  result.length = length;
  result.AddBranch(IndirectBranch, GetTarget(opcode, addr), nullptr, true);
  return true;
}

uint32_t JmpIndrRm::GetTarget(const uint16_t opcode, const uint64_t addr) {
  const auto m = GetMFormatOpcodeField(opcode);
  return m;
}

bool JsrIndrRm::Info(const uint16_t opcode, const uint64_t addr,
                     BN::InstructionInfo &result) {
  result.length = length;
  result.AddBranch(CallDestination, GetTarget(opcode, addr), nullptr, true);
  return true;
}

uint32_t JsrIndrRm::GetTarget(const uint16_t opcode, const uint64_t addr) {
  const auto m = GetMFormatOpcodeField(opcode);
  return m;
}

bool Rte::Info(const uint16_t opcode, const uint64_t addr,
               BN::InstructionInfo &result) {
  result.length = length;
  result.AddBranch(FunctionReturn, 0, nullptr, true);
  return true;
}

bool Rts::Info(const uint16_t opcode, const uint64_t addr,
               BN::InstructionInfo &result) {
  result.length = length;
  result.AddBranch(FunctionReturn, 0, nullptr, true);
  return true;
}
}  // namespace SuperH
