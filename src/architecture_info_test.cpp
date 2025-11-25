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

namespace BN = BinaryNinja;
namespace SH = SuperH;

// Test that all instructions are decoded
class TestInfoAllInstructions : public ::testing::TestWithParam<int> {};

TEST_P(TestInfoAllInstructions, TestInfo) {
  const uint16_t opcode = GetParam();

  const std::array<uint8_t, 2> bytes = {
      static_cast<uint8_t>((opcode & 0xFF00) >> 8),
      static_cast<uint8_t>(opcode & 0x00FF),
  };

  const auto arch = std::make_unique<SH::SH1Architecture>("shtest");
  auto result = BN::InstructionInfo{};

  EXPECT_TRUE(
      arch->GetInstructionInfo(bytes.data(), 0x0, bytes.size(), result));
  EXPECT_EQ(result.length, SH::Instruction::length);
}

// Test all instructions
INSTANTIATE_TEST_SUITE_P(
    TestAll, TestInfoAllInstructions,
    ::testing::ValuesIn(SH::Opcodes::GetAllOpcodes()),
    [](const testing::TestParamInfo<TestInfoAllInstructions::ParamType>& info) {
      auto op = SH::Opcodes::NAMES.at(info.param);

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op;
    });

// Test instruction info for simple (non-branching) instructions
class TestInfoSimpleInstructions : public ::testing::TestWithParam<int> {};

// Inspect simple instructions
// 1. No branch targets are set
TEST_P(TestInfoSimpleInstructions, TestInfo) {
  const uint16_t opcode = GetParam();

  const std::array<uint8_t, 2> bytes = {
      static_cast<uint8_t>((opcode & 0xFF00) >> 8),
      static_cast<uint8_t>(opcode & 0x00FF),
  };

  const auto arch = std::make_unique<SH::SH1Architecture>("shtest");
  auto result = BN::InstructionInfo{};

  arch->GetInstructionInfo(bytes.data(), 0x0, bytes.size(), result);
  EXPECT_EQ(result.branchCount, 0);
}

// Test all non-branching instructions
INSTANTIATE_TEST_SUITE_P(
    TestNonBranching, TestInfoSimpleInstructions,
    ::testing::ValuesIn(SH::Opcodes::GetSimpleOpcodes()),
    [](const testing::TestParamInfo<TestInfoSimpleInstructions::ParamType>&
           info) {
      auto op = SH::Opcodes::NAMES.at(info.param);

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op;
    });

// Test instruction info for return instructions
class TestInfoReturnInstructions : public ::testing::TestWithParam<int> {};

TEST_P(TestInfoReturnInstructions, TestInfo) {
  const uint16_t opcode = GetParam();

  const std::array<uint8_t, 2> bytes = {
      static_cast<uint8_t>((opcode & 0xFF00) >> 8),
      static_cast<uint8_t>(opcode & 0x00FF),
  };

  const auto arch = std::make_unique<SH::SH1Architecture>("shtest");
  auto result = BN::InstructionInfo{};

  arch->GetInstructionInfo(bytes.data(), 0x0, bytes.size(), result);
  EXPECT_EQ(result.branchCount, 1);
  EXPECT_EQ(*result.branchTarget, 0x0);
  EXPECT_EQ(*result.branchType, FunctionReturn);
  EXPECT_EQ(result.delaySlots, true);
}

// Test all non-branching instructions
INSTANTIATE_TEST_SUITE_P(
    TestReturns, TestInfoReturnInstructions,
    ::testing::Values(SH::Opcodes::Rte, SH::Opcodes::Rts),
    [](const testing::TestParamInfo<TestInfoReturnInstructions::ParamType>&
           info) {
      auto op = SH::Opcodes::NAMES.at(info.param);

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op;
    });

// TODO: Add tests for complex branching instructions