// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include <binaryninjaapi.h>

#include "instructions.h"
#include "opcodes.h"
#include "registers.h"

#define ITEXT(instr) result.emplace_back(InstructionToken, instr);
#define SPACETEXT result.emplace_back(TextToken, " ");
#define OPSEPTEXT result.emplace_back(OperandSeparatorToken, ",");
#define REGTOFPREG(reg) reg += Registers::FR0;

namespace BN = BinaryNinja;

namespace SuperH {
/*
 * Text format methods
 */

void GenerateRegisterTokens(std::vector<BN::InstructionTextToken> &result,
                            RegTextInfo rti) {
  // The @ symbol for indirect addressing always appears first
  if (rti.indirect) {
    result.emplace_back(BeginMemoryOperandToken, "@");
  }
  // If we are using a displacement, the format is (disp,Reg)
  if (rti.use_disp) {
    result.emplace_back(TextToken, "(");

    char disp[19];
    std::snprintf(disp, std::size(disp), "%#lx", rti.disp);
    result.emplace_back(IntegerToken, disp, rti.disp);
    result.emplace_back(TextToken, ",");
  }
  // Some instructions are indexed from R0, formatted similar to the
  // displacement
  if (rti.indexed) {
    result.emplace_back(TextToken, "(");
    result.emplace_back(RegisterToken, Registers::to_string(Registers::R0));
    result.emplace_back(TextToken, ",");
  }
  // Add the pre-decrement operator
  if (rti.predec) {
    result.emplace_back(TextToken, "-");
  }

  // Add the actual register operand
  result.emplace_back(RegisterToken, Registers::to_string(rti.regnum));

  // Add the post-increment operator
  if (rti.postinc) {
    result.emplace_back(TextToken, "+");
  }
  // Close displacement
  if (rti.use_disp) {
    result.emplace_back(TextToken, ")");
  }
  // Close index
  if (rti.indexed) {
    result.emplace_back(TextToken, ")");
  }
}

// Produce text, in unsigned hex format, for immediate operand
void generateImmText(std::vector<BN::InstructionTextToken> &result,
                     const uint8_t imm8) {
  char immText[5];

  result.emplace_back(TextToken, "#");
  std::snprintf(immText, sizeof(immText), "%#hhx", imm8);
  result.emplace_back(IntegerToken, immText, imm8);
}

// Opcode text only; e.g. NOP
void Text0Format(std::vector<BN::InstructionTextToken> &result,
                 const std::string &instr) {
  ITEXT(instr)
}

// dst: direct register; e.g. MOVT Rn
void TextNFormatDstRn(const uint16_t opcode,
                      std::vector<BN::InstructionTextToken> &result,
                      const std::string &instr, const bool fp) {
  auto n = GetNFormatOpcodeField(opcode);
  if (fp) {
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
}

// dst: indirect register; e.g. TAS.B @Rn
void TextNFormatDstIndrRn(const uint16_t opcode,
                          std::vector<BN::InstructionTextToken> &result,
                          const std::string &instr) {
  const auto n = GetNFormatOpcodeField(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n, .indirect = true});
}

// src: control/sys reg, dst: direct reg; e.g. STS MACH, Rn
void TextNFormatSrcCtrlSysRegDstRn(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, const uint32_t ctrl_sys_reg, const bool fp) {
  auto n = GetNFormatOpcodeField(opcode);
  if (fp) {
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = ctrl_sys_reg});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
}

void TextNFormatSrcRmDstCtrlSysReg(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, const uint32_t ctrl_sys_reg, const bool fp) {
  auto n = GetNFormatOpcodeField(opcode);
  if (fp) {
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = ctrl_sys_reg});
}

void TextNFormatSrcIndrPostincRmDstCtrlSysReg(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, const uint32_t ctrl_sys_reg, const bool fp) {
  auto n = GetNFormatOpcodeField(opcode);
  if (fp) {
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(
      result, RegTextInfo{.regnum = n, .indirect = true, .postinc = true});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = ctrl_sys_reg});
}

// src: control/sys reg, dst: indirect, pre-decremented reg; e.g. STC.L SR,@-Rn
void TextNFormatSrcCtrlSysRegDstIndrPredecRn(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, const uint32_t ctrl_sys_reg, const bool fp) {
  auto n = GetNFormatOpcodeField(opcode);
  if (fp) {
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = ctrl_sys_reg});
  OPSEPTEXT
  GenerateRegisterTokens(
      result, RegTextInfo{.regnum = n, .indirect = true, .predec = true});
}

// src: direct reg, dst: control/sys reg; e.g. LDC Rm,SR
void TextMFormatSrcRmDstCtrlSysReg(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, const uint32_t ctrl_sys_reg) {
  const auto m = GetMFormatOpcodeField(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = m});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = ctrl_sys_reg});
}

// src: indirect, post-incremented reg, dst: control/sys reg; e.g. LDC.L @Rm+,SR
void TextMFormatSrcIndrPostincRmDstCtrlSysReg(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, const uint32_t ctrl_sys_reg) {
  const auto m = GetMFormatOpcodeField(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(
      result, RegTextInfo{.regnum = m, .indirect = true, .postinc = true});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = ctrl_sys_reg});
}

// src: indirect reg; e.g. JMP @Rm
void TextMFormatSrcIndrRm(const uint16_t opcode,
                          std::vector<BN::InstructionTextToken> &result,
                          const std::string &instr) {
  const auto m = GetMFormatOpcodeField(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = m, .indirect = true});
}

// src: PC relative reg; e.g. BRAF Rm
void TextMFormatSrcPCrelRm(const uint16_t opcode,
                           std::vector<BN::InstructionTextToken> &result,
                           const std::string &instr) {
  const auto m = GetMFormatOpcodeField(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = m});
}

// src/dst: direct registers; e.g. ADD Rm, Rn
void TextNMFormatSrcRmDstRn(const uint16_t opcode,
                            std::vector<BN::InstructionTextToken> &result,
                            const std::string &instr, const bool fp) {
  auto [n, m] = GetNMFormatOpcodeFields(opcode);
  if (fp) {
    REGTOFPREG(m)
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = m});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
}

// FPU instruction format FR0, FRm, FRn; e.g. FMAC FR0,FRm,FRn
void TextNMFormatFR0FRmFRn(const uint16_t opcode,
                           std::vector<BN::InstructionTextToken> &result,
                           const std::string &instr) {
  auto [n, m] = GetNMFormatOpcodeFields(opcode);
  REGTOFPREG(m)
  REGTOFPREG(n)

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = Registers::FR0});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = m});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
}

// src: direct reg, dst: indirect reg; e.g. MOV.L Rm, @Rn
void TextNMFormatSrcRmDstIndrRn(const uint16_t opcode,
                                std::vector<BN::InstructionTextToken> &result,
                                const std::string &instr, const bool fp) {
  auto [n, m] = GetNMFormatOpcodeFields(opcode);
  if (fp) {
    REGTOFPREG(m)
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = m});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n, .indirect = true});
}

// src: indirect reg, dst: direct reg; e.g. MOV.L @Rm, Rn
void TextNMFormatSrcIndrRmDstRn(const uint16_t opcode,
                                std::vector<BN::InstructionTextToken> &result,
                                const std::string &instr, const bool fp) {
  auto [n, m] = GetNMFormatOpcodeFields(opcode);
  if (fp) {
    REGTOFPREG(m)
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = m, .indirect = true});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
}

// src: n & m, indirect and post-incremented, dst: MACH, MACL; e.g. MAC.W
// @Rm+,@Rn+
void TextNMFormatSrcIndrPostincRmRnDstMAC(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr) {
  auto [n, m] = GetNMFormatOpcodeFields(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(
      result, RegTextInfo{.regnum = m, .indirect = true, .postinc = true});
  OPSEPTEXT
  GenerateRegisterTokens(
      result, RegTextInfo{.regnum = n, .indirect = true, .postinc = true});
}

// src: indirect, post-incremented reg, dst: direct reg; e.g. MOV.L @Rm+,Rn
void TextNMFormatSrcIndrPostincRmDstRn(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, const bool fp) {
  auto [n, m] = GetNMFormatOpcodeFields(opcode);
  if (fp) {
    REGTOFPREG(m)
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(
      result, RegTextInfo{.regnum = m, .indirect = true, .postinc = true});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
}

// src: direct reg, dst: indirect, pre-decremented reg; e.g. MOV.L Rm,@-Rn
void TextNMFormatSrcRmDstIndrPredecRn(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, const bool fp) {
  auto [n, m] = GetNMFormatOpcodeFields(opcode);
  if (fp) {
    REGTOFPREG(m)
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = m});
  OPSEPTEXT
  GenerateRegisterTokens(
      result, RegTextInfo{.regnum = n, .indirect = true, .predec = true});
}

// src: direct reg, dst: indirect, R0-indexed register; e.g. MOV.L Rm,@(R0,Rn)
void TextNMFormatSrcRmDstIndrIdxRn(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, const bool fp) {
  auto [n, m] = GetNMFormatOpcodeFields(opcode);
  if (fp) {
    REGTOFPREG(m)
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = m});
  OPSEPTEXT
  GenerateRegisterTokens(
      result, RegTextInfo{.regnum = n, .indirect = true, .indexed = true});
}

// src: indirect, R0-indexed register, dst: direct register; e.g. MOV.L
// @(R0,Rm),Rn
void TextNMFormatSrcIndrIdxRmDstRn(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, const bool fp) {
  auto [n, m] = GetNMFormatOpcodeFields(opcode);
  if (fp) {
    REGTOFPREG(m)
    REGTOFPREG(n)
  }

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(
      result, RegTextInfo{.regnum = m, .indirect = true, .indexed = true});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
}

// src: indirect reg w/displacement, dst: R0; e.g. MOV.B @(disp,Rm),R0
void TextMDFormatSrcIndrDispRmDstR0(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr) {
  const auto [m, d] = ExtractMDFormatOpcodeFields(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(
      result,
      RegTextInfo{.regnum = m, .indirect = true, .use_disp = true, .disp = d});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = Registers::R0});
}

// src: R0, dst: indirect reg w/displacement; e.g. MOV.B R0,@(disp,Rn)
void TextND4FormatSrcR0DstIndrDispRn(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr) {
  const auto [n, d] = ExtractND4FormatOpcodeFields(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = Registers::R0});
  OPSEPTEXT
  GenerateRegisterTokens(
      result,
      RegTextInfo{.regnum = n, .indirect = true, .use_disp = true, .disp = d});
}

// src: direct reg, dst: indirect reg w/displacement; e.g. MOV.L Rm,@(disp,Rn)
void TextNMDFormatSrcRmDstIndrDispRn(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr) {
  const auto [n, m, d] = ExtractNMDFormatOpcodeFields(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = m});
  OPSEPTEXT
  GenerateRegisterTokens(
      result,
      RegTextInfo{.regnum = n, .indirect = true, .use_disp = true, .disp = d});
}

// src: indirect reg w/displacement, dst: direct reg; e.g. MOV.L @(disp,Rm),Rn
void TextNMDFormatSrcIndrDispRmDstRn(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr) {
  const auto [n, m, d] = ExtractNMDFormatOpcodeFields(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(
      result,
      RegTextInfo{.regnum = m, .indirect = true, .use_disp = true, .disp = d});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
}

// src: indirect GBR reg w/displacement, dst: R0; e.g. MOV.L @(disp,GBR),R0
void TextDFormatSrcIndrGBRDispDstR0(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr) {
  const auto d = ExtractDFormatOpcodeFields(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = Registers::GBR,
                                             .indirect = true,
                                             .use_disp = true,
                                             .disp = d});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = Registers::R0});
}

// src: R0, dst: indirect GBR reg w/displacement; e.g. MOV.L R0,@(disp,GBR)
void TextDFormatSrcR0DstIndrGBRDisp(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr) {
  const auto d = ExtractDFormatOpcodeFields(opcode);

  ITEXT(instr)
  SPACETEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = Registers::R0});
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = Registers::GBR,
                                             .indirect = true,
                                             .use_disp = true,
                                             .disp = d});
}

// This text format method is specific to one instruction
// src: indirect PC-relative displacement, dst: R0; e.g. MOVA @(disp,PC),R0 -->
// display as MOVA #addr, R0
void MOVATextDFormatSrcIndrPCrelDispDstR0(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, uint64_t addr) {
  const auto d = ExtractDFormatOpcodeFields(opcode);
  const auto target = (static_cast<uint64_t>(d) * 4) + (addr & 0xFFFFFFFC) +
                      (2 * INSTRUCTION_SIZE);
  std::ostringstream ss;
  ss << "0x" << std::hex << target;

  ITEXT(instr)
  SPACETEXT
  result.emplace_back(TextToken, "#");
  result.emplace_back(PossibleAddressToken, ss.str(), target);
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = Registers::R0});
}

// src: PC-relative label; e.g. BF label
void TextDFormatSrcPCrel(std::vector<BN::InstructionTextToken> &result,
                         const std::string &instr, uint32_t target) {
  ITEXT(instr)
  SPACETEXT
  std::ostringstream ss;
  ss << "0x" << std::hex << target;
  result.emplace_back(PossibleAddressToken, ss.str(), target);
}

// src: PC-relative label; e.g. BRA label
void TextD12FormatSrcPCrel(std::vector<BN::InstructionTextToken> &result,
                           const std::string &instr, uint32_t target) {
  ITEXT(instr)
  SPACETEXT
  std::ostringstream ss;
  ss << "0x" << std::hex << target;
  result.emplace_back(PossibleAddressToken, ss.str(), target);
}

// This text format method is specific to two instructions
// src: PC-relative with displacement, dst: direct register; e.g. MOV.L
// @(disp,PC),Rn --> display as MOV.L #addr, Rn
void MOVWLTextND8FormatSrcPCrelDispDstRn(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, size_t size, uint64_t addr) {
  const auto [n, d] = ExtractND8FormatOpcodeFields(opcode);

  std::ostringstream ss;
  uint64_t target;

  if (size == 2) {
    target = (static_cast<uint64_t>(d) * 2) + addr + (2 * INSTRUCTION_SIZE);
    ss << "0x" << std::hex << target;
  } else if (size == 4) {
    target = (static_cast<uint64_t>(d) * 4) + (addr & 0xFFFFFFFC) +
             (2 * INSTRUCTION_SIZE);
    ss << "0x" << std::hex << target;
  } else {
    BN::LogError(
        "unexpected size (%zu) passed to MOVWLTextND8FormatSrcPCrelDispDstRn",
        size);
    return;
  }
  ITEXT(instr)
  SPACETEXT
  result.emplace_back(TextToken, "@");
  result.emplace_back(PossibleAddressToken, ss.str(), target);
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
}

// src: immediate value, dst: indirect, R0-indexed GDB reg; e.g. AND.B
// #imm,@(R0,GBR)
void TextIFormatSrcImmDstIndrIdxGBR(
    const uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr) {
  const auto i = GetIFormatOpcodeField(opcode);

  ITEXT(instr)
  SPACETEXT
  generateImmText(result, i);
  OPSEPTEXT
  GenerateRegisterTokens(
      result,
      RegTextInfo{.regnum = Registers::GBR, .indirect = true, .indexed = true});
}

// src: immediate value, dst: R0; e.g. AND.B #imm,@(R0,GBR)
void TextIFormatSrcImmDstR0(const uint16_t opcode,
                            std::vector<BN::InstructionTextToken> &result,
                            const std::string &instr) {
  const auto i = GetIFormatOpcodeField(opcode);

  ITEXT(instr)
  SPACETEXT
  generateImmText(result, i);
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = Registers::R0});
}

// src: immediate value; e.g. TRAPA #imm
void TextIFormatSrcImm(const uint16_t opcode,
                       std::vector<BN::InstructionTextToken> &result,
                       const std::string &instr) {
  const auto i = GetIFormatOpcodeField(opcode);

  ITEXT(instr)
  SPACETEXT
  generateImmText(result, i);
}

// src: immediate value, dst: direct reg; e.g. ADD #imm,Rn
void TextNIFormatSrcImmDstRn(const uint16_t opcode,
                             std::vector<BN::InstructionTextToken> &result,
                             const std::string &instr) {
  const auto [n, i] = ExtractNIFormatOpcodeFields(opcode);

  ITEXT(instr)
  SPACETEXT
  generateImmText(result, i);
  OPSEPTEXT
  GenerateRegisterTokens(result, RegTextInfo{.regnum = n});
}

/*
 * Instruction text methods
 */
bool AddRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "ADD");
  return true;
}

bool AddImmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNIFormatSrcImmDstRn(opcode, result, "ADD");
  return true;
}

bool AddcRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "ADDC");
  return true;
}

bool AddvRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "ADDV");
  return true;
}

bool AndRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "AND");
  return true;
}

bool AndImmR0::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextIFormatSrcImmDstR0(opcode, result, "AND");
  return true;
}

bool AndbImmIndrR0Gbr::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  TextIFormatSrcImmDstIndrIdxGBR(opcode, result, "AND.B");
  return true;
}

bool BfDisp::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  TextDFormatSrcPCrel(result, "BF", GetTarget(opcode, addr));
  return true;
}

bool BfsDisp::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() == SH_1_ISA) {
    // unimplemented on SH-1
    return false;
  }
  TextDFormatSrcPCrel(result, "BF/S", GetTarget(opcode, addr));
  return true;
}

bool BraDisp::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextD12FormatSrcPCrel(result, "BRA", GetTarget(opcode, addr));
  return true;
}

bool BrafRm::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() == SH_1_ISA) {
    // unimplemented on SH-1
    return false;
  }
  TextMFormatSrcPCrelRm(opcode, result, "BRAF");
  return true;
}

bool BsrDisp::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextD12FormatSrcPCrel(result, "BSR", GetTarget(opcode, addr));
  return true;
}

bool BsrfRm::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() == SH_1_ISA) {
    // unimplemented on SH-1
    return false;
  }
  TextMFormatSrcPCrelRm(opcode, result, "BSRF");
  return true;
}

bool BtDisp::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  TextDFormatSrcPCrel(result, "BT", GetTarget(opcode, addr));
  return true;
}

bool BtsDisp::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() == SH_1_ISA) {
    // unimplemented on SH-1
    return false;
  }
  TextDFormatSrcPCrel(result, "BT/S", GetTarget(opcode, addr));
  return true;
}

bool Clrmac::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  Text0Format(result, "CLRMAC");
  return true;
}

bool Clrt::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                std::vector<BN::InstructionTextToken> &result) {
  Text0Format(result, "CLRT");
  return true;
}

bool CmpEqRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "CMP/EQ");
  return true;
}

bool CmpGeRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "CMP/GE");
  return true;
}

bool CmpGtRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "CMP/GT");
  return true;
}

bool CmpHiRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "CMP/HI");
  return true;
}

bool CmpHsRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "CMP/HS");
  return true;
}

bool CmpPlRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "CMP/PL");
  return true;
}

bool CmpPzRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "CMP/PZ");
  return true;
}

bool CmpStrRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "CMP/STR");
  return true;
}

bool CmpEqImmR0::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  TextIFormatSrcImmDstR0(opcode, result, "CMP/EQ");
  return true;
}

bool Div0sRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "DIV0S");
  return true;
}

bool Div0u::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                 std::vector<BN::InstructionTextToken> &result) {
  Text0Format(result, "DIV0U");
  return true;
}

bool Div1RmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "DIV1");
  return true;
}

bool DmulslRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() == SH_1_ISA) {
    // unimplemented on SH-1
    return false;
  }
  TextNMFormatSrcRmDstRn(opcode, result, "DMULS.L");
  return true;
}

bool DmululRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() == SH_1_ISA) {
    // unimplemented on SH-1
    return false;
  }
  TextNMFormatSrcRmDstRn(opcode, result, "DMULU.L");
  return true;
}

bool DtRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() == SH_1_ISA) {
    // unimplemented on SH-1
    return false;
  }
  TextNFormatDstRn(opcode, result, "DT");
  return true;
}

bool ExtsbRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "EXTS.B");
  return true;
}

bool ExtswRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "EXTS.W");
  return true;
}

bool ExtubRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "EXTU.B");
  return true;
}

bool ExtuwRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "EXTU.W");
  return true;
}

bool JmpIndrRm::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcIndrRm(opcode, result, "JMP");
  return true;
}

bool JsrIndrRm::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcIndrRm(opcode, result, "JSR");
  return true;
}

bool LdcRmSr::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcRmDstCtrlSysReg(opcode, result, "LDC", Registers::SR);
  return true;
}

bool LdcRmGbr::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcRmDstCtrlSysReg(opcode, result, "LDC", Registers::GBR);
  return true;
}

bool LdcRmVbr::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcRmDstCtrlSysReg(opcode, result, "LDC", Registers::VBR);
  return true;
}

bool LdclIndrRmPostincSr::Text(const uint16_t opcode, const uint64_t addr,
                               size_t &len,
                               std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcIndrPostincRmDstCtrlSysReg(opcode, result, "LDC.L",
                                           Registers::SR);
  return true;
}

bool LdclIndrRmPostincGbr::Text(const uint16_t opcode, const uint64_t addr,
                                size_t &len,
                                std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcIndrPostincRmDstCtrlSysReg(opcode, result, "LDC.L",
                                           Registers::GBR);
  return true;
}

bool LdclIndrRmPostincVbr::Text(const uint16_t opcode, const uint64_t addr,
                                size_t &len,
                                std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcIndrPostincRmDstCtrlSysReg(opcode, result, "LDC.L",
                                           Registers::VBR);
  return true;
}

bool LdsRmMach::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcRmDstCtrlSysReg(opcode, result, "LDS", Registers::MACH);
  return true;
}

bool LdsRmMacl::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcRmDstCtrlSysReg(opcode, result, "LDS", Registers::MACL);
  return true;
}

bool LdsRmPr::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcRmDstCtrlSysReg(opcode, result, "LDS", Registers::PR);
  return true;
}

bool LdslIndrRmPostincMach::Text(
    const uint16_t opcode, const uint64_t addr, size_t &len,
    std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcIndrPostincRmDstCtrlSysReg(opcode, result, "LDS.L",
                                           Registers::MACH);
  return true;
}

bool LdslIndrRmPostincMacl::Text(
    const uint16_t opcode, const uint64_t addr, size_t &len,
    std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcIndrPostincRmDstCtrlSysReg(opcode, result, "LDS.L",
                                           Registers::MACL);
  return true;
}

bool LdslIndrRmPostincPr::Text(const uint16_t opcode, const uint64_t addr,
                               size_t &len,
                               std::vector<BN::InstructionTextToken> &result) {
  TextMFormatSrcIndrPostincRmDstCtrlSysReg(opcode, result, "LDS.L",
                                           Registers::PR);
  return true;
}

bool MaclIndrRmPostincIndrRnPostinc::Text(
    const uint16_t opcode, const uint64_t addr, size_t &len,
    std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() == SH_1_ISA) {
    // unimplemented on SH-1
    return false;
  }
  TextNMFormatSrcIndrPostincRmRnDstMAC(opcode, result, "MAC.L");
  return true;
}

bool MacwIndrRmPostincIndrRnPostinc::Text(
    const uint16_t opcode, const uint64_t addr, size_t &len,
    std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() == SH_1_ISA) {
    // unimplemented on SH-1
    return false;
  }
  TextNMFormatSrcIndrPostincRmRnDstMAC(opcode, result, "MAC.W");
  return true;
}

bool MacIndrRmPostincIndrRnPostinc::Text(
    const uint16_t opcode, const uint64_t addr, size_t &len,
    std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcIndrPostincRmRnDstMAC(opcode, result, "MAC");
  return true;
}

bool MovRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "MOV");
  return true;
}

bool MovbRmIndrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                        std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstIndrRn(opcode, result, "MOV.B");
  return true;
}

bool MovwRmIndrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                        std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstIndrRn(opcode, result, "MOV.W");
  return true;
}

bool MovlRmIndrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                        std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstIndrRn(opcode, result, "MOV.L");
  return true;
}

bool MovbIndrRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                        std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcIndrRmDstRn(opcode, result, "MOV.B");
  return true;
}

bool MovwIndrRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                        std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcIndrRmDstRn(opcode, result, "MOV.W");
  return true;
}

bool MovlIndrRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                        std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcIndrRmDstRn(opcode, result, "MOV.L");
  return true;
}

bool MovbRmIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                              size_t &len,
                              std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstIndrPredecRn(opcode, result, "MOV.B");
  return true;
}

bool MovwRmIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                              size_t &len,
                              std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstIndrPredecRn(opcode, result, "MOV.W");
  return true;
}

bool MovlRmIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                              size_t &len,
                              std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstIndrPredecRn(opcode, result, "MOV.L");
  return true;
}

bool MovbIndrRmPostincRn::Text(const uint16_t opcode, const uint64_t addr,
                               size_t &len,
                               std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcIndrPostincRmDstRn(opcode, result, "MOV.B");
  return true;
}

bool MovwIndrRmPostincRn::Text(const uint16_t opcode, const uint64_t addr,
                               size_t &len,
                               std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcIndrPostincRmDstRn(opcode, result, "MOV.W");
  return true;
}

bool MovlIndrRmPostincRn::Text(const uint16_t opcode, const uint64_t addr,
                               size_t &len,
                               std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcIndrPostincRmDstRn(opcode, result, "MOV.L");
  return true;
}

bool MovbRmIndrR0Rn::Text(const uint16_t opcode, const uint64_t addr,
                          size_t &len,
                          std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstIndrIdxRn(opcode, result, "MOV.B");
  return true;
}

bool MovwRmIndrR0Rn::Text(const uint16_t opcode, const uint64_t addr,
                          size_t &len,
                          std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstIndrIdxRn(opcode, result, "MOV.W");
  return true;
}

bool MovlRmIndrR0Rn::Text(const uint16_t opcode, const uint64_t addr,
                          size_t &len,
                          std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstIndrIdxRn(opcode, result, "MOV.L");
  return true;
}

bool MovbIndrR0RmRn::Text(const uint16_t opcode, const uint64_t addr,
                          size_t &len,
                          std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcIndrIdxRmDstRn(opcode, result, "MOV.B");
  return true;
}

bool MovwIndrR0RmRn::Text(const uint16_t opcode, const uint64_t addr,
                          size_t &len,
                          std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcIndrIdxRmDstRn(opcode, result, "MOV.W");
  return true;
}

bool MovlIndrR0RmRn::Text(const uint16_t opcode, const uint64_t addr,
                          size_t &len,
                          std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcIndrIdxRmDstRn(opcode, result, "MOV.L");
  return true;
}

bool MovImmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNIFormatSrcImmDstRn(opcode, result, "MOV");
  return true;
}

bool MovwIndrDispPcRn::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  MOVWLTextND8FormatSrcPCrelDispDstRn(opcode, result, "MOV.W", 2, addr);
  return true;
}

bool MovlIndrDispPcRn::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  MOVWLTextND8FormatSrcPCrelDispDstRn(opcode, result, "MOV.L", 4, addr);
  return true;
}

bool MovbIndrDispGbrR0::Text(const uint16_t opcode, const uint64_t addr,
                             size_t &len,
                             std::vector<BN::InstructionTextToken> &result) {
  TextDFormatSrcIndrGBRDispDstR0(opcode, result, "MOV.B");
  return true;
}

bool MovwIndrDispGbrR0::Text(const uint16_t opcode, const uint64_t addr,
                             size_t &len,
                             std::vector<BN::InstructionTextToken> &result) {
  TextDFormatSrcIndrGBRDispDstR0(opcode, result, "MOV.W");
  return true;
}

bool MovlIndrDispGbrR0::Text(const uint16_t opcode, const uint64_t addr,
                             size_t &len,
                             std::vector<BN::InstructionTextToken> &result) {
  TextDFormatSrcIndrGBRDispDstR0(opcode, result, "MOV.L");
  return true;
}

bool MovbR0IndrDispGbr::Text(const uint16_t opcode, const uint64_t addr,
                             size_t &len,
                             std::vector<BN::InstructionTextToken> &result) {
  TextDFormatSrcR0DstIndrGBRDisp(opcode, result, "MOV.B");
  return true;
}

bool MovwR0IndrDispGbr::Text(const uint16_t opcode, const uint64_t addr,
                             size_t &len,
                             std::vector<BN::InstructionTextToken> &result) {
  TextDFormatSrcR0DstIndrGBRDisp(opcode, result, "MOV.W");
  return true;
}

bool MovlR0IndrDispGbr::Text(const uint16_t opcode, const uint64_t addr,
                             size_t &len,
                             std::vector<BN::InstructionTextToken> &result) {
  TextDFormatSrcR0DstIndrGBRDisp(opcode, result, "MOV.L");
  return true;
}

bool MovbR0IndrDispRn::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  TextND4FormatSrcR0DstIndrDispRn(opcode, result, "MOV.B");
  return true;
}

bool MovwR0IndrDispRn::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  TextND4FormatSrcR0DstIndrDispRn(opcode, result, "MOV.W");
  return true;
}

bool MovlRmIndrDispRn::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  TextNMDFormatSrcRmDstIndrDispRn(opcode, result, "MOV.L");
  return true;
}

bool MovbIndrDispRmR0::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  TextMDFormatSrcIndrDispRmDstR0(opcode, result, "MOV.B");
  return true;
}

bool MovwIndrDispRmR0::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  TextMDFormatSrcIndrDispRmDstR0(opcode, result, "MOV.W");
  return true;
}

bool MovlIndrDispRmRn::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  TextNMDFormatSrcIndrDispRmDstRn(opcode, result, "MOV.L");
  return true;
}

bool MovaIndrDispPcR0::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  MOVATextDFormatSrcIndrPCrelDispDstR0(opcode, result, "MOVA", addr);
  return true;
}

bool MovtRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "MOVT");
  return true;
}

bool MullRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() == SH_1_ISA) {
    // unimplemented on SH-1
    return false;
  }
  TextNMFormatSrcRmDstRn(opcode, result, "MUL.L");
  return true;
}

bool MulswRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "MULS.W");
  return true;
}

bool MulsRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "MULS");
  return true;
}

bool MuluwRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "MULU.W");
  return true;
}

bool MuluRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "MULU");
  return true;
}

bool NegRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "NEG");
  return true;
}

bool NegcRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "NEGC");
  return true;
}

bool Nop::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
               std::vector<BN::InstructionTextToken> &result) {
  Text0Format(result, "NOP");
  return true;
}

bool NotRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "NOT");
  return true;
}

bool OrRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "OR");
  return true;
}

bool OrImmR0::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextIFormatSrcImmDstR0(opcode, result, "OR");
  return true;
}

bool OrbImmIndrR0Gbr::Text(const uint16_t opcode, const uint64_t addr,
                           size_t &len,
                           std::vector<BN::InstructionTextToken> &result) {
  TextIFormatSrcImmDstIndrIdxGBR(opcode, result, "OR.B");
  return true;
}

bool RotclRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "ROTCL");
  return true;
}

bool RotcrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "ROTCR");
  return true;
}

bool RotlRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "ROTL");
  return true;
}

bool RotrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "ROTR");
  return true;
}

bool Rte::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
               std::vector<BN::InstructionTextToken> &result) {
  Text0Format(result, "RTE");
  return true;
}

bool Rts::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
               std::vector<BN::InstructionTextToken> &result) {
  Text0Format(result, "RTS");
  return true;
}

bool Sett::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                std::vector<BN::InstructionTextToken> &result) {
  Text0Format(result, "SETT");
  return true;
}

bool ShalRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "SHAL");
  return true;
}

bool SharRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "SHAR");
  return true;
}

bool ShllRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "SHLL");
  return true;
}

bool Shll2Rn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "SHLL2");
  return true;
}

bool Shll8Rn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "SHLL8");
  return true;
}

bool Shll16Rn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "SHLL16");
  return true;
}

bool ShlrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                  std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "SHLR");
  return true;
}

bool Shlr2Rn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "SHLR2");
  return true;
}

bool Shlr8Rn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "SHLR8");
  return true;
}

bool Shlr16Rn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstRn(opcode, result, "SHLR16");
  return true;
}

bool Sleep::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                 std::vector<BN::InstructionTextToken> &result) {
  Text0Format(result, "SLEEP");
  return true;
}

bool StcSrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstRn(opcode, result, "STC", Registers::SR);
  return true;
}

bool StcGbrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstRn(opcode, result, "STC", Registers::GBR);
  return true;
}

bool StcVbrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstRn(opcode, result, "STC", Registers::VBR);
  return true;
}

bool StclSrIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                              size_t &len,
                              std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstIndrPredecRn(opcode, result, "STC.L",
                                          Registers::SR);
  return true;
}

bool StclGbrIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                               size_t &len,
                               std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstIndrPredecRn(opcode, result, "STC.L",
                                          Registers::GBR);
  return true;
}

bool StclVbrIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                               size_t &len,
                               std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstIndrPredecRn(opcode, result, "STC.L",
                                          Registers::VBR);
  return true;
}

bool StsMachRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstRn(opcode, result, "STS", Registers::MACH);
  return true;
}

bool StsMaclRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstRn(opcode, result, "STS", Registers::MACL);
  return true;
}

bool StsPrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstRn(opcode, result, "STS", Registers::PR);
  return true;
}

bool StslMachIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                                size_t &len,
                                std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstIndrPredecRn(opcode, result, "STS.L",
                                          Registers::MACH);
  return true;
}

bool StslMaclIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                                size_t &len,
                                std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstIndrPredecRn(opcode, result, "STS.L",
                                          Registers::MACL);
  return true;
}

bool StslPrIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                              size_t &len,
                              std::vector<BN::InstructionTextToken> &result) {
  TextNFormatSrcCtrlSysRegDstIndrPredecRn(opcode, result, "STS.L",
                                          Registers::PR);
  return true;
}

bool SubRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "SUB");
  return true;
}

bool SubcRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "SUBC");
  return true;
}

bool SubvRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "SUBV");
  return true;
}

bool SwapbRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "SWAP.B");
  return true;
}

bool SwapwRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "SWAP.W");
  return true;
}

bool TasbIndrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  TextNFormatDstIndrRn(opcode, result, "TAS.B");
  return true;
}

bool TrapaImm::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextIFormatSrcImm(opcode, result, "TRAPA");
  return true;
}

bool TstRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "TST");
  return true;
}

bool TstImmR0::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextIFormatSrcImmDstR0(opcode, result, "TST");
  return true;
}

bool TstbImmIndrR0Gbr::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  TextIFormatSrcImmDstIndrIdxGBR(opcode, result, "TST.B");
  return true;
}

bool XorRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "XOR");
  return true;
}

bool XorImmR0::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  TextIFormatSrcImmDstR0(opcode, result, "XOR");
  return true;
}

bool XorbImmIndrR0Gbr::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  TextIFormatSrcImmDstIndrIdxGBR(opcode, result, "XOR.B");
  return true;
}

bool XtrctRmRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  TextNMFormatSrcRmDstRn(opcode, result, "XTRCT");
  return true;
}

/*
 * Floating Point Instructions
 * SH-2E Only
 */

bool FabsFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatDstRn(opcode, result, "FABS", true);
  return true;
}

bool FaddFrmFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcRmDstRn(opcode, result, "FADD", true);
  return true;
}

bool FcmpEqFrmFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                        std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcRmDstRn(opcode, result, "FCMP/EQ", true);
  return true;
}

bool FcmpGtFrmFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                        std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcRmDstRn(opcode, result, "FCMP/GT", true);
  return true;
}

bool FdivFrmFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcRmDstRn(opcode, result, "FDIV", true);
  return true;
}

bool Fldi0Frn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatDstRn(opcode, result, "FLDI0", true);
  return true;
}

bool Fldi1Frn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatDstRn(opcode, result, "FLDI1", true);
  return true;
}

bool FldsFrmFpul::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                       std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcRmDstCtrlSysReg(opcode, result, "FLDS", Registers::FPUL, true);
  return true;
}

bool FloatFpulFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                        std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcCtrlSysRegDstRn(opcode, result, "FLOAT", Registers::FPUL, true);
  return true;
}

bool FmacFr0FrmFrn::Text(const uint16_t opcode, const uint64_t addr,
                         size_t &len,
                         std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatFR0FRmFRn(opcode, result, "FMAC");
  return true;
}

bool FmovFrmFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcRmDstRn(opcode, result, "FMOV", true);
  return true;
}

bool FmovsIndrRmFrn::Text(const uint16_t opcode, const uint64_t addr,
                          size_t &len,
                          std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcIndrRmDstRn(opcode, result, "FMOV.S", true);
  return true;
}

bool FmovsFrmIndrRn::Text(const uint16_t opcode, const uint64_t addr,
                          size_t &len,
                          std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcRmDstIndrRn(opcode, result, "FMOV.S", true);
  return true;
}

bool FmovsIndrRmPostincFrn::Text(
    const uint16_t opcode, const uint64_t addr, size_t &len,
    std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcIndrPostincRmDstRn(opcode, result, "FMOV.S", true);
  return true;
}

bool FmovsFrmIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                                size_t &len,
                                std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcRmDstIndrPredecRn(opcode, result, "FMOV.S", true);
  return true;
}

bool FmovsIndrR0RmFrn::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcIndrIdxRmDstRn(opcode, result, "FMOV.S", true);
  return true;
}

bool FmovsFrmIndrR0Rn::Text(const uint16_t opcode, const uint64_t addr,
                            size_t &len,
                            std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcRmDstIndrIdxRn(opcode, result, "FMOV.S", true);
  return true;
}

bool FmulFrmFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcRmDstRn(opcode, result, "FMUL", true);
  return true;
}

bool FnegFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                   std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatDstRn(opcode, result, "FNEG", true);
  return true;
}

bool FstsFpulFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                       std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcCtrlSysRegDstRn(opcode, result, "FSTS", Registers::FPUL, true);
  return true;
}

bool FsubFrmFrn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNMFormatSrcRmDstRn(opcode, result, "FSUB", true);
  return true;
}

bool FtrcFrmFpul::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                       std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcRmDstCtrlSysReg(opcode, result, "FTRC", Registers::FPUL, true);
  return true;
}

bool LdsRmFpul::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcRmDstCtrlSysReg(opcode, result, "LDS", Registers::FPUL, true);
  return true;
}

bool LdslIndrRmPostincFpul::Text(
    const uint16_t opcode, const uint64_t addr, size_t &len,
    std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcIndrPostincRmDstCtrlSysReg(opcode, result, "LDS.L",
                                           Registers::FPUL, true);
  return true;
}

bool LdsRmFpscr::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcRmDstCtrlSysReg(opcode, result, "LDS", Registers::FPSCR, true);
  return true;
}

bool LdslIndrRmPostincFpscr::Text(
    const uint16_t opcode, const uint64_t addr, size_t &len,
    std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcIndrPostincRmDstCtrlSysReg(opcode, result, "LDS.L",
                                           Registers::FPSCR, true);
  return true;
}

bool StsFpulRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                     std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcCtrlSysRegDstRn(opcode, result, "STS", Registers::FPUL, true);
  return true;
}

bool StslFpulIndrPredecRn::Text(const uint16_t opcode, const uint64_t addr,
                                size_t &len,
                                std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcCtrlSysRegDstIndrPredecRn(opcode, result, "STS.L",
                                          Registers::FPUL, true);
  return true;
}

bool StsFpscrRn::Text(const uint16_t opcode, const uint64_t addr, size_t &len,
                      std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcCtrlSysRegDstRn(opcode, result, "STS", Registers::FPSCR, true);
  return true;
}

bool StslFpscrIndrPredecRn::Text(
    const uint16_t opcode, const uint64_t addr, size_t &len,
    std::vector<BN::InstructionTextToken> &result) {
  if (this->GetIsaType() != SH_2E_ISA) {
    return false;
  }
  TextNFormatSrcCtrlSysRegDstIndrPredecRn(opcode, result, "STS.L",
                                          Registers::FPSCR, true);
  return true;
}
}  // namespace SuperH
