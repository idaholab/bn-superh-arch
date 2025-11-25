// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "instructions.h"

#include "architecture.h"

namespace SuperH {
/*
 * Abstract instruction class methods that need to be inherited/used by
 * subclasses
 */
Instruction::Instruction(const IsaType &t) { this->isa_type = t; }

IsaType Instruction::GetIsaType() const { return this->isa_type; }

/*
 * Concrete instruction class constructors
 */
AddRmRn::AddRmRn(const IsaType &t) : Instruction(t) {}

AddImmRn::AddImmRn(const IsaType &t) : Instruction(t) {}

AddcRmRn::AddcRmRn(const IsaType &t) : Instruction(t) {}

AddvRmRn::AddvRmRn(const IsaType &t) : Instruction(t) {}

AndRmRn::AndRmRn(const IsaType &t) : Instruction(t) {}

AndImmR0::AndImmR0(const IsaType &t) : Instruction(t) {}

AndbImmIndrR0Gbr::AndbImmIndrR0Gbr(const IsaType &t) : Instruction(t) {}

BfDisp::BfDisp(const IsaType &t) : Instruction(t) {}

BfsDisp::BfsDisp(const IsaType &t) : Instruction(t) {}

BraDisp::BraDisp(const IsaType &t) : Instruction(t) {}

BrafRm::BrafRm(const IsaType &t) : Instruction(t) {}

BsrDisp::BsrDisp(const IsaType &t) : Instruction(t) {}

BsrfRm::BsrfRm(const IsaType &t) : Instruction(t) {}

BtDisp::BtDisp(const IsaType &t) : Instruction(t) {}

BtsDisp::BtsDisp(const IsaType &t) : Instruction(t) {}

Clrmac::Clrmac(const IsaType &t) : Instruction(t) {}

Clrt::Clrt(const IsaType &t) : Instruction(t) {}

CmpEqRmRn::CmpEqRmRn(const IsaType &t) : Instruction(t) {}

CmpGeRmRn::CmpGeRmRn(const IsaType &t) : Instruction(t) {}

CmpGtRmRn::CmpGtRmRn(const IsaType &t) : Instruction(t) {}

CmpHiRmRn::CmpHiRmRn(const IsaType &t) : Instruction(t) {}

CmpHsRmRn::CmpHsRmRn(const IsaType &t) : Instruction(t) {}

CmpPlRn::CmpPlRn(const IsaType &t) : Instruction(t) {}

CmpPzRn::CmpPzRn(const IsaType &t) : Instruction(t) {}

CmpStrRmRn::CmpStrRmRn(const IsaType &t) : Instruction(t) {}

CmpEqImmR0::CmpEqImmR0(const IsaType &t) : Instruction(t) {}

Div0sRmRn::Div0sRmRn(const IsaType &t) : Instruction(t) {}

Div0u::Div0u(const IsaType &t) : Instruction(t) {}

Div1RmRn::Div1RmRn(const IsaType &t) : Instruction(t) {}

DmulslRmRn::DmulslRmRn(const IsaType &t) : Instruction(t) {}

DmululRmRn::DmululRmRn(const IsaType &t) : Instruction(t) {}

DtRn::DtRn(const IsaType &t) : Instruction(t) {}

ExtsbRmRn::ExtsbRmRn(const IsaType &t) : Instruction(t) {}

ExtswRmRn::ExtswRmRn(const IsaType &t) : Instruction(t) {}

ExtubRmRn::ExtubRmRn(const IsaType &t) : Instruction(t) {}

ExtuwRmRn::ExtuwRmRn(const IsaType &t) : Instruction(t) {}

FabsFrn::FabsFrn(const IsaType &t) : Instruction(t) {}

FaddFrmFrn::FaddFrmFrn(const IsaType &t) : Instruction(t) {}

FcmpEqFrmFrn::FcmpEqFrmFrn(const IsaType &t) : Instruction(t) {}

FcmpGtFrmFrn::FcmpGtFrmFrn(const IsaType &t) : Instruction(t) {}

FdivFrmFrn::FdivFrmFrn(const IsaType &t) : Instruction(t) {}

Fldi0Frn::Fldi0Frn(const IsaType &t) : Instruction(t) {}

Fldi1Frn::Fldi1Frn(const IsaType &t) : Instruction(t) {}

FldsFrmFpul::FldsFrmFpul(const IsaType &t) : Instruction(t) {}

FloatFpulFrn::FloatFpulFrn(const IsaType &t) : Instruction(t) {}

FmacFr0FrmFrn::FmacFr0FrmFrn(const IsaType &t) : Instruction(t) {}

FmovFrmFrn::FmovFrmFrn(const IsaType &t) : Instruction(t) {}

FmovsIndrR0RmFrn::FmovsIndrR0RmFrn(const IsaType &t) : Instruction(t) {}

FmovsIndrRmPostincFrn::FmovsIndrRmPostincFrn(const IsaType &t)
    : Instruction(t) {}

FmovsIndrRmFrn::FmovsIndrRmFrn(const IsaType &t) : Instruction(t) {}

FmovsFrmIndrR0Rn::FmovsFrmIndrR0Rn(const IsaType &t) : Instruction(t) {}

FmovsFrmIndrPredecRn::FmovsFrmIndrPredecRn(const IsaType &t) : Instruction(t) {}

FmovsFrmIndrRn::FmovsFrmIndrRn(const IsaType &t) : Instruction(t) {}

FmulFrmFrn::FmulFrmFrn(const IsaType &t) : Instruction(t) {}

FnegFrn::FnegFrn(const IsaType &t) : Instruction(t) {}

FstsFpulFrn::FstsFpulFrn(const IsaType &t) : Instruction(t) {}

FsubFrmFrn::FsubFrmFrn(const IsaType &t) : Instruction(t) {}

FtrcFrmFpul::FtrcFrmFpul(const IsaType &t) : Instruction(t) {}

LdsRmFpscr::LdsRmFpscr(const IsaType &t) : Instruction(t) {}

LdsRmFpul::LdsRmFpul(const IsaType &t) : Instruction(t) {}

LdslIndrRmPostincFpscr::LdslIndrRmPostincFpscr(const IsaType &t)
    : Instruction(t) {}

LdslIndrRmPostincFpul::LdslIndrRmPostincFpul(const IsaType &t)
    : Instruction(t) {}

StsFpscrRn::StsFpscrRn(const IsaType &t) : Instruction(t) {}

StsFpulRn::StsFpulRn(const IsaType &t) : Instruction(t) {}

StslFpscrIndrPredecRn::StslFpscrIndrPredecRn(const IsaType &t)
    : Instruction(t) {}

StslFpulIndrPredecRn::StslFpulIndrPredecRn(const IsaType &t) : Instruction(t) {}

JmpIndrRm::JmpIndrRm(const IsaType &t) : Instruction(t) {}

JsrIndrRm::JsrIndrRm(const IsaType &t) : Instruction(t) {}

LdcRmSr::LdcRmSr(const IsaType &t) : Instruction(t) {}

LdcRmGbr::LdcRmGbr(const IsaType &t) : Instruction(t) {}

LdcRmVbr::LdcRmVbr(const IsaType &t) : Instruction(t) {}

LdclIndrRmPostincSr::LdclIndrRmPostincSr(const IsaType &t) : Instruction(t) {}

LdclIndrRmPostincGbr::LdclIndrRmPostincGbr(const IsaType &t) : Instruction(t) {}

LdclIndrRmPostincVbr::LdclIndrRmPostincVbr(const IsaType &t) : Instruction(t) {}

LdsRmMach::LdsRmMach(const IsaType &t) : Instruction(t) {}

LdsRmMacl::LdsRmMacl(const IsaType &t) : Instruction(t) {}

LdsRmPr::LdsRmPr(const IsaType &t) : Instruction(t) {}

LdslIndrRmPostincMach::LdslIndrRmPostincMach(const IsaType &t)
    : Instruction(t) {}

LdslIndrRmPostincMacl::LdslIndrRmPostincMacl(const IsaType &t)
    : Instruction(t) {}

LdslIndrRmPostincPr::LdslIndrRmPostincPr(const IsaType &t) : Instruction(t) {}

MaclIndrRmPostincIndrRnPostinc::MaclIndrRmPostincIndrRnPostinc(const IsaType &t)
    : Instruction(t) {}

MacwIndrRmPostincIndrRnPostinc::MacwIndrRmPostincIndrRnPostinc(const IsaType &t)
    : Instruction(t) {}

MacIndrRmPostincIndrRnPostinc::MacIndrRmPostincIndrRnPostinc(const IsaType &t)
    : Instruction(t) {}

MovRmRn::MovRmRn(const IsaType &t) : Instruction(t) {}

MovbRmIndrRn::MovbRmIndrRn(const IsaType &t) : Instruction(t) {}

MovwRmIndrRn::MovwRmIndrRn(const IsaType &t) : Instruction(t) {}

MovlRmIndrRn::MovlRmIndrRn(const IsaType &t) : Instruction(t) {}

MovbIndrRmRn::MovbIndrRmRn(const IsaType &t) : Instruction(t) {}

MovwIndrRmRn::MovwIndrRmRn(const IsaType &t) : Instruction(t) {}

MovlIndrRmRn::MovlIndrRmRn(const IsaType &t) : Instruction(t) {}

MovbRmIndrPredecRn::MovbRmIndrPredecRn(const IsaType &t) : Instruction(t) {}

MovwRmIndrPredecRn::MovwRmIndrPredecRn(const IsaType &t) : Instruction(t) {}

MovlRmIndrPredecRn::MovlRmIndrPredecRn(const IsaType &t) : Instruction(t) {}

MovbIndrRmPostincRn::MovbIndrRmPostincRn(const IsaType &t) : Instruction(t) {}

MovwIndrRmPostincRn::MovwIndrRmPostincRn(const IsaType &t) : Instruction(t) {}

MovlIndrRmPostincRn::MovlIndrRmPostincRn(const IsaType &t) : Instruction(t) {}

MovbRmIndrR0Rn::MovbRmIndrR0Rn(const IsaType &t) : Instruction(t) {}

MovwRmIndrR0Rn::MovwRmIndrR0Rn(const IsaType &t) : Instruction(t) {}

MovlRmIndrR0Rn::MovlRmIndrR0Rn(const IsaType &t) : Instruction(t) {}

MovbIndrR0RmRn::MovbIndrR0RmRn(const IsaType &t) : Instruction(t) {}

MovwIndrR0RmRn::MovwIndrR0RmRn(const IsaType &t) : Instruction(t) {}

MovlIndrR0RmRn::MovlIndrR0RmRn(const IsaType &t) : Instruction(t) {}

MovImmRn::MovImmRn(const IsaType &t) : Instruction(t) {}

MovwIndrDispPcRn::MovwIndrDispPcRn(const IsaType &t) : Instruction(t) {}

MovlIndrDispPcRn::MovlIndrDispPcRn(const IsaType &t) : Instruction(t) {}

MovbIndrDispGbrR0::MovbIndrDispGbrR0(const IsaType &t) : Instruction(t) {}

MovwIndrDispGbrR0::MovwIndrDispGbrR0(const IsaType &t) : Instruction(t) {}

MovlIndrDispGbrR0::MovlIndrDispGbrR0(const IsaType &t) : Instruction(t) {}

MovbR0IndrDispGbr::MovbR0IndrDispGbr(const IsaType &t) : Instruction(t) {}

MovwR0IndrDispGbr::MovwR0IndrDispGbr(const IsaType &t) : Instruction(t) {}

MovlR0IndrDispGbr::MovlR0IndrDispGbr(const IsaType &t) : Instruction(t) {}

MovbR0IndrDispRn::MovbR0IndrDispRn(const IsaType &t) : Instruction(t) {}

MovwR0IndrDispRn::MovwR0IndrDispRn(const IsaType &t) : Instruction(t) {}

MovlRmIndrDispRn::MovlRmIndrDispRn(const IsaType &t) : Instruction(t) {}

MovbIndrDispRmR0::MovbIndrDispRmR0(const IsaType &t) : Instruction(t) {}

MovwIndrDispRmR0::MovwIndrDispRmR0(const IsaType &t) : Instruction(t) {}

MovlIndrDispRmRn::MovlIndrDispRmRn(const IsaType &t) : Instruction(t) {}

MovaIndrDispPcR0::MovaIndrDispPcR0(const IsaType &t) : Instruction(t) {}

MovtRn::MovtRn(const IsaType &t) : Instruction(t) {}

MullRmRn::MullRmRn(const IsaType &t) : Instruction(t) {}

MulswRmRn::MulswRmRn(const IsaType &t) : Instruction(t) {}

MulsRmRn::MulsRmRn(const IsaType &t) : Instruction(t) {}

MuluwRmRn::MuluwRmRn(const IsaType &t) : Instruction(t) {}

MuluRmRn::MuluRmRn(const IsaType &t) : Instruction(t) {}

NegRmRn::NegRmRn(const IsaType &t) : Instruction(t) {}

NegcRmRn::NegcRmRn(const IsaType &t) : Instruction(t) {}

Nop::Nop(const IsaType &t) : Instruction(t) {}

NotRmRn::NotRmRn(const IsaType &t) : Instruction(t) {}

OrRmRn::OrRmRn(const IsaType &t) : Instruction(t) {}

OrImmR0::OrImmR0(const IsaType &t) : Instruction(t) {}

OrbImmIndrR0Gbr::OrbImmIndrR0Gbr(const IsaType &t) : Instruction(t) {}

RotclRn::RotclRn(const IsaType &t) : Instruction(t) {}

RotcrRn::RotcrRn(const IsaType &t) : Instruction(t) {}

RotlRn::RotlRn(const IsaType &t) : Instruction(t) {}

RotrRn::RotrRn(const IsaType &t) : Instruction(t) {}

Rte::Rte(const IsaType &t) : Instruction(t) {}

Rts::Rts(const IsaType &t) : Instruction(t) {}

Sett::Sett(const IsaType &t) : Instruction(t) {}

ShalRn::ShalRn(const IsaType &t) : Instruction(t) {}

SharRn::SharRn(const IsaType &t) : Instruction(t) {}

ShllRn::ShllRn(const IsaType &t) : Instruction(t) {}

Shll2Rn::Shll2Rn(const IsaType &t) : Instruction(t) {}

Shll8Rn::Shll8Rn(const IsaType &t) : Instruction(t) {}

Shll16Rn::Shll16Rn(const IsaType &t) : Instruction(t) {}

ShlrRn::ShlrRn(const IsaType &t) : Instruction(t) {}

Shlr2Rn::Shlr2Rn(const IsaType &t) : Instruction(t) {}

Shlr8Rn::Shlr8Rn(const IsaType &t) : Instruction(t) {}

Shlr16Rn::Shlr16Rn(const IsaType &t) : Instruction(t) {}

Sleep::Sleep(const IsaType &t) : Instruction(t) {}

StcSrRn::StcSrRn(const IsaType &t) : Instruction(t) {}

StcGbrRn::StcGbrRn(const IsaType &t) : Instruction(t) {}

StcVbrRn::StcVbrRn(const IsaType &t) : Instruction(t) {}

StclSrIndrPredecRn::StclSrIndrPredecRn(const IsaType &t) : Instruction(t) {}

StclGbrIndrPredecRn::StclGbrIndrPredecRn(const IsaType &t) : Instruction(t) {}

StclVbrIndrPredecRn::StclVbrIndrPredecRn(const IsaType &t) : Instruction(t) {}

StsMachRn::StsMachRn(const IsaType &t) : Instruction(t) {}

StsMaclRn::StsMaclRn(const IsaType &t) : Instruction(t) {}

StsPrRn::StsPrRn(const IsaType &t) : Instruction(t) {}

StslMachIndrPredecRn::StslMachIndrPredecRn(const IsaType &t) : Instruction(t) {}

StslMaclIndrPredecRn::StslMaclIndrPredecRn(const IsaType &t) : Instruction(t) {}

StslPrIndrPredecRn::StslPrIndrPredecRn(const IsaType &t) : Instruction(t) {}

SubRmRn::SubRmRn(const IsaType &t) : Instruction(t) {}

SubcRmRn::SubcRmRn(const IsaType &t) : Instruction(t) {}

SubvRmRn::SubvRmRn(const IsaType &t) : Instruction(t) {}

SwapbRmRn::SwapbRmRn(const IsaType &t) : Instruction(t) {}

SwapwRmRn::SwapwRmRn(const IsaType &t) : Instruction(t) {}

TasbIndrRn::TasbIndrRn(const IsaType &t) : Instruction(t) {}

TrapaImm::TrapaImm(const IsaType &t) : Instruction(t) {}

TstRmRn::TstRmRn(const IsaType &t) : Instruction(t) {}

TstImmR0::TstImmR0(const IsaType &t) : Instruction(t) {}

TstbImmIndrR0Gbr::TstbImmIndrR0Gbr(const IsaType &t) : Instruction(t) {}

XorRmRn::XorRmRn(const IsaType &t) : Instruction(t) {}

XorImmR0::XorImmR0(const IsaType &t) : Instruction(t) {}

XorbImmIndrR0Gbr::XorbImmIndrR0Gbr(const IsaType &t) : Instruction(t) {}

XtrctRmRn::XtrctRmRn(const IsaType &t) : Instruction(t) {}

/*
 * Instruction decoder method that will parse opcodes and return a
 * pointer to an instruction object
 */
std::optional<std::unique_ptr<Instruction>> DecodeInstruction(
    const IsaType &t, const uint16_t opcode) {
  switch ((opcode >> 12) & 0xF) {
    // First 4 bits are always part of the opcode
    case 0b0000:
      return ParsePrefix0000(t, opcode);
    case 0b0001:
      // MOV.L Rm,@(disp,Rn)  0001nnnnmmmmdddd
      return std::make_unique<MovlRmIndrDispRn>(t);
    case 0b0010:
      return ParsePrefix0010(t, opcode);
    case 0b0011:
      return ParsePrefix0011(t, opcode);
    case 0b0100:
      return ParsePrefix0100(t, opcode);
    case 0b0101:
      // MOV.L @(disp,Rm),Rn  0101nnnnmmmmdddd
      return std::make_unique<MovlIndrDispRmRn>(t);
    case 0b0110:
      return ParsePrefix0110(t, opcode);
    case 0b0111:
      // ADD #imm,Rn          0111nnnniiiiiiii
      return std::make_unique<AddImmRn>(t);
    case 0b1000:
      return ParsePrefix1000(t, opcode);
    case 0b1001:
      // MOV.W @(disp,PC),Rn  1001nnnndddddddd
      return std::make_unique<MovwIndrDispPcRn>(t);
    case 0b1010:
      // BRA label            1010dddddddddddd
      return std::make_unique<BraDisp>(t);
    case 0b1011:
      // BSR label            1011dddddddddddd
      return std::make_unique<BsrDisp>(t);
    case 0b1100:
      return ParsePrefix1100(t, opcode);
    case 0b1101:
      // MOV.L @(disp,PC),Rn  1101nnnndddddddd
      return std::make_unique<MovlIndrDispPcRn>(t);
    case 0b1110:
      // MOV #imm,Rn          1110nnnniiiiiiii
      return std::make_unique<MovImmRn>(t);
    case 0b1111:
      return ParsePrefix1111(t, opcode);
    default:
      return std::nullopt;
  }
}

std::optional<std::unique_ptr<Instruction>> ParsePrefix0000(
    const IsaType &t, const uint16_t opcode) {
  switch (opcode & 0xF) {
    // Parse 4-bit suffix for opcode with prefix 0000
    case 0b0010:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0000 and suffix 0010
        case 0b0000:
          // STC SR,Rn            0000nnnn00000010
          return std::make_unique<StcSrRn>(t);
        case 0b0001:
          // STC GBR,Rn           0000nnnn00010010
          return std::make_unique<StcGbrRn>(t);
        case 0b0010:
          // STC VBR,Rn           0000nnnn00100010
          return std::make_unique<StcVbrRn>(t);
        default:
          return std::nullopt;
      }
    case 0b0011:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0000 and suffix 0011
        case 0b0000:
          // BSRF Rm              0000mmmm00000011
          return std::make_unique<BsrfRm>(t);
        case 0b0010:
          // BRAF Rm              0000mmmm00100011
          return std::make_unique<BrafRm>(t);
        default:
          return std::nullopt;
      }
    case 0b0100:
      // MOV.B Rm,@(R0,Rn)    0000nnnnmmmm0100
      return std::make_unique<MovbRmIndrR0Rn>(t);
    case 0b0101:
      // MOV.W Rm,@(R0,Rn)    0000nnnnmmmm0101
      return std::make_unique<MovwRmIndrR0Rn>(t);
    case 0b0110:
      // MOV.L Rm,@(R0,Rn)    0000nnnnmmmm0110
      return std::make_unique<MovlRmIndrR0Rn>(t);
    case 0b0111:
      // MUL.L Rm,Rn          0000nnnnmmmm0111
      return std::make_unique<MullRmRn>(t);
    case 0b1000:
      switch (opcode) {
        // Match based on entire opcode, there are no operands
        case 0b0000000000001000:
          // CLRT                 0000000000001000
          return std::make_unique<Clrt>(t);
        case 0b0000000000011000:
          // SETT                 0000000000011000
          return std::make_unique<Sett>(t);
        case 0b0000000000101000:
          // CLRMAC               0000000000101000
          return std::make_unique<Clrmac>(t);
        default:
          return std::nullopt;
      }
    case 0b1001:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0000 and suffix 1001
        case 0b0000: {
          if (opcode == 0b0000000000001001) {
            // Make sure the full opcode matches
            // NOP                  0000000000001001
            return std::make_unique<Nop>(t);
          }
          break;
        }
        case 0b0001: {
          if (opcode == 0b0000000000011001) {
            // Make sure the full opcode matches
            // DIV0U                0000000000011001
            return std::make_unique<Div0u>(t);
          }
          break;
        }
        case 0b0010:
          // MOVT Rn              0000nnnn00101001
          return std::make_unique<MovtRn>(t);
        default:
          return std::nullopt;
      }
    case 0b1010:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0000 and suffix 1010
        case 0b0000:
          // STS MACH,Rn          0000nnnn00001010
          return std::make_unique<StsMachRn>(t);
        case 0b0001:
          // STS MACL,Rn          0000nnnn00011010
          return std::make_unique<StsMaclRn>(t);
        case 0b0010:
          // STS PR,Rn            0000nnnn00101010
          return std::make_unique<StsPrRn>(t);

        case 0b0101:
          // STS FPUL,Rn          0000nnnn01011010
          return std::make_unique<StsFpulRn>(t);
        case 0b0110:
          // STS DSR,Rn           0000nnnn01101010 // NOTE: DSP and 2E conflict
          //                        return StsDsrRn(t);                    case
          //                        0b0110:
          // STS FPSCR,Rn         0000nnnn01101010
          return std::make_unique<StsFpscrRn>(t);
        default:
          return std::nullopt;
      }
    case 0b1011:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0000 and suffix 1011
        case 0b0000: {
          if (opcode == 0b0000000000001011) {
            // Make sure the full opcode matches
            // RTS                  0000000000001011
            return std::make_unique<Rts>(t);
          }
          break;
        }
        case 0b0001: {
          if (opcode == 0b0000000000011011) {
            // Make sure the full opcode matches
            // SLEEP                0000000000011011
            return std::make_unique<Sleep>(t);
          }
          break;
        }
        case 0b0010: {
          if (opcode == 0b0000000000101011) {
            // Make sure the full opcode matches
            // RTE                  0000000000101011
            return std::make_unique<Rte>(t);
          }
          break;
        }
        default:
          return std::nullopt;
      }
    case 0b1100:
      // MOV.B @(R0,Rm),Rn    0000nnnnmmmm1100
      return std::make_unique<MovbIndrR0RmRn>(t);
    case 0b1101:
      // MOV.W @(R0,Rm),Rn    0000nnnnmmmm1101
      return std::make_unique<MovwIndrR0RmRn>(t);
    case 0b1110:
      // MOV.L @(R0,Rm),Rn    0000nnnnmmmm1110
      return std::make_unique<MovlIndrR0RmRn>(t);
    case 0b1111:
      // MAC.L @Rm+,@Rn+      0000nnnnmmmm1111
      return std::make_unique<MaclIndrRmPostincIndrRnPostinc>(t);
    default:
      return std::nullopt;
  }
}

std::optional<std::unique_ptr<Instruction>> ParsePrefix0010(
    const IsaType &t, const uint16_t opcode) {
  switch (opcode & 0xF) {
    // Parse 4-bit suffix for opcode with prefix 0010
    case 0b0000:
      // MOV.B Rm,@Rn         0010nnnnmmmm0000
      return std::make_unique<MovbRmIndrRn>(t);
    case 0b0001:
      // MOV.W Rm,@Rn         0010nnnnmmmm0001
      return std::make_unique<MovwRmIndrRn>(t);
    case 0b0010:
      // MOV.L Rm,@Rn         0010nnnnmmmm0010
      return std::make_unique<MovlRmIndrRn>(t);
    case 0b0100:
      // MOV.B Rm,@–Rn        0010nnnnmmmm0100
      return std::make_unique<MovbRmIndrPredecRn>(t);
    case 0b0101:
      // MOV.W Rm,@–Rn        0010nnnnmmmm0101
      return std::make_unique<MovwRmIndrPredecRn>(t);
    case 0b0110:
      // MOV.L Rm,@–Rn        0010nnnnmmmm0110
      return std::make_unique<MovlRmIndrPredecRn>(t);
    case 0b0111:
      // DIV0S Rm,Rn          0010nnnnmmmm0111
      return std::make_unique<Div0sRmRn>(t);
    case 0b1000:
      // TST Rm,Rn            0010nnnnmmmm1000
      return std::make_unique<TstRmRn>(t);
    case 0b1001:
      // AND Rm,Rn            0010nnnnmmmm1001
      return std::make_unique<AndRmRn>(t);
    case 0b1010:
      // XOR Rm,Rn            0010nnnnmmmm1010
      return std::make_unique<XorRmRn>(t);
    case 0b1011:
      // OR Rm,Rn             0010nnnnmmmm1011
      return std::make_unique<OrRmRn>(t);
    case 0b1100:
      // CMP/STR Rm,Rn        0010nnnnmmmm1100
      return std::make_unique<CmpStrRmRn>(t);
    case 0b1101:
      // XTRCT Rm,Rn          0010nnnnmmmm1101
      return std::make_unique<XtrctRmRn>(t);
    case 0b1110:
      // MULU.W Rm,Rn         0010nnnnmmmm1110
      return std::make_unique<MuluwRmRn>(t);
    case 0b1111:
      // MULS.W Rm,Rn         0010nnnnmmmm1111
      return std::make_unique<MulswRmRn>(t);
    default:
      return std::nullopt;
  }
}

std::optional<std::unique_ptr<Instruction>> ParsePrefix0011(
    const IsaType &t, const uint16_t opcode) {
  switch (opcode & 0xF) {
    // Parse 4-bit suffix for opcode with prefix 0011
    case 0b0000:
      // CMP/EQ Rm,Rn         0011nnnnmmmm0000
      return std::make_unique<CmpEqRmRn>(t);
    case 0b0010:
      // CMP/HS Rm,Rn         0011nnnnmmmm0010
      return std::make_unique<CmpHsRmRn>(t);
    case 0b0011:
      // CMP/GE Rm,Rn         0011nnnnmmmm0011
      return std::make_unique<CmpGeRmRn>(t);
    case 0b0100:
      // DIV1 Rm,Rn           0011nnnnmmmm0100
      return std::make_unique<Div1RmRn>(t);
    case 0b0101:
      // DMULU.L Rm,Rn        0011nnnnmmmm0101
      return std::make_unique<DmululRmRn>(t);
    case 0b0110:
      // CMP/HI Rm,Rn         0011nnnnmmmm0110
      return std::make_unique<CmpHiRmRn>(t);
    case 0b0111:
      // CMP/GT Rm,Rn         0011nnnnmmmm0111
      return std::make_unique<CmpGtRmRn>(t);
    case 0b1000:
      // SUB Rm,Rn            0011nnnnmmmm1000
      return std::make_unique<SubRmRn>(t);
    case 0b1010:
      // SUBC Rm,Rn           0011nnnnmmmm1010
      return std::make_unique<SubcRmRn>(t);
    case 0b1011:
      // SUBV Rm,Rn           0011nnnnmmmm1011
      return std::make_unique<SubvRmRn>(t);
    case 0b1100:
      // ADD Rm,Rn            0011nnnnmmmm1100
      return std::make_unique<AddRmRn>(t);
    case 0b1101:
      // DMULS.L Rm,Rn        0011nnnnmmmm1101
      return std::make_unique<DmulslRmRn>(t);
    case 0b1110:
      // ADDC Rm,Rn           0011nnnnmmmm1110
      return std::make_unique<AddcRmRn>(t);
    case 0b1111:
      // ADDV Rm,Rn           0011nnnnmmmm1111
      return std::make_unique<AddvRmRn>(t);
    default:
      return std::nullopt;
  }
}

std::optional<std::unique_ptr<Instruction>> ParsePrefix0100(
    const IsaType &t, const uint16_t opcode) {
  switch (opcode & 0xF) {
    // Parse 4-bit suffix for opcode with prefix 0100
    case 0b0000:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 0000
        case 0b0000:
          // SHLL Rn              0100nnnn00000000
          return std::make_unique<ShllRn>(t);
        case 0b0001:
          // DT Rn                0100nnnn00010000
          return std::make_unique<DtRn>(t);
        case 0b0010:
          // SHAL Rn              0100nnnn00100000
          return std::make_unique<ShalRn>(t);
        default:
          return std::nullopt;
      }
    case 0b0001:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 0001
        case 0b0000:
          // SHLR Rn              0100nnnn00000001
          return std::make_unique<ShlrRn>(t);
        case 0b0001:
          // CMP/PZ Rn            0100nnnn00010001
          return std::make_unique<CmpPzRn>(t);
        case 0b0010:
          // SHAR Rn              0100nnnn00100001
          return std::make_unique<SharRn>(t);
        default:
          return std::nullopt;
      }
    case 0b0010:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 0010
        case 0b0000:
          // STS.L MACH,@–Rn      0100nnnn00000010
          return std::make_unique<StslMachIndrPredecRn>(t);
        case 0b0001:
          // STS.L MACL,@–Rn      0100nnnn00010010
          return std::make_unique<StslMaclIndrPredecRn>(t);
        case 0b0010:
          // STS.L PR,@–Rn        0100nnnn00100010
          return std::make_unique<StslPrIndrPredecRn>(t);
        case 0b0101:
          // STS.L FPUL,@-Rn      0100nnnn01010010
          return std::make_unique<StslFpulIndrPredecRn>(t);
        case 0b0110:
          // STS.L FPSCR,@-Rn     0100nnnn01100010
          return std::make_unique<StslFpscrIndrPredecRn>(t);
        default:
          return std::nullopt;
      }
    case 0b0011:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 0011
        case 0b0000:
          // STC.L SR,@–Rn        0100nnnn00000011
          return std::make_unique<StclSrIndrPredecRn>(t);
        case 0b0001:
          // STC.L GBR,@–Rn       0100nnnn00010011
          return std::make_unique<StclGbrIndrPredecRn>(t);
        case 0b0010:
          // STC.L VBR,@–Rn       0100nnnn00100011
          return std::make_unique<StclVbrIndrPredecRn>(t);
        default:
          return std::nullopt;
      }
    case 0b0100:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 0100
        case 0b0000:
          // ROTL Rn              0100nnnn00000100
          return std::make_unique<RotlRn>(t);
        //                    case 0b0001:
        //                        // SETRC Rm             0100mmmm00010100
        //                        return SetrcRm(t);
        case 0b0010:
          // ROTCL Rn             0100nnnn00100100
          return std::make_unique<RotclRn>(t);
        default:
          return std::nullopt;
      }
    case 0b0101:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 0101
        case 0b0000:
          // ROTR Rn              0100nnnn00000101
          return std::make_unique<RotrRn>(t);
        case 0b0001:
          // CMP/PL Rn            0100nnnn00010101
          return std::make_unique<CmpPlRn>(t);
        case 0b0010:
          // ROTCR Rn             0100nnnn00100101
          return std::make_unique<RotcrRn>(t);
        default:
          return std::nullopt;
      }
    case 0b0110:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 0110
        case 0b0000:
          // LDS.L @Rm+,MACH      0100mmmm00000110
          return std::make_unique<LdslIndrRmPostincMach>(t);
        case 0b0001:
          // LDS.L @Rm+,MACL      0100mmmm00010110
          return std::make_unique<LdslIndrRmPostincMacl>(t);
        case 0b0010:
          // LDS.L @Rm+,PR        0100mmmm00100110
          return std::make_unique<LdslIndrRmPostincPr>(t);
        case 0b0101:
          // LDS.L @Rm+, FPUL     0100mmmm01010110
          return std::make_unique<LdslIndrRmPostincFpul>(t);
        case 0b0110:
          // LDS.L @Rm+,DSR       0100mmmm01100110
          //                        return LdslIndrRmPostincDsr(t);
          // LDS.L @Rm+, FPSCR    0100mmmm01100110
          return std::make_unique<LdslIndrRmPostincFpscr>(t);
        default:
          return std::nullopt;
      }
    case 0b0111:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 0111
        case 0b0000:
          // LDC.L @Rm+,SR        0100mmmm00000111
          return std::make_unique<LdclIndrRmPostincSr>(t);
        case 0b0001:
          // LDC.L @Rm+,GBR       0100mmmm00010111
          return std::make_unique<LdclIndrRmPostincGbr>(t);
        case 0b0010:
          // LDC.L @Rm+,VBR       0100mmmm00100111
          return std::make_unique<LdclIndrRmPostincVbr>(t);
        default:
          return std::nullopt;
      }
    case 0b1000:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 1000
        case 0b0000:
          // SHLL2 Rn             0100nnnn00001000
          return std::make_unique<Shll2Rn>(t);
        case 0b0001:
          // SHLL8 Rn             0100nnnn00011000
          return std::make_unique<Shll8Rn>(t);
        case 0b0010:
          // SHLL16 Rn            0100nnnn00101000
          return std::make_unique<Shll16Rn>(t);
        default:
          return std::nullopt;
      }
    case 0b1001:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 1001
        case 0b0000:
          // SHLR2 Rn             0100nnnn00001001
          return std::make_unique<Shlr2Rn>(t);
        case 0b0001:
          // SHLR8 Rn             0100nnnn00011001
          return std::make_unique<Shlr8Rn>(t);
        case 0b0010:
          // SHLR16 Rn            0100nnnn00101001
          return std::make_unique<Shlr16Rn>(t);
        default:
          return std::nullopt;
      }
    case 0b1010:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 1010
        case 0b0000:
          // LDS Rm,MACH          0100mmmm00001010
          return std::make_unique<LdsRmMach>(t);
        case 0b0001:
          // LDS Rm,MACL          0100mmmm00011010
          return std::make_unique<LdsRmMacl>(t);
        case 0b0010:
          // LDS Rm,PR            0100mmmm00101010
          return std::make_unique<LdsRmPr>(t);
        case 0b0101:
          // LDS Rm,FPUL          0100mmmm01011010
          return std::make_unique<LdsRmFpul>(t);
        //                    case 0b0110:
        //                        // LDS Rm,DSR           0100mmmm01101010 //
        //                        return LdsRmDsr(t);
        case 0b0110:
          // LDS Rm,FPSCR         0100mmmm01101010
          return std::make_unique<LdsRmFpscr>(t);
        default:
          return std::nullopt;
      }
    case 0b1011:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 1011
        case 0b0000:
          // JSR @Rm              0100mmmm00001011
          return std::make_unique<JsrIndrRm>(t);
        case 0b0001:
          // TAS.B @Rn            0100nnnn00011011
          return std::make_unique<TasbIndrRn>(t);
        case 0b0010:
          // JMP @Rm              0100mmmm00101011
          return std::make_unique<JmpIndrRm>(t);
        default:
          return std::nullopt;
      }
    case 0b1110:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 0100 and suffix 1110
        case 0b0000:
          // LDC  Rm,SR           0100mmmm00001110
          return std::make_unique<LdcRmSr>(t);
        case 0b0001:
          // LDC Rm,GBR           0100mmmm00011110
          return std::make_unique<LdcRmGbr>(t);
        case 0b0010:
          // LDC Rm,VBR           0100mmmm00101110
          return std::make_unique<LdcRmVbr>(t);
        default:
          return std::nullopt;
      }
    case 0b1111:
      // MAC.W @Rm+,@Rn+      0100nnnnmmmm1111
      return std::make_unique<MacwIndrRmPostincIndrRnPostinc>(t);
    default:
      return std::nullopt;
  }
}

std::optional<std::unique_ptr<Instruction>> ParsePrefix0110(
    const IsaType &t, const uint16_t opcode) {
  switch (opcode & 0xF) {
    // Parse 4-bit suffix for opcode with prefix 0110
    case 0b0000:
      // MOV.B @Rm,Rn         0110nnnnmmmm0000
      return std::make_unique<MovbIndrRmRn>(t);
    case 0b0001:
      // MOV.W @Rm,Rn         0110nnnnmmmm0001
      return std::make_unique<MovwIndrRmRn>(t);
    case 0b0010:
      // MOV.L @Rm,Rn         0110nnnnmmmm0010
      return std::make_unique<MovlIndrRmRn>(t);
    case 0b0011:
      // MOV Rm,Rn            0110nnnnmmmm0011
      return std::make_unique<MovRmRn>(t);
    case 0b0100:
      // MOV.B @Rm+,Rn        0110nnnnmmmm0100
      return std::make_unique<MovbIndrRmPostincRn>(t);
    case 0b0101:
      // MOV.W @Rm+,Rn        0110nnnnmmmm0101
      return std::make_unique<MovwIndrRmPostincRn>(t);
    case 0b0110:
      // MOV.L @Rm+,Rn        0110nnnnmmmm0110
      return std::make_unique<MovlIndrRmPostincRn>(t);
    case 0b0111:
      // NOT Rm,Rn            0110nnnnmmmm0111
      return std::make_unique<NotRmRn>(t);
    case 0b1000:
      // SWAP.B Rm,Rn         0110nnnnmmmm1000
      return std::make_unique<SwapbRmRn>(t);
    case 0b1001:
      // SWAP.W Rm,Rn         0110nnnnmmmm1001
      return std::make_unique<SwapwRmRn>(t);
    case 0b1010:
      // NEGC Rm,Rn           0110nnnnmmmm1010
      return std::make_unique<NegcRmRn>(t);
    case 0b1011:
      // NEG Rm,Rn            0110nnnnmmmm1011
      return std::make_unique<NegRmRn>(t);
    case 0b1100:
      // EXTU.B Rm,Rn         0110nnnnmmmm1100
      return std::make_unique<ExtubRmRn>(t);
    case 0b1101:
      // EXTU.W Rm,Rn         0110nnnnmmmm1101
      return std::make_unique<ExtuwRmRn>(t);
    case 0b1110:
      // EXTS.B Rm,Rn         0110nnnnmmmm1110
      return std::make_unique<ExtsbRmRn>(t);
    case 0b1111:
      // EXTS.W Rm,Rn         0110nnnnmmmm1111
      return std::make_unique<ExtswRmRn>(t);
    default:
      return std::nullopt;
  }
}

std::optional<std::unique_ptr<Instruction>> ParsePrefix1000(
    const IsaType &t, const uint16_t opcode) {
  switch ((opcode >> 8) & 0xF) {
    // Parse bits 8-11 of opcode starting with 1000
    case 0b0000:
      // MOV.B R0,@(disp,Rn)  10000000nnnndddd
      return std::make_unique<MovbR0IndrDispRn>(t);
    case 0b0001:
      // MOV.W R0,@(disp,Rn)  10000001nnnndddd
      return std::make_unique<MovwR0IndrDispRn>(t);
    case 0b0100:
      // MOV.B @(disp,Rm),R0  10000100mmmmdddd
      return std::make_unique<MovbIndrDispRmR0>(t);
    case 0b0101:
      // MOV.W @(disp,Rm),R0  10000101mmmmdddd
      return std::make_unique<MovwIndrDispRmR0>(t);
    case 0b1000:
      // CMP/EQ #imm,R0       10001000iiiiiiii
      return std::make_unique<CmpEqImmR0>(t);
    case 0b1001:
      // BT label             10001001dddddddd
      return std::make_unique<BtDisp>(t);
    case 0b1011:
      // BF label             10001011dddddddd
      return std::make_unique<BfDisp>(t);
    case 0b1101:
      // BT/S label           10001101dddddddd
      return std::make_unique<BtsDisp>(t);
    case 0b1111:
      // BF/S label           10001111dddddddd
      return std::make_unique<BfsDisp>(t);
    default:
      return std::nullopt;
  }
}

std::optional<std::unique_ptr<Instruction>> ParsePrefix1100(
    const IsaType &t, const uint16_t opcode) {
  switch ((opcode >> 8) & 0xF) {
    // Parse bits 8-11 of opcode starting with 1100
    case 0b0000:
      // MOV.B R0,@(disp,GBR) 11000000dddddddd
      return std::make_unique<MovbR0IndrDispGbr>(t);
    case 0b0001:
      // MOV.W R0,@(disp,GBR) 11000001dddddddd
      return std::make_unique<MovwR0IndrDispGbr>(t);
    case 0b0010:
      // MOV.L R0,@(disp,GBR) 11000010dddddddd
      return std::make_unique<MovlR0IndrDispGbr>(t);
    case 0b0011:
      // TRAPA #imm           11000011iiiiiiii
      return std::make_unique<TrapaImm>(t);
    case 0b0100:
      // MOV.B @(disp,GBR),R0 11000100dddddddd
      return std::make_unique<MovbIndrDispGbrR0>(t);
    case 0b0101:
      // MOV.W @(disp,GBR),R0 11000101dddddddd
      return std::make_unique<MovwIndrDispGbrR0>(t);
    case 0b0110:
      // MOV.L @(disp,GBR),R0 11000110dddddddd
      return std::make_unique<MovlIndrDispGbrR0>(t);
    case 0b0111:
      // MOVA @(disp,PC),R0   11000111dddddddd
      return std::make_unique<MovaIndrDispPcR0>(t);
    case 0b1000:
      // TST #imm,R0          11001000iiiiiiii
      return std::make_unique<TstImmR0>(t);
    case 0b1001:
      // AND #imm,R0          11001001iiiiiiii
      return std::make_unique<AndImmR0>(t);
    case 0b1010:
      // XOR #imm,R0          11001010iiiiiiii
      return std::make_unique<XorImmR0>(t);
    case 0b1011:
      // OR #imm,R0           11001011iiiiiiii
      return std::make_unique<OrImmR0>(t);
    case 0b1100:
      // TST.B #imm,@(R0,GBR) 11001100iiiiiiii
      return std::make_unique<TstbImmIndrR0Gbr>(t);
    case 0b1101:
      // AND.B #imm,@(R0,GBR) 11001101iiiiiiii
      return std::make_unique<AndbImmIndrR0Gbr>(t);
    case 0b1110:
      // XOR.B #imm,@(R0,GBR) 11001110iiiiiiii
      return std::make_unique<XorbImmIndrR0Gbr>(t);
    case 0b1111:
      // OR.B #imm,@(R0,GBR)  11001111iiiiiiii
      return std::make_unique<OrbImmIndrR0Gbr>(t);
    default:
      return std::nullopt;
  }
}

std::optional<std::unique_ptr<Instruction>> ParsePrefix1111(
    const IsaType &t, const uint16_t opcode) {
  switch (opcode & 0xF) {
    // 4-bit suffix for opcode starting with 1111
    case 0b0000:
      // FADD FRm,FRn         1111nnnnmmmm0000
      return std::make_unique<FaddFrmFrn>(t);
    case 0b0001:
      // FSUB FRm,FRn         1111nnnnmmmm0001
      return std::make_unique<FsubFrmFrn>(t);
    case 0b0010:
      // FMUL FRm,FRn         1111nnnnmmmm0010
      return std::make_unique<FmulFrmFrn>(t);
    case 0b0011:
      // FDIV FRm,FRn         1111nnnnmmmm0011
      return std::make_unique<FdivFrmFrn>(t);
    case 0b0100:
      // FCMP/EQ FRm,FRn      1111nnnnmmmm0100
      return std::make_unique<FcmpEqFrmFrn>(t);
    case 0b0101:
      // FCMP/GT FRm,FRn      1111nnnnmmmm0101
      return std::make_unique<FcmpGtFrmFrn>(t);
    case 0b0110:
      // FMOV.S @(R0,Rm),FRn  1111nnnnmmmm0110
      return std::make_unique<FmovsIndrR0RmFrn>(t);
    case 0b0111:
      // FMOV.S FRm,@(R0,Rn)  1111nnnnmmmm0111
      return std::make_unique<FmovsFrmIndrR0Rn>(t);
    case 0b1000:
      // FMOV.S @Rm,FRn       1111nnnnmmmm1000
      return std::make_unique<FmovsIndrRmFrn>(t);
    case 0b1001:
      // FMOV.S @Rm+,FRn      1111nnnnmmmm1001
      return std::make_unique<FmovsIndrRmPostincFrn>(t);
    case 0b1010:
      // FMOV.S FRm,@Rn       1111nnnnmmmm1010
      return std::make_unique<FmovsFrmIndrRn>(t);
    case 0b1011:
      // FMOV.S FRm,@-Rn      1111nnnnmmmm1011
      return std::make_unique<FmovsFrmIndrPredecRn>(t);
    case 0b1100:
      // FMOV FRm,FRn         1111nnnnmmmm1100
      return std::make_unique<FmovFrmFrn>(t);
    case 0b1101:
      switch ((opcode >> 4) & 0xF) {
        // Parse bits 4-7 of the opcode with prefix 1111 and suffix 1101
        case 0b0000:
          // FSTS FPUL,FRn        1111nnnn00001101
          return std::make_unique<FstsFpulFrn>(t);
        case 0b0001:
          // FLDS FRm,FPUL        1111mmmm00011101
          return std::make_unique<FldsFrmFpul>(t);
        case 0b0010:
          // FLOAT FPUL,FRn       1111nnnn00101101
          return std::make_unique<FloatFpulFrn>(t);
        case 0b0011:
          // FTRC FRm,FPUL        1111mmmm00111101
          return std::make_unique<FtrcFrmFpul>(t);
        case 0b0100:
          // FNEG FRn             1111nnnn01001101
          return std::make_unique<FnegFrn>(t);
        case 0b0101:
          // FABS FRn             1111nnnn01011101
          return std::make_unique<FabsFrn>(t);
        case 0b1000:
          // FLDI0 FRn            1111nnnn10001101
          return std::make_unique<Fldi0Frn>(t);
        case 0b1001:
          // FLDI1 FRn            1111nnnn10011101
          return std::make_unique<Fldi1Frn>(t);
        default:
          return std::nullopt;
      }
    case 0b1110:
      // FMAC FR0,FRm,FRn     1111nnnnmmmm1110
      return std::make_unique<FmacFr0FrmFrn>(t);
    default:
      return std::nullopt;
  }
}
}  // namespace SuperH
