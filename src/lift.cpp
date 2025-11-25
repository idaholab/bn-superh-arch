// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include <binaryninjaapi.h>

#include "flags.h"
#include "instructions.h"
#include "opcodes.h"
#include "registers.h"

#define TBIT                                                   \
  il.And(Sizes::LONG, il.Register(Sizes::LONG, Registers::SR), \
         il.Const(Sizes::LONG, 0x1))
#define SETT                                                                 \
  il.SetRegister(Sizes::LONG, Registers::SR,                                 \
                 il.Or(Sizes::LONG, il.Register(Sizes::LONG, Registers::SR), \
                       il.Const(Sizes::LONG, 0x1)))
#define CLRT                                                                  \
  il.SetRegister(Sizes::LONG, Registers::SR,                                  \
                 il.And(Sizes::LONG, il.Register(Sizes::LONG, Registers::SR), \
                        il.Const(Sizes::LONG, 0xFFFFFFF7)))

// LONG operations
#define REG_L(regnum) il.Register(Sizes::LONG, regnum)
#define SETREG_L(regnum, expr) il.SetRegister(Sizes::LONG, regnum, expr)
#define ADD_L(expr1, expr2) il.Add(Sizes::LONG, expr1, expr2)
#define SUB_L(expr1, expr2) il.Sub(Sizes::LONG, expr1, expr2)
#define LOAD_L(addr) il.Load(Sizes::LONG, addr)
#define STORE_L(addr, val) il.Store(Sizes::LONG, addr, val)
#define CONST_L(expr) il.Const(Sizes::LONG, expr)
#define UGT_L(expr1, expr2) \
  il.CompareUnsignedGreaterThan(Sizes::LONG, expr1, expr2)

// WORD operations
#define REG_W(regnum) il.Register(Sizes::WORD, regnum)
#define LOAD_W(addr) il.Load(Sizes::WORD, addr)
#define STORE_W(addr, val) il.Store(Sizes::WORD, addr, val)

// BYTE operations
#define CONST_B(expr) il.Const(Sizes::BYTE, expr)
#define REG_B(regnum) il.Register(Sizes::BYTE, regnum)
#define LOAD_B(addr) il.Load(Sizes::BYTE, addr)
#define STORE_B(addr, val) il.Store(Sizes::BYTE, addr, val)

namespace SuperH {

// Default Lift
bool Instruction::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                       BN::LowLevelILFunction &il, BN::Architecture *arch) {
  il.AddInstruction(il.Unimplemented());
  len = length;
  return true;
}

// stealing this from:
// https://github.com/Vector35/arch-mips/blob/7f38beb482f62e2e32e4da3bce3735f2b7ba63b3/il.cpp#L154
static void ConditionalJump(BN::Architecture *arch, BN::LowLevelILFunction &il,
                            const size_t cond, size_t addrSize,
                            const uint64_t t, const uint64_t f) {
  BNLowLevelILLabel *trueLabel = il.GetLabelForAddress(arch, t);
  BNLowLevelILLabel *falseLabel = il.GetLabelForAddress(arch, f);

  if (trueLabel && falseLabel) {
    il.AddInstruction(il.If(cond, *trueLabel, *falseLabel));
    return;
  }

  BN::LowLevelILLabel trueCode, falseCode;

  if (trueLabel) {
    il.AddInstruction(il.If(cond, *trueLabel, falseCode));
    il.MarkLabel(falseCode);
    il.AddInstruction(il.Jump(il.ConstPointer(addrSize, f)));
    return;
  }

  if (falseLabel) {
    il.AddInstruction(il.If(cond, trueCode, *falseLabel));
    il.MarkLabel(trueCode);
    il.AddInstruction(il.Jump(il.ConstPointer(addrSize, t)));
    return;
  }

  il.AddInstruction(il.If(cond, trueCode, falseCode));
  il.MarkLabel(trueCode);
  il.AddInstruction(il.Jump(il.ConstPointer(addrSize, t)));
  il.MarkLabel(falseCode);
  il.AddInstruction(il.Jump(il.ConstPointer(addrSize, f)));
}

bool AddRmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                   BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [Rn, Rm] = GetNMFormatOpcodeFields(opcode);

  il.AddInstruction(SETREG_L(Rn, ADD_L(REG_L(Rn), REG_L(Rm))));
  return true;
}

bool AddImmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                    BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [Rn, i] = ExtractNIFormatOpcodeFields(opcode);
  const auto imm = static_cast<int8_t>(i);  // convert to signed int

  il.AddInstruction(SETREG_L(Rn, ADD_L(REG_L(Rn), CONST_L(imm))));
  return true;
}

bool AddcRmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                    BN::LowLevelILFunction &il, BN::Architecture *arch) {
  BN::LowLevelILLabel t1, f1, done1, t2, done2;
  const auto [Rn, Rm] = GetNMFormatOpcodeFields(opcode);

  const auto tmp0 = REG_L(Rn);
  const auto tmp1 = ADD_L(REG_L(Rn), REG_L(Rm));

  // Rn = Rn + Rm + T
  il.AddInstruction(
      SETREG_L(Rn, il.Add(Sizes::LONG, tmp1, TBIT, Flags::CARRY)));

  // if (tmp0 > tmp1)
  il.AddInstruction(il.If(UGT_L(tmp0, tmp1), t1, f1));
  // true: T = 1
  il.MarkLabel(t1);
  il.AddInstruction(SETT);
  il.AddInstruction(il.Goto(done1));
  // else: T = 0
  il.MarkLabel(f1);
  il.AddInstruction(CLRT);
  il.AddInstruction(il.Goto(done1));

  // if (tmp1 > Rn)
  il.MarkLabel(done1);
  il.AddInstruction(il.If(UGT_L(tmp1, REG_L(Rn)), t2, done2));
  // true: T = 1
  il.MarkLabel(t2);
  il.AddInstruction(SETT);
  il.AddInstruction(il.Goto(done2));

  // done
  il.MarkLabel(done2);
  return true;
}

// TODO: AddvRmRn::Lift
// TODO: AndRmRn::Lift
// TODO: AndImmR0::Lift
// TODO: AndbImmIndrR0Gbr::Lift

bool BfDisp::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                  BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto target = BfDisp::GetTarget(opcode, addr);
  const auto condition =
      il.CompareEqual(Sizes::LONG, TBIT, il.Const(Sizes::LONG, 0));

  ConditionalJump(arch, il, condition, Sizes::LONG, target, addr + len);
  return true;
}

// TODO: BfsDisp::Lift
// When we implement BfsDisp, we will need to somehow
// get the next instruction's IL
// Ref: https://github.com/Vector35/arch-mips/blob/master/arch_mips.cpp#L457

// TODO: BraDisp::Lift
// TODO: BrafRm::Lift
// TODO: BsrDisp::Lift
// TODO: BsrfRm::Lift

bool BtDisp::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                  BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto target = BtDisp::GetTarget(opcode, addr);
  const auto condition =
      il.CompareEqual(Sizes::LONG, TBIT, il.Const(Sizes::LONG, 1));

  ConditionalJump(arch, il, condition, Sizes::LONG, target, addr + len);
  return true;
}

// TODO: BtsDisp::Lift
// When we implement BtsDisp, we will need to somehow
// get the next instruction's IL
// Ref: https://github.com/Vector35/arch-mips/blob/master/arch_mips.cpp#L457

// TODO: Clrmac::Lift

bool Clrt::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                BN::LowLevelILFunction &il, BN::Architecture *arch) {
  il.AddInstruction(CLRT);
  return true;
}

// TODO: CmpEqRmRn::Lift
// TODO: CmpGeRmRn::Lift
// TODO: CmpGtRmRn::Lift
// TODO: CmpGtRmRn::Lift
// TODO: CmpHiRmRn::Lift
// TODO: CmpHsRmRn::Lift
// TODO: CmpPlRn::Lift
// TODO: CmpPzRn::Lift
// TODO: CmpStrRmRn::Lift
// TODO: CmpEqImmR0::Lift
// TODO: Div0sRmRn::Lift
// TODO: Div0u::LiftLift
// TODO: Div1RmRn::Lift
// TODO: DmulslRmRn::Lift
// TODO: DmululRmRn::Lift
// TODO: DtRn::Lift
// TODO: ExtsbRmRn::Lift
// TODO: ExtswRmRn::Lift
// TODO: ExtubRmRn::Lift
// TODO: ExtuwRmRn::Lift

bool JmpIndrRm::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                     BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto m = GetMFormatOpcodeField(opcode);
  il.AddInstruction(il.Jump(REG_L(m)));
  return true;
}

bool JsrIndrRm::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                     BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto m = GetMFormatOpcodeField(opcode);
  il.AddInstruction(SETREG_L(Registers::PR, REG_L(Registers::PC)));
  il.AddInstruction(il.Call(REG_L(m)));
  return true;
}

// TODO: LdcRmSr::Lift
// TODO: LdcRmGbr::Lift
// TODO: LdcRmVbr::Lift
// TODO: LdclIndrRmPostincSr::Lift
// TODO: LdclIndrRmPostincGbr::Lift
// TODO: LdclIndrRmPostincVbr::Lift
// TODO: LdsRmMach::Lift
// TODO: LdsRmMacl::Lift
// TODO: LdsRmPr::Lift
// TODO: LdslIndrRmPostincMach::Lift
// TODO: LdslIndrRmPostincMacl::Lift
// TODO: LdslIndrRmPostincPr::Lift
// TODO: MaclIndrRmPostincIndrRnPostinc::Lift
// TODO: MacwIndrRmPostincIndrRnPostinc::Lift
// TODO: MacIndrRmPostincIndrRnPostinc::Lift

bool MovRmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                   BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(n, REG_L(m)));
  return true;
}

bool MovbRmIndrRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                        BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(STORE_B(REG_L(n), REG_B(m)));
  return true;
}

bool MovwRmIndrRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                        BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(STORE_W(REG_W(n), REG_W(m)));
  return true;
}

bool MovlRmIndrRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                        BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(STORE_L(REG_L(n), REG_L(m)));
  return true;
}

bool MovbIndrRmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                        BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(n, il.SignExtend(Sizes::LONG, LOAD_B(REG_L(m)))));
  return true;
}

bool MovwIndrRmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                        BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(n, il.SignExtend(Sizes::LONG, LOAD_W(REG_L(m)))));
  return true;
}

bool MovlIndrRmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                        BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(n, LOAD_L(REG_L(m))));
  return true;
}

bool MovbRmIndrPredecRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                              BN::LowLevelILFunction &il,
                              BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(STORE_B(SUB_L(REG_L(n), CONST_L(1)), REG_B(m)));
  il.AddInstruction(SETREG_L(n, SUB_L(REG_L(n), CONST_L(1))));
  return true;
}

bool MovwRmIndrPredecRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                              BN::LowLevelILFunction &il,
                              BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(STORE_W(SUB_L(REG_L(n), CONST_L(1)), REG_W(m)));
  il.AddInstruction(SETREG_L(n, SUB_L(REG_L(n), CONST_L(2))));
  return true;
}

bool MovlRmIndrPredecRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                              BN::LowLevelILFunction &il,
                              BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(STORE_L(SUB_L(REG_L(n), CONST_L(4)), REG_L(m)));
  il.AddInstruction(SETREG_L(n, SUB_L(REG_L(n), CONST_L(4))));
  return true;
}

bool MovbIndrRmPostincRn::Lift(const uint16_t opcode, uint64_t addr,
                               size_t &len, BN::LowLevelILFunction &il,
                               BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(n, il.SignExtend(Sizes::LONG, LOAD_B(REG_L(m)))));
  if (n != m) {
    il.AddInstruction(SETREG_L(m, ADD_L(REG_L(m), CONST_L(1))));
  }
  return true;
}

bool MovwIndrRmPostincRn::Lift(const uint16_t opcode, uint64_t addr,
                               size_t &len, BN::LowLevelILFunction &il,
                               BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(n, il.SignExtend(Sizes::LONG, LOAD_W(REG_L(m)))));
  if (n != m) {
    il.AddInstruction(SETREG_L(m, ADD_L(REG_L(m), CONST_L(2))));
  }
  return true;
}

bool MovlIndrRmPostincRn::Lift(const uint16_t opcode, uint64_t addr,
                               size_t &len, BN::LowLevelILFunction &il,
                               BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(n, LOAD_L(REG_L(m))));
  if (n != m) {
    il.AddInstruction(SETREG_L(m, ADD_L(REG_L(m), CONST_L(4))));
  }
  return true;
}

bool MovbRmIndrR0Rn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                          BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(STORE_B(ADD_L(REG_L(n), REG_L(Registers::R0)), REG_B(m)));
  return true;
}

bool MovwRmIndrR0Rn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                          BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(STORE_W(ADD_L(REG_L(n), REG_L(Registers::R0)), REG_W(m)));
  return true;
}

bool MovlRmIndrR0Rn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                          BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(STORE_L(ADD_L(REG_L(n), REG_L(Registers::R0)), REG_L(m)));
  return true;
}

bool MovbIndrR0RmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                          BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(
      n, il.SignExtend(Sizes::LONG,
                       LOAD_B(ADD_L(REG_L(m), REG_L(Registers::R0))))));
  return true;
}

bool MovwIndrR0RmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                          BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(
      n, il.SignExtend(Sizes::LONG,
                       LOAD_W(ADD_L(REG_L(m), REG_L(Registers::R0))))));
  return true;
}

bool MovlIndrR0RmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                          BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, m] = GetNMFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(n, LOAD_L(ADD_L(REG_L(m), REG_L(Registers::R0)))));
  return true;
}

bool MovImmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                    BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto [n, i] = ExtractNIFormatOpcodeFields(opcode);
  il.AddInstruction(SETREG_L(n, il.SignExtend(Sizes::LONG, CONST_B(i))));
  return true;
}

bool MovwIndrDispPcRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                            BN::LowLevelILFunction &il,
                            BN::Architecture *arch) {
  const auto [n, d] = ExtractND8FormatOpcodeFields(opcode);
  const auto target =
      (static_cast<uint64_t>(d) * 2) + addr + (2 * INSTRUCTION_SIZE);

  il.AddInstruction(SETREG_L(
      n, il.SignExtend(Sizes::LONG, LOAD_W(il.Const(Sizes::LONG, target)))));
  return true;
}

bool MovlIndrDispPcRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                            BN::LowLevelILFunction &il,
                            BN::Architecture *arch) {
  const auto [n, d] = ExtractND8FormatOpcodeFields(opcode);
  const auto target = (static_cast<uint64_t>(d) * 4) + (addr & 0xFFFFFFFC) +
                      (2 * INSTRUCTION_SIZE);

  il.AddInstruction(SETREG_L(n, LOAD_L(il.Const(Sizes::LONG, target))));
  return true;
}

bool MovbIndrDispGbrR0::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                             BN::LowLevelILFunction &il,
                             BN::Architecture *arch) {
  const auto d = ExtractDFormatOpcodeFields(opcode);

  il.AddInstruction(SETREG_L(
      Registers::R0,
      il.SignExtend(Sizes::LONG,
                    LOAD_B(ADD_L(CONST_L(d), REG_L(Registers::GBR))))));
  return true;
}

bool MovwIndrDispGbrR0::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                             BN::LowLevelILFunction &il,
                             BN::Architecture *arch) {
  const auto d = ExtractDFormatOpcodeFields(opcode);
  const auto target = static_cast<uint16_t>(d) * 2;

  il.AddInstruction(SETREG_L(
      Registers::R0,
      il.SignExtend(Sizes::LONG,
                    LOAD_W(ADD_L(CONST_L(target), REG_L(Registers::GBR))))));
  return true;
}

bool MovlIndrDispGbrR0::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                             BN::LowLevelILFunction &il,
                             BN::Architecture *arch) {
  const auto d = ExtractDFormatOpcodeFields(opcode);
  const auto target = static_cast<uint32_t>(d) * 4;

  il.AddInstruction(SETREG_L(
      Registers::R0, LOAD_L(ADD_L(REG_L(Registers::GBR), CONST_L(target)))));
  return true;
}

bool MovbR0IndrDispGbr::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                             BN::LowLevelILFunction &il,
                             BN::Architecture *arch) {
  const auto d = ExtractDFormatOpcodeFields(opcode);

  il.AddInstruction(
      STORE_B(ADD_L(REG_L(Registers::GBR), CONST_L(d)), REG_B(Registers::R0)));
  return true;
}

bool MovwR0IndrDispGbr::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                             BN::LowLevelILFunction &il,
                             BN::Architecture *arch) {
  const auto d = ExtractDFormatOpcodeFields(opcode);
  const auto target = static_cast<uint16_t>(d) * 2;

  il.AddInstruction(STORE_W(ADD_L(REG_L(Registers::GBR), CONST_L(target)),
                            REG_W(Registers::R0)));
  return true;
}

bool MovlR0IndrDispGbr::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                             BN::LowLevelILFunction &il,
                             BN::Architecture *arch) {
  const auto d = ExtractDFormatOpcodeFields(opcode);
  const auto target = static_cast<uint32_t>(d) * 4;

  il.AddInstruction(STORE_L(ADD_L(REG_L(Registers::GBR), CONST_L(target)),
                            REG_L(Registers::R0)));
  return true;
}

bool MovbR0IndrDispRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                            BN::LowLevelILFunction &il,
                            BN::Architecture *arch) {
  const auto [n, d] = ExtractND4FormatOpcodeFields(opcode);

  il.AddInstruction(STORE_B(ADD_L(REG_L(n), CONST_L(d)), REG_B(Registers::R0)));
  return true;
}

bool MovwR0IndrDispRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                            BN::LowLevelILFunction &il,
                            BN::Architecture *arch) {
  auto [n, d] = ExtractND4FormatOpcodeFields(opcode);
  d *= 2;

  il.AddInstruction(STORE_W(ADD_L(REG_L(n), CONST_L(d)), REG_W(Registers::R0)));
  return true;
}

bool MovlRmIndrDispRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                            BN::LowLevelILFunction &il,
                            BN::Architecture *arch) {
  const auto [n, m, d] = ExtractNMDFormatOpcodeFields(opcode);
  const auto target = (static_cast<uint32_t>(d) & 0xF) * 4;

  il.AddInstruction(STORE_L(ADD_L(REG_L(n), CONST_L(target)), REG_L(m)));
  return true;
}

bool MovbIndrDispRmR0::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                            BN::LowLevelILFunction &il,
                            BN::Architecture *arch) {
  const auto [m, d] = ExtractMDFormatOpcodeFields(opcode);

  il.AddInstruction(SETREG_L(
      Registers::R0,
      il.SignExtend(Sizes::LONG, LOAD_B(ADD_L(CONST_L(d), REG_L(m))))));
  return true;
}

bool MovwIndrDispRmR0::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                            BN::LowLevelILFunction &il,
                            BN::Architecture *arch) {
  auto [m, d] = ExtractMDFormatOpcodeFields(opcode);
  d *= 2;

  il.AddInstruction(SETREG_L(
      Registers::R0,
      il.SignExtend(Sizes::LONG, LOAD_W(ADD_L(CONST_L(d), REG_L(m))))));
  return true;
}

bool MovlIndrDispRmRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                            BN::LowLevelILFunction &il,
                            BN::Architecture *arch) {
  const auto [n, m, d] = ExtractNMDFormatOpcodeFields(opcode);
  const auto target = (static_cast<uint32_t>(d) & 0xF) * 4;

  il.AddInstruction(SETREG_L(n, LOAD_L(ADD_L(REG_L(m), CONST_L(target)))));
  return true;
}

bool MovaIndrDispPcR0::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                            BN::LowLevelILFunction &il,
                            BN::Architecture *arch) {
  const auto n = GetNFormatOpcodeField(opcode);
  const auto d = ExtractDFormatOpcodeFields(opcode);

  const auto target = (static_cast<uint64_t>(d) * 4) + (addr & 0xFFFFFFFC) +
                      (2 * INSTRUCTION_SIZE);

  il.AddInstruction(SETREG_L(Registers::R0, CONST_L(target)));
  return true;
}

bool MovtRn::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                  BN::LowLevelILFunction &il, BN::Architecture *arch) {
  const auto n = GetNFormatOpcodeField(opcode);

  il.AddInstruction(SETREG_L(n, TBIT));
  return true;
}

// TODO: MullRmRn::Lift
// TODO: MulswRmRn::Lift
// TODO: MulsRmRn::Lift
// TODO: MuluwRmRn::Lift
// TODO: MuluRmRn::Lift
// TODO: NegRmRn::Lift
// TODO: NegcRmRn::Lift

bool Nop::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
               BN::LowLevelILFunction &il, BN::Architecture *arch) {
  il.AddInstruction(il.Nop());
  return true;
}

// TODO: NotRmRn::Lift
// TODO: OrRmRn::Lift
// TODO: OrImmR0::
// TODO: OrbImmIndrR0Gbr::Lift
// TODO: RotclRn::Lift
// TODO: RotcrRn::Lift
// TODO: RotlRn::Lift
// TODO: RotrRn::Lift
// TODO: Rte::Lift

bool Rts::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
               BN::LowLevelILFunction &il, BN::Architecture *arch) {
  il.AddInstruction(il.Return(REG_L(Registers::PR)));
  return true;
}

bool Sett::Lift(const uint16_t opcode, uint64_t addr, size_t &len,
                BN::LowLevelILFunction &il, BN::Architecture *arch) {
  il.AddInstruction(SETT);
  return true;
}

// TODO: ShalRn::Lift
// TODO: SharRn::Lift
// TODO: ShllRn::Lift
// TODO: Shll2Rn::Lift
// TODO: Shll8Rn::Lift
// TODO: Shll16Rn::Lift
// TODO: ShlrRn::Lift
// TODO: Shlr2Rn::Lift
// TODO: Shlr8Rn::Lift
// TODO: Shlr16Rn::Lift
// TODO: Sleep::LiftLift
// TODO: StcSrRn::Lift
// TODO: StcGbrRn::Lift
// TODO: StcVbrRn::Lift
// TODO: StclSrIndrPredecRn::Lift
// TODO: StclGbrIndrPredecRn::Lift
// TODO: StclVbrIndrPredecRn::Lift
// TODO: StsMachRn::Lift
// TODO: StsMaclRn::Lift
// TODO: StsPrRn::Lift
// TODO: StslMachIndrPredecRn::Lift
// TODO: StslMaclIndrPredecRn::Lift
// TODO: StslPrIndrPredecRn::Lift
// TODO: SubRmRn::Lift
// TODO: SubcRmRn::Lift
// TODO: SubvRmRn::Lift
// TODO: SwapbRmRn::Lift
// TODO: SwapwRmRn::Lift
// TODO: TasbIndrRn::Lift
// TODO: TrapaImm::Lift
// TODO: TstRmRn::Lift
// TODO: TstImmR0::Lift
// TODO: TstbImmIndrR0Gbr::Lift
// TODO: XorRmRn::Lift
// TODO: XorImmR0::Lift
// TODO: XorbImmIndrR0Gbr::Lift
// TODO: XtrctRmRn::Lift

/*
 * Floating Point Instructions
 * SH-2E Only
 */

// TODO: FabsFrn::Lift
// TODO: FaddFrmFrn::Lift
// TODO: FcmpEqFrmFrn::Lift
// TODO: FcmpGtFrmFrn::Lift
// TODO: FdivFrmFrn::Lift
// TODO: Fldi0Frn::Lift
// TODO: Fldi1Frn::Lift
// TODO: FldsFrmFpul::Lift
// TODO: FloatFpulFrn::Lift
// TODO: FmacFr0FrmFrn::Lift
// TODO: FmovFrmFrn::Lift
// TODO: FmovsIndrRmFrn::Lift
// TODO: FmovsFrmIndrRn::Lift
// TODO: FmovsIndrRmPostincFrn::Lift
// TODO: FmovsFrmIndrPredecRn::Lift
// TODO: FmovsIndrR0RmFrn::Lift
// TODO: FmovsFrmIndrR0Rn::Lift
// TODO: FmulFrmFrn::Lift
// TODO: FnegFrn::Lift
// TODO: FstsFpulFrn::Lift
// TODO: FsubFrmFrn::Lift
// TODO: FtrcFrmFpul::Lift
// TODO: LdsRmFpul::Lift
// TODO: LdslIndrRmPostincFpul::Lift
// TODO: LdsRmFpscr::Lift
// TODO: LdslIndrRmPostincFpscr::Lift
// TODO: StsFpulRn::Lift
// TODO: StslFpulIndrPredecRn::Lift
// TODO: StsFpscrRn::Lift
// TODO: StslFpscrIndrPredecRn::Lift
}  // namespace SuperH
