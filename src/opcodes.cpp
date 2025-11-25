// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "opcodes.h"

namespace SuperH {

namespace Opcodes {
// Construct a list of all instructions
std::vector<int> GetAllOpcodes() {
  std::vector<int> opcodes;
  opcodes.reserve(Opcodes::NAMES.size());
  for (const auto& [opcode, _] : Opcodes::NAMES) {
    opcodes.push_back(opcode);
  }
  return opcodes;
}

// Construct a list of all simple (non-branching) instructions
std::vector<int> GetSimpleOpcodes() {
  std::vector<int> opcodes;
  opcodes.reserve(Opcodes::NAMES.size());
  for (const auto& [opcode, _] : Opcodes::NAMES) {
    if (Opcodes::BRANCHING.find(opcode) == Opcodes::BRANCHING.end()) {
      opcodes.push_back(opcode);
    }
  }
  return opcodes;
}
}  // namespace Opcodes

static uint8_t mask_and_shift_out(const uint16_t opcode, const uint16_t mask,
                                  const uint8_t shift) {
  return (opcode & mask) >> shift;
}

std::pair<N, M> GetNMFormatOpcodeFields(const uint16_t opcode) {
  const auto n = mask_and_shift_out(opcode, 0x0F00, 8);
  const auto m = mask_and_shift_out(opcode, 0x00F0, 4);

  return std::make_pair(n, m);
}

uint16_t SetNMFormatOpcodeFields(const uint16_t opcode, const N n, const M m) {
  return opcode | static_cast<uint16_t>(n) << 8 | static_cast<uint16_t>(m) << 4;
}

N GetNFormatOpcodeField(const uint16_t opcode) {
  return mask_and_shift_out(opcode, 0x0F00, 8);
}

uint16_t SetNFormatOpcodeField(const uint16_t opcode, const N n) {
  return opcode | static_cast<uint16_t>(n) << 8;
}

M GetMFormatOpcodeField(const uint16_t opcode) {
  return mask_and_shift_out(opcode, 0x0F00, 8);
}

uint16_t SetMFormatOpcodeField(const uint16_t opcode, const M m) {
  return opcode | static_cast<uint16_t>(m) << 8;
}

I GetIFormatOpcodeField(const uint16_t opcode) { return opcode & 0x00FF; }

uint16_t SetIFormatOpcodeField(const uint16_t opcode, const I i) {
  return opcode | static_cast<uint16_t>(i);
}

std::pair<M, D> ExtractMDFormatOpcodeFields(const uint16_t opcode) {
  const auto m = mask_and_shift_out(opcode, 0x00F0, 4);
  const auto d = opcode & 0x000F;
  return std::make_pair(m, d);
}

std::pair<N, D> ExtractND4FormatOpcodeFields(const uint16_t opcode) {
  const auto n = mask_and_shift_out(opcode, 0x00F0, 4);
  const auto d = opcode & 0x000F;
  return std::make_pair(n, d);
}

std::tuple<N, M, D> ExtractNMDFormatOpcodeFields(const uint16_t opcode) {
  const auto n = mask_and_shift_out(opcode, 0x0F00, 8);
  const auto m = mask_and_shift_out(opcode, 0x00F0, 4);
  const auto d = opcode & 0x000F;
  return std::make_tuple(n, m, d);
}

D ExtractDFormatOpcodeFields(const uint16_t opcode) { return opcode & 0x00FF; }

D12 ExtractD12FormatOpcodeFields(const uint16_t opcode) {
  return opcode & 0x0FFF;
}

std::pair<N, D> ExtractND8FormatOpcodeFields(const uint16_t opcode) {
  const auto n = mask_and_shift_out(opcode, 0x0F00, 8);
  const auto d = opcode & 0x00FF;
  return std::make_pair(n, d);
}

std::pair<N, I> ExtractNIFormatOpcodeFields(const uint16_t opcode) {
  const auto n = (opcode & 0x0F00) >> 8;
  const auto i = opcode & 0x00FF;
  return std::make_pair(n, i);
}
}  // namespace SuperH
