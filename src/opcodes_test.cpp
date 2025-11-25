// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "opcodes.h"

#include <gtest/gtest.h>

namespace SH = SuperH;

// Test the extraction of opcode fields
//
// LEGEND
//
// xxxx: opcode
// mmmm: source register
// nnnn: destination register
// iiii: immediate data
// dddd: displacement

// n format: xxxx nnnn xxxx xxxx
TEST(OpcodesTest, ExtractN) {
  constexpr uint8_t n = 0b1111;
  constexpr uint16_t opcode = 0b1010000010101010 | static_cast<uint16_t>(n)
                                                       << 8;

  EXPECT_EQ(n, SH::GetNFormatOpcodeField(opcode));
}

// m format: xxxx mmmm xxxx xxxx
TEST(OpcodesTest, ExtractM) {
  constexpr uint8_t m = 0b1001;
  constexpr uint16_t opcode = 0b1010000010101010 | static_cast<uint16_t>(m)
                                                       << 8;

  EXPECT_EQ(m, SH::GetMFormatOpcodeField(opcode));
}

// nm format: xxxx nnnn mmmm xxxx
TEST(OpcodesTest, GetNM) {
  constexpr uint8_t n = 0b1010;
  constexpr uint8_t m = 0b0101;
  const uint16_t opcode = SH::SetNMFormatOpcodeFields(0b1100000000001111, n, m);

  constexpr auto want = std::make_pair(n, m);

  EXPECT_EQ(want, SH::GetNMFormatOpcodeFields(opcode));
}

// md format: xxxx xxxx mmmm dddd
TEST(OpcodesTest, ExtractMD) {
  constexpr uint8_t m = 0b1010;
  constexpr uint8_t d = 0b0101;
  constexpr uint16_t opcode =
      0b11001100 << 8 | static_cast<uint16_t>(m) << 4 | d;

  constexpr auto want = std::make_pair(m, d);

  EXPECT_EQ(want, SH::ExtractMDFormatOpcodeFields(opcode));
}

// nd4 format: xxxx xxxx mmmm dddd
TEST(OpcodesTest, ExtractND4) {
  constexpr uint8_t m = 0b1010;
  constexpr uint8_t d = 0b0101;
  constexpr uint16_t opcode =
      0b11001100 << 8 | static_cast<uint16_t>(m) << 4 | d;

  constexpr auto want = std::make_pair(m, d);

  EXPECT_EQ(want, SH::ExtractND4FormatOpcodeFields(opcode));
}

// nmd format: xxxx nnnn mmmm dddd
TEST(OpcodesTest, ExtractNMD) {
  constexpr uint8_t n = 0b1010;
  constexpr uint8_t m = 0b0101;
  constexpr uint8_t d = 0b1001;
  constexpr uint16_t opcode = 0b1100 << 12 | static_cast<uint16_t>(n) << 8 |
                              static_cast<uint16_t>(m) << 4 | d;

  constexpr auto want = std::make_tuple(n, m, d);

  EXPECT_EQ(want, SH::ExtractNMDFormatOpcodeFields(opcode));
}

// d format: xxxx xxxx dddd dddd
TEST(OpcodesTest, ExtractD) {
  constexpr uint8_t d = 0b11001100;
  constexpr uint16_t opcode = 0b10101010 << 8 | d;

  EXPECT_EQ(d, SH::ExtractDFormatOpcodeFields(opcode));
}

// d12 format: xxxx dddd dddd dddd
TEST(OpcodesTest, ExtractD12) {
  constexpr uint16_t d = 0b110011001100;
  constexpr uint16_t opcode = 0b1010 << 12 | static_cast<uint16_t>(d);

  EXPECT_EQ(d, SH::ExtractD12FormatOpcodeFields(opcode));
}

// nd8 format: xxxx nnnn dddd dddd
TEST(OpcodesTest, ExtractND8) {
  constexpr uint8_t n = 0b1010;
  constexpr uint8_t d = 0b11001100;
  constexpr uint16_t opcode = 0b1111 << 12 | static_cast<uint16_t>(n) << 8 | d;

  constexpr auto want = std::make_pair(n, d);

  EXPECT_EQ(want, SH::ExtractND8FormatOpcodeFields(opcode));
}

// i format: xxxx xxxx iiii iiii
TEST(OpcodesTest, ExtractI) {
  constexpr uint8_t i = 0b1010;
  constexpr uint16_t opcode = 0b11001100 << 8 | static_cast<uint16_t>(i);

  EXPECT_EQ(i, SH::GetIFormatOpcodeField(opcode));
}

// ni format: xxxx nnnn iiii iiii
TEST(OpcodesTest, ExtractNI) {
  constexpr uint8_t n = 0b1010;
  constexpr uint8_t i = 0b01010101;
  constexpr uint16_t opcode = 0b1100 << 12 | static_cast<uint16_t>(n) << 8 | i;

  constexpr auto want = std::make_pair(n, i);

  EXPECT_EQ(want, SH::ExtractNIFormatOpcodeFields(opcode));
}
