// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include <binaryninjaapi.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

#include "architecture.h"
#include "instructions.h"
#include "opcodes.h"
#include "registers.h"

namespace BN = BinaryNinja;
namespace SH = SuperH;

// Convert an array of instruction text tokens to a string
static std::string tokens_to_string(
    const std::vector<BN::InstructionTextToken> &tokens) {
  std::string out;

  for (const auto &token : tokens) {
    out.append(token.text);
  }

  return out;
}

// Compare two text token vectors
static void compare_text_tokens(
    const std::vector<BN::InstructionTextToken> &a,
    const std::vector<BN::InstructionTextToken> &b) {
  EXPECT_EQ(a.size(), b.size());
  for (size_t i = 0; i < std::min(a.size(), b.size()); i++) {
    EXPECT_EQ(a[i].type, b[i].type)
        << "token types @ index " << i << " do not match";
  }

  const auto a_assembly = tokens_to_string(a);
  const auto b_assembly = tokens_to_string(b);

  EXPECT_EQ(a_assembly, b_assembly) << "accumulated token strings do not match";
}

// Verify that an architecture returns the correct text tokens
static void test_architecture_text(
    const uint16_t opcode, const uint64_t address,
    const std::vector<BN::InstructionTextToken> &want) {
  const auto arch = std::make_unique<SH::SH1Architecture>("shtest");

  const std::array<uint8_t, 2> bytes = {
      static_cast<uint8_t>((opcode & 0xFF00) >> 8),
      static_cast<uint8_t>(opcode & 0x00FF),
  };

  size_t len = 0;
  auto got = std::vector<BN::InstructionTextToken>{};

  EXPECT_TRUE(arch->GetInstructionText(bytes.data(), address, len, got));
  EXPECT_EQ(len, SH::Sizes::WORD);
  compare_text_tokens(got, want);
}

// Format: OP
class TestText0 : public ::testing::TestWithParam<int> {};

TEST_P(TestText0, TestText) {
  const uint16_t opcode = GetParam();
  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, SH::Opcodes::NAMES.at(opcode)},
  };

  test_architecture_text(opcode, 0x0, want);
}

// Test all 'OP' format instructions
INSTANTIATE_TEST_SUITE_P(
    TestTextFormat0, TestText0,
    ::testing::Values(SH::Opcodes::Clrmac, SH::Opcodes::Clrt,
                      SH::Opcodes::Div0u, SH::Opcodes::Nop, SH::Opcodes::Rte,
                      SH::Opcodes::Rts, SH::Opcodes::Sett, SH::Opcodes::Sleep),
    [](const testing::TestParamInfo<TestText0::ParamType> &info) {
      const auto op = SH::Opcodes::NAMES.at(info.param);
      return "OP_" + op;
    });

// Format: OP RN
class TestTextRn : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(TestTextRn, TestText) {
  uint16_t opcode = std::get<0>(GetParam());
  const SH::N rn = std::get<1>(GetParam());

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, SH::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {RegisterToken, SH::Registers::to_string(rn)},
  };

  opcode = SH::SetNFormatOpcodeField(opcode, rn);
  test_architecture_text(opcode, 0x0, want);
}

// Test all 'OP RN' format instructions for all possible values of RN
INSTANTIATE_TEST_SUITE_P(
    TestTextFormatRn, TestTextRn,
    ::testing::Combine(
        ::testing::Values(SH::Opcodes::CmpPlRn, SH::Opcodes::CmpPzRn,
                          SH::Opcodes::MovtRn, SH::Opcodes::RotlRn,
                          SH::Opcodes::RotrRn, SH::Opcodes::ShalRn,
                          SH::Opcodes::SharRn, SH::Opcodes::ShllRn,
                          SH::Opcodes::ShlrRn),
        ::testing::Range(0, 16)),  // All gp registers
    [](const testing::TestParamInfo<TestTextRn::ParamType> &info) {
      auto op = SH::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto rn = SH::Registers::to_string(std::get<1>(info.param));

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_RN_" + rn;
    });

// Format: OP RM,RN
class TestTextRmRn
    : public ::testing::TestWithParam<std::tuple<int, int, int>> {};

TEST_P(TestTextRmRn, TestText) {
  uint16_t opcode = std::get<0>(GetParam());
  const SH::M rm = std::get<1>(GetParam());
  const SH::N rn = std::get<2>(GetParam());

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, SH::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {RegisterToken, SH::Registers::to_string(rm)},
      {OperandSeparatorToken, ","},
      {RegisterToken, SH::Registers::to_string(rn)},
  };

  opcode = SH::SetNMFormatOpcodeFields(opcode, rn, rm);
  test_architecture_text(opcode, 0x0, want);
}

// Test all 'OP RM/RN' format instructions for all combinations of RM & RN
INSTANTIATE_TEST_SUITE_P(
    TestTextFormatRmRn, TestTextRmRn,
    ::testing::Combine(
        ::testing::Values(SH::Opcodes::AddRmRn, SH::Opcodes::AddcRmRn,
                          SH::Opcodes::AddvRmRn, SH::Opcodes::AndRmRn,
                          SH::Opcodes::CmpEqRmRn, SH::Opcodes::CmpGeRmRn,
                          SH::Opcodes::CmpGtRmRn, SH::Opcodes::CmpHiRmRn,
                          SH::Opcodes::CmpHsRmRn, SH::Opcodes::CmpStrRmRn,
                          SH::Opcodes::Div0sRmRn, SH::Opcodes::Div1RmRn,
                          SH::Opcodes::ExtsbRmRn, SH::Opcodes::ExtswRmRn,
                          SH::Opcodes::ExtubRmRn, SH::Opcodes::ExtuwRmRn,
                          SH::Opcodes::MovRmRn, SH::Opcodes::MulswRmRn,
                          SH::Opcodes::MuluwRmRn, SH::Opcodes::NegRmRn,
                          SH::Opcodes::NegcRmRn, SH::Opcodes::NotRmRn,
                          SH::Opcodes::OrRmRn, SH::Opcodes::SubRmRn,
                          SH::Opcodes::SubcRmRn, SH::Opcodes::SubvRmRn,
                          SH::Opcodes::SwapbRmRn, SH::Opcodes::SwapwRmRn,
                          SH::Opcodes::TstRmRn, SH::Opcodes::XorRmRn,
                          SH::Opcodes::XtrctRmRn),
        ::testing::Range(0, 16), ::testing::Range(0, 16)),
    [](const testing::TestParamInfo<TestTextRmRn::ParamType> &info) {
      auto op = SH::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto rm = SH::Registers::to_string(std::get<1>(info.param));
      const auto rn = SH::Registers::to_string(std::get<2>(info.param));

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_RM_" + rm + "_RN_" + rn;
    });
