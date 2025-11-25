// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef SRC_INSTRUCTIONS_H_
#define SRC_INSTRUCTIONS_H_

#include <binaryninjaapi.h>

#include <cstdint>
#include <string>
#include <vector>

#include "sizes.h"

#define INSTRUCTION_SIZE 2

namespace BN = BinaryNinja;

namespace SuperH {
enum IsaType { SH_1_ISA, SH_2E_ISA, SH_DSP_ISA };

/*
 * Text format methods
 */
struct RegTextInfo {
  uint32_t regnum;
  bool indirect, predec, postinc, use_disp, indexed;
  int64_t disp;
};

static void GenerateRegisterTokens(
    std::vector<BN::InstructionTextToken> &result, RegTextInfo rti);

static void generateImmText(std::vector<BN::InstructionTextToken> &result,
                            uint8_t imm8);

static void Text0Format(std::vector<BN::InstructionTextToken> &result,
                        const std::string &instr);

static void TextNFormatDstRn(uint16_t opcode,
                             std::vector<BN::InstructionTextToken> &result,
                             const std::string &instr, bool fp = false);

static void TextNFormatDstIndrRn(uint16_t opcode,
                                 std::vector<BN::InstructionTextToken> &result,
                                 const std::string &instr);

static void TextNFormatSrcCtrlSysRegDstRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, uint32_t ctrl_sys_reg, bool fp = false);

static void TextNFormatSrcRmDstCtrlSysReg(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, uint32_t ctrl_sys_reg, bool fp = false);

static void TextNFormatSrcIndrPostincRmDstCtrlSysReg(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, uint32_t ctrl_sys_reg, bool fp = false);

static void TextNFormatSrcCtrlSysRegDstIndrPredecRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, uint32_t ctrl_sys_reg, bool fp = false);

static void TextMFormatSrcRmDstCtrlSysReg(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, uint32_t ctrl_sys_reg);

static void TextMFormatSrcIndrPostincRmDstCtrlSysReg(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, uint32_t ctrl_sys_reg);

static void TextMFormatSrcIndrRm(uint16_t opcode,
                                 std::vector<BN::InstructionTextToken> &result,
                                 const std::string &instr);

static void TextMFormatSrcPCrelRm(uint16_t opcode,
                                  std::vector<BN::InstructionTextToken> &result,
                                  const std::string &instr);

static void TextNMFormatSrcRmDstRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, bool fp = false);

static void TextNMFormatFR0FRmFRn(uint16_t opcode,
                                  std::vector<BN::InstructionTextToken> &result,
                                  const std::string &instr);

static void TextNMFormatSrcRmDstIndrRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, bool fp = false);

static void TextNMFormatSrcIndrRmDstRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, bool fp = false);

void TextNMFormatSrcIndrPostincRmRnDstMAC(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr);

void TextNMFormatSrcIndrPostincRmDstRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, bool fp = false);

void TextNMFormatSrcRmDstIndrPredecRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, bool fp = false);

void TextNMFormatSrcRmDstIndrIdxRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, bool fp = false);

void TextNMFormatSrcIndrIdxRmDstRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, bool fp = false);

void TextMDFormatSrcIndrDispRmDstR0(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr);

void TextND4FormatSrcR0DstIndrDispRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr);

void TextNMDFormatSrcRmDstIndrDispRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr);

void TextNMDFormatSrcIndrDispRmDstRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr);

void TextDFormatSrcIndrGBRDispDstR0(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr);

void TextDFormatSrcR0DstIndrGBRDisp(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr);

void MOVATextDFormatSrcIndrPCrelDispDstR0(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, uint64_t addr);

void TextDFormatSrcPCrel(std::vector<BN::InstructionTextToken> &result,
                         const std::string &instr, uint32_t target);

void TextD12FormatSrcPCrel(std::vector<BN::InstructionTextToken> &result,
                           const std::string &instr, uint32_t target);

void MOVWLTextND8FormatSrcPCrelDispDstRn(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr, size_t size, uint64_t addr);

void TextIFormatSrcImmDstIndrIdxGBR(
    uint16_t opcode, std::vector<BN::InstructionTextToken> &result,
    const std::string &instr);

void TextIFormatSrcImmDstR0(uint16_t opcode,
                            std::vector<BN::InstructionTextToken> &result,
                            const std::string &instr);

void TextIFormatSrcImm(uint16_t opcode,
                       std::vector<BN::InstructionTextToken> &result,
                       const std::string &instr);

void TextNIFormatSrcImmDstRn(uint16_t opcode,
                             std::vector<BN::InstructionTextToken> &result,
                             const std::string &instr);

/*
 * Abstract instruction class that all instructions must use.
 * This allows the return type of the decoder function (DecodeInstruction) to
 * be generic.
 */
class Instruction {
  IsaType isa_type;

 public:
  virtual ~Instruction() = default;
  explicit Instruction(const IsaType &t);

  static constexpr size_t length = Sizes::WORD;

  virtual bool Text(uint16_t opcode, uint64_t addr, size_t &len,
                    std::vector<BN::InstructionTextToken> &result) = 0;

  virtual bool Info(uint16_t opcode, uint64_t addr,
                    BN::InstructionInfo &result);

  virtual bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
                    BN::LowLevelILFunction &il, BN::Architecture *arch);

  [[nodiscard]] IsaType GetIsaType()
      const;  // can be used by each method to make isa-specific decisions
};

std::optional<std::unique_ptr<Instruction>> DecodeInstruction(const IsaType &t,
                                                              uint16_t opcode);

std::optional<std::unique_ptr<Instruction>> ParsePrefix0000(const IsaType &t,
                                                            uint16_t opcode);

std::optional<std::unique_ptr<Instruction>> ParsePrefix0010(const IsaType &t,
                                                            uint16_t opcode);

std::optional<std::unique_ptr<Instruction>> ParsePrefix0011(const IsaType &t,
                                                            uint16_t opcode);

std::optional<std::unique_ptr<Instruction>> ParsePrefix0100(const IsaType &t,
                                                            uint16_t opcode);

std::optional<std::unique_ptr<Instruction>> ParsePrefix0110(const IsaType &t,
                                                            uint16_t opcode);

std::optional<std::unique_ptr<Instruction>> ParsePrefix1000(const IsaType &t,
                                                            uint16_t opcode);

std::optional<std::unique_ptr<Instruction>> ParsePrefix1100(const IsaType &t,
                                                            uint16_t opcode);

std::optional<std::unique_ptr<Instruction>> ParsePrefix1111(const IsaType &t,
                                                            uint16_t opcode);

/*
 * Concrete classes for CPU instructions that should be returned from
 * DecodeInstruction.
 */
class AddRmRn final : public Instruction {
 public:
  explicit AddRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class AddImmRn final : public Instruction {
 public:
  explicit AddImmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class AddcRmRn final : public Instruction {
 public:
  explicit AddcRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class AddvRmRn final : public Instruction {
 public:
  explicit AddvRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class AndRmRn final : public Instruction {
 public:
  explicit AndRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class AndImmR0 final : public Instruction {
 public:
  explicit AndImmR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class AndbImmIndrR0Gbr final : public Instruction {
 public:
  explicit AndbImmIndrR0Gbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class BfDisp final : public Instruction {
 public:
  explicit BfDisp(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;

  static uint32_t GetTarget(uint16_t opcode, uint64_t addr);
};

class BfsDisp final : public Instruction {
 public:
  explicit BfsDisp(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  // TODO: Lift

  static uint32_t GetTarget(uint16_t opcode, uint64_t addr);
};

class BraDisp final : public Instruction {
 public:
  explicit BraDisp(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  // TODO: Lift

  static uint32_t GetTarget(uint16_t opcode, uint64_t addr);
};

class BrafRm final : public Instruction {
 public:
  explicit BrafRm(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  // TODO: Lift

  static uint32_t GetTarget(uint16_t opcode, uint64_t addr);
};

class BsrDisp final : public Instruction {
 public:
  explicit BsrDisp(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  // TODO: Lift

  static uint32_t GetTarget(uint16_t opcode, uint64_t addr);
};

class BsrfRm final : public Instruction {
 public:
  explicit BsrfRm(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  // TODO: Lift

  static uint32_t GetTarget(uint16_t opcode, uint64_t addr);
};

class BtDisp final : public Instruction {
 public:
  explicit BtDisp(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;

  static uint32_t GetTarget(uint16_t opcode, uint64_t addr);
};

class BtsDisp final : public Instruction {
 public:
  explicit BtsDisp(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  // TODO: Lift

  static uint32_t GetTarget(uint16_t opcode, uint64_t addr);
};

class Clrmac final : public Instruction {
 public:
  explicit Clrmac(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Clrt final : public Instruction {
 public:
  explicit Clrt(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class CmpEqRmRn final : public Instruction {
 public:
  explicit CmpEqRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class CmpGeRmRn final : public Instruction {
 public:
  explicit CmpGeRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class CmpGtRmRn final : public Instruction {
 public:
  explicit CmpGtRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class CmpHiRmRn final : public Instruction {
 public:
  explicit CmpHiRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class CmpHsRmRn final : public Instruction {
 public:
  explicit CmpHsRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class CmpPlRn final : public Instruction {
 public:
  explicit CmpPlRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class CmpPzRn final : public Instruction {
 public:
  explicit CmpPzRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class CmpStrRmRn final : public Instruction {
 public:
  explicit CmpStrRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class CmpEqImmR0 final : public Instruction {
 public:
  explicit CmpEqImmR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Div0sRmRn final : public Instruction {
 public:
  explicit Div0sRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Div0u final : public Instruction {
 public:
  explicit Div0u(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Div1RmRn final : public Instruction {
 public:
  explicit Div1RmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class DmulslRmRn final : public Instruction {
 public:
  explicit DmulslRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class DmululRmRn final : public Instruction {
 public:
  explicit DmululRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class DtRn final : public Instruction {
 public:
  explicit DtRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class ExtsbRmRn final : public Instruction {
 public:
  explicit ExtsbRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class ExtswRmRn final : public Instruction {
 public:
  explicit ExtswRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class ExtubRmRn final : public Instruction {
 public:
  explicit ExtubRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class ExtuwRmRn final : public Instruction {
 public:
  explicit ExtuwRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FabsFrn final : public Instruction {
 public:
  explicit FabsFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FaddFrmFrn final : public Instruction {
 public:
  explicit FaddFrmFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FcmpEqFrmFrn final : public Instruction {
 public:
  explicit FcmpEqFrmFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FcmpGtFrmFrn final : public Instruction {
 public:
  explicit FcmpGtFrmFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FdivFrmFrn final : public Instruction {
 public:
  explicit FdivFrmFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Fldi0Frn final : public Instruction {
 public:
  explicit Fldi0Frn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Fldi1Frn final : public Instruction {
 public:
  explicit Fldi1Frn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FldsFrmFpul final : public Instruction {
 public:
  explicit FldsFrmFpul(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FloatFpulFrn final : public Instruction {
 public:
  explicit FloatFpulFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FmacFr0FrmFrn final : public Instruction {
 public:
  explicit FmacFr0FrmFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FmovFrmFrn final : public Instruction {
 public:
  explicit FmovFrmFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FmovsIndrR0RmFrn final : public Instruction {
 public:
  explicit FmovsIndrR0RmFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FmovsIndrRmPostincFrn final : public Instruction {
 public:
  explicit FmovsIndrRmPostincFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FmovsIndrRmFrn final : public Instruction {
 public:
  explicit FmovsIndrRmFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FmovsFrmIndrR0Rn final : public Instruction {
 public:
  explicit FmovsFrmIndrR0Rn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FmovsFrmIndrPredecRn final : public Instruction {
 public:
  explicit FmovsFrmIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FmovsFrmIndrRn final : public Instruction {
 public:
  explicit FmovsFrmIndrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FmulFrmFrn final : public Instruction {
 public:
  explicit FmulFrmFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FnegFrn final : public Instruction {
 public:
  explicit FnegFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FstsFpulFrn final : public Instruction {
 public:
  explicit FstsFpulFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FsubFrmFrn final : public Instruction {
 public:
  explicit FsubFrmFrn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class FtrcFrmFpul final : public Instruction {
 public:
  explicit FtrcFrmFpul(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdsRmFpscr final : public Instruction {
 public:
  explicit LdsRmFpscr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdsRmFpul final : public Instruction {
 public:
  explicit LdsRmFpul(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdslIndrRmPostincFpscr final : public Instruction {
 public:
  explicit LdslIndrRmPostincFpscr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdslIndrRmPostincFpul final : public Instruction {
 public:
  explicit LdslIndrRmPostincFpul(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StsFpscrRn final : public Instruction {
 public:
  explicit StsFpscrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StsFpulRn final : public Instruction {
 public:
  explicit StsFpulRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StslFpscrIndrPredecRn final : public Instruction {
 public:
  explicit StslFpscrIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StslFpulIndrPredecRn final : public Instruction {
 public:
  explicit StslFpulIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class JmpIndrRm final : public Instruction {
 public:
  explicit JmpIndrRm(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;

  static uint32_t GetTarget(uint16_t opcode, uint64_t addr);
};

class JsrIndrRm final : public Instruction {
 public:
  explicit JsrIndrRm(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;

  static uint32_t GetTarget(uint16_t opcode, uint64_t addr);
};

class LdcRmSr final : public Instruction {
 public:
  explicit LdcRmSr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdcRmGbr final : public Instruction {
 public:
  explicit LdcRmGbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdcRmVbr final : public Instruction {
 public:
  explicit LdcRmVbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdclIndrRmPostincSr final : public Instruction {
 public:
  explicit LdclIndrRmPostincSr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdclIndrRmPostincGbr final : public Instruction {
 public:
  explicit LdclIndrRmPostincGbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdclIndrRmPostincVbr final : public Instruction {
 public:
  explicit LdclIndrRmPostincVbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdsRmMach final : public Instruction {
 public:
  explicit LdsRmMach(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdsRmMacl final : public Instruction {
 public:
  explicit LdsRmMacl(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdsRmPr final : public Instruction {
 public:
  explicit LdsRmPr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdslIndrRmPostincMach final : public Instruction {
 public:
  explicit LdslIndrRmPostincMach(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdslIndrRmPostincMacl final : public Instruction {
 public:
  explicit LdslIndrRmPostincMacl(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class LdslIndrRmPostincPr final : public Instruction {
 public:
  explicit LdslIndrRmPostincPr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class MaclIndrRmPostincIndrRnPostinc final : public Instruction {
 public:
  explicit MaclIndrRmPostincIndrRnPostinc(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class MacwIndrRmPostincIndrRnPostinc final : public Instruction {
 public:
  explicit MacwIndrRmPostincIndrRnPostinc(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class MacIndrRmPostincIndrRnPostinc final : public Instruction {
 public:
  explicit MacIndrRmPostincIndrRnPostinc(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class MovRmRn final : public Instruction {
 public:
  explicit MovRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovbRmIndrRn final : public Instruction {
 public:
  explicit MovbRmIndrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwRmIndrRn final : public Instruction {
 public:
  explicit MovwRmIndrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlRmIndrRn final : public Instruction {
 public:
  explicit MovlRmIndrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovbIndrRmRn final : public Instruction {
 public:
  explicit MovbIndrRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwIndrRmRn final : public Instruction {
 public:
  explicit MovwIndrRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlIndrRmRn final : public Instruction {
 public:
  explicit MovlIndrRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovbRmIndrPredecRn final : public Instruction {
 public:
  explicit MovbRmIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwRmIndrPredecRn final : public Instruction {
 public:
  explicit MovwRmIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlRmIndrPredecRn final : public Instruction {
 public:
  explicit MovlRmIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovbIndrRmPostincRn final : public Instruction {
 public:
  explicit MovbIndrRmPostincRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwIndrRmPostincRn final : public Instruction {
 public:
  explicit MovwIndrRmPostincRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlIndrRmPostincRn final : public Instruction {
 public:
  explicit MovlIndrRmPostincRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovbRmIndrR0Rn final : public Instruction {
 public:
  explicit MovbRmIndrR0Rn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwRmIndrR0Rn final : public Instruction {
 public:
  explicit MovwRmIndrR0Rn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlRmIndrR0Rn final : public Instruction {
 public:
  explicit MovlRmIndrR0Rn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovbIndrR0RmRn final : public Instruction {
 public:
  explicit MovbIndrR0RmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwIndrR0RmRn final : public Instruction {
 public:
  explicit MovwIndrR0RmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlIndrR0RmRn final : public Instruction {
 public:
  explicit MovlIndrR0RmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovImmRn final : public Instruction {
 public:
  explicit MovImmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwIndrDispPcRn final : public Instruction {
 public:
  explicit MovwIndrDispPcRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlIndrDispPcRn final : public Instruction {
 public:
  explicit MovlIndrDispPcRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovbIndrDispGbrR0 final : public Instruction {
 public:
  explicit MovbIndrDispGbrR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwIndrDispGbrR0 final : public Instruction {
 public:
  explicit MovwIndrDispGbrR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlIndrDispGbrR0 final : public Instruction {
 public:
  explicit MovlIndrDispGbrR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovbR0IndrDispGbr final : public Instruction {
 public:
  explicit MovbR0IndrDispGbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwR0IndrDispGbr final : public Instruction {
 public:
  explicit MovwR0IndrDispGbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlR0IndrDispGbr final : public Instruction {
 public:
  explicit MovlR0IndrDispGbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovbR0IndrDispRn final : public Instruction {
 public:
  explicit MovbR0IndrDispRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwR0IndrDispRn final : public Instruction {
 public:
  explicit MovwR0IndrDispRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlRmIndrDispRn final : public Instruction {
 public:
  explicit MovlRmIndrDispRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovbIndrDispRmR0 final : public Instruction {
 public:
  explicit MovbIndrDispRmR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovwIndrDispRmR0 final : public Instruction {
 public:
  explicit MovwIndrDispRmR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovlIndrDispRmRn final : public Instruction {
 public:
  explicit MovlIndrDispRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovaIndrDispPcR0 final : public Instruction {
 public:
  explicit MovaIndrDispPcR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MovtRn final : public Instruction {
 public:
  explicit MovtRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class MullRmRn final : public Instruction {
 public:
  explicit MullRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class MulswRmRn final : public Instruction {
 public:
  explicit MulswRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class MulsRmRn final : public Instruction {
 public:
  explicit MulsRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class MuluwRmRn final : public Instruction {
 public:
  explicit MuluwRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class MuluRmRn final : public Instruction {
 public:
  explicit MuluRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class NegRmRn final : public Instruction {
 public:
  explicit NegRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class NegcRmRn final : public Instruction {
 public:
  explicit NegcRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Nop final : public Instruction {
 public:
  explicit Nop(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class NotRmRn final : public Instruction {
 public:
  explicit NotRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class OrRmRn final : public Instruction {
 public:
  explicit OrRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class OrImmR0 final : public Instruction {
 public:
  explicit OrImmR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class OrbImmIndrR0Gbr final : public Instruction {
 public:
  explicit OrbImmIndrR0Gbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class RotclRn final : public Instruction {
 public:
  explicit RotclRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class RotcrRn final : public Instruction {
 public:
  explicit RotcrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class RotlRn final : public Instruction {
 public:
  explicit RotlRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class RotrRn final : public Instruction {
 public:
  explicit RotrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Rte final : public Instruction {
 public:
  explicit Rte(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  // TODO: Lift
};

class Rts final : public Instruction {
 public:
  explicit Rts(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Info(uint16_t opcode, uint64_t addr,
            BN::InstructionInfo &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class Sett final : public Instruction {
 public:
  explicit Sett(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  bool Lift(uint16_t opcode, uint64_t addr, size_t &len,
            BN::LowLevelILFunction &il, BN::Architecture *arch) override;
};

class ShalRn final : public Instruction {
 public:
  explicit ShalRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class SharRn final : public Instruction {
 public:
  explicit SharRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class ShllRn final : public Instruction {
 public:
  explicit ShllRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Shll2Rn final : public Instruction {
 public:
  explicit Shll2Rn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Shll8Rn final : public Instruction {
 public:
  explicit Shll8Rn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Shll16Rn final : public Instruction {
 public:
  explicit Shll16Rn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class ShlrRn final : public Instruction {
 public:
  explicit ShlrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Shlr2Rn final : public Instruction {
 public:
  explicit Shlr2Rn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Shlr8Rn final : public Instruction {
 public:
  explicit Shlr8Rn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Shlr16Rn final : public Instruction {
 public:
  explicit Shlr16Rn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class Sleep final : public Instruction {
 public:
  explicit Sleep(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StcSrRn final : public Instruction {
 public:
  explicit StcSrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StcGbrRn final : public Instruction {
 public:
  explicit StcGbrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StcVbrRn final : public Instruction {
 public:
  explicit StcVbrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StclSrIndrPredecRn final : public Instruction {
 public:
  explicit StclSrIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StclGbrIndrPredecRn final : public Instruction {
 public:
  explicit StclGbrIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StclVbrIndrPredecRn final : public Instruction {
 public:
  explicit StclVbrIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StsMachRn final : public Instruction {
 public:
  explicit StsMachRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StsMaclRn final : public Instruction {
 public:
  explicit StsMaclRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StsPrRn final : public Instruction {
 public:
  explicit StsPrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StslMachIndrPredecRn final : public Instruction {
 public:
  explicit StslMachIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StslMaclIndrPredecRn final : public Instruction {
 public:
  explicit StslMaclIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class StslPrIndrPredecRn final : public Instruction {
 public:
  explicit StslPrIndrPredecRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class SubRmRn final : public Instruction {
 public:
  explicit SubRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class SubcRmRn final : public Instruction {
 public:
  explicit SubcRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class SubvRmRn final : public Instruction {
 public:
  explicit SubvRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class SwapbRmRn final : public Instruction {
 public:
  explicit SwapbRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class SwapwRmRn final : public Instruction {
 public:
  explicit SwapwRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class TasbIndrRn final : public Instruction {
 public:
  explicit TasbIndrRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class TrapaImm final : public Instruction {
 public:
  explicit TrapaImm(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class TstRmRn final : public Instruction {
 public:
  explicit TstRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class TstImmR0 final : public Instruction {
 public:
  explicit TstImmR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class TstbImmIndrR0Gbr final : public Instruction {
 public:
  explicit TstbImmIndrR0Gbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class XorRmRn final : public Instruction {
 public:
  explicit XorRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class XorImmR0 final : public Instruction {
 public:
  explicit XorImmR0(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class XorbImmIndrR0Gbr final : public Instruction {
 public:
  explicit XorbImmIndrR0Gbr(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};

class XtrctRmRn final : public Instruction {
 public:
  explicit XtrctRmRn(const IsaType &t);

  bool Text(uint16_t opcode, uint64_t addr, size_t &len,
            std::vector<BN::InstructionTextToken> &result) override;

  // TODO: Lift
};
}  // namespace SuperH

#endif  // SRC_INSTRUCTIONS_H_
