#ifndef _VMHANDLERDEFS_H
#define _VMHANDLERDEFS_H


#include <Windows.h>

EXTERN_C VOID VmEnter(PVOID Context, PVOID Code, PVOID HandlerTable, PVOID Storage);

EXTERN_C VOID ViVmExit();
EXTERN_C VOID ViNop();

EXTERN_C VOID ViLdib();
EXTERN_C VOID ViLdiw();
EXTERN_C VOID ViLdid();
EXTERN_C VOID ViLdiq();

EXTERN_C VOID ViLdibsxw();
EXTERN_C VOID ViLdibsxd();
EXTERN_C VOID ViLdibsxq();
EXTERN_C VOID ViLdiwsxd();
EXTERN_C VOID ViLdiwsxq();
EXTERN_C VOID ViLdidsxq();

EXTERN_C VOID ViLdibzxw();
EXTERN_C VOID ViLdibzxd();
EXTERN_C VOID ViLdibzxq();
EXTERN_C VOID ViLdiwzxd();
EXTERN_C VOID ViLdiwzxq();
EXTERN_C VOID ViLdidzxq();

EXTERN_C VOID ViLdab();
EXTERN_C VOID ViLdaw();
EXTERN_C VOID ViLdad();
EXTERN_C VOID ViLdaq();

EXTERN_C VOID ViStab();
EXTERN_C VOID ViStaw();
EXTERN_C VOID ViStad();
EXTERN_C VOID ViStaq();

EXTERN_C VOID ViSxbw();
EXTERN_C VOID ViSxbd();
EXTERN_C VOID ViSxbq();
EXTERN_C VOID ViSxwd();
EXTERN_C VOID ViSxwq();
EXTERN_C VOID ViSxdq();

EXTERN_C VOID ViZxbw();
EXTERN_C VOID ViZxbd();
EXTERN_C VOID ViZxbq();
EXTERN_C VOID ViZxwd();
EXTERN_C VOID ViZxwq();
EXTERN_C VOID ViZxdq();

EXTERN_C VOID ViPushb();
EXTERN_C VOID ViPushw();
EXTERN_C VOID ViPushd();
EXTERN_C VOID ViPushq();

EXTERN_C VOID ViPopb();
EXTERN_C VOID ViPopw();
EXTERN_C VOID ViPopd();
EXTERN_C VOID ViPopq();

EXTERN_C VOID ViNpopb();
EXTERN_C VOID ViNpopw();
EXTERN_C VOID ViNpopd();
EXTERN_C VOID ViNpopq();

EXTERN_C VOID ViAddb();
EXTERN_C VOID ViAddw();
EXTERN_C VOID ViAddd();
EXTERN_C VOID ViAddq();

EXTERN_C VOID ViSubb();
EXTERN_C VOID ViSubw();
EXTERN_C VOID ViSubd();
EXTERN_C VOID ViSubq();

EXTERN_C VOID ViMulb();
EXTERN_C VOID ViMulw();
EXTERN_C VOID ViMuld();
EXTERN_C VOID ViMulq();

EXTERN_C VOID ViDivb();
EXTERN_C VOID ViDivw();
EXTERN_C VOID ViDivd();
EXTERN_C VOID ViDivq();

EXTERN_C VOID ViAndb();
EXTERN_C VOID ViAndw();
EXTERN_C VOID ViAndd();
EXTERN_C VOID ViAndq();

EXTERN_C VOID ViOrb();
EXTERN_C VOID ViOrw();
EXTERN_C VOID ViOrd();
EXTERN_C VOID ViOrq();

EXTERN_C VOID ViXorb();
EXTERN_C VOID ViXorw();
EXTERN_C VOID ViXord();
EXTERN_C VOID ViXorq();

EXTERN_C VOID ViShlb();
EXTERN_C VOID ViShlw();
EXTERN_C VOID ViShld();
EXTERN_C VOID ViShlq();

EXTERN_C VOID ViShrb();
EXTERN_C VOID ViShrw();
EXTERN_C VOID ViShrd();
EXTERN_C VOID ViShrq();

EXTERN_C VOID ViCmpb();
EXTERN_C VOID ViCmpw();
EXTERN_C VOID ViCmpd();
EXTERN_C VOID ViCmpq();

EXTERN_C VOID ViJmp();
EXTERN_C VOID ViJmpi();
EXTERN_C VOID ViJl();
EXTERN_C VOID ViJle();
EXTERN_C VOID ViJg();
EXTERN_C VOID ViJge();
EXTERN_C VOID ViJz();
EXTERN_C VOID ViJnz();

EXTERN_C VOID ViCall();
EXTERN_C VOID ViRet();
EXTERN_C VOID ViX86Call();

EXTERN_C VOID ViEditsp();

EXTERN_C VOID ViStsp();
EXTERN_C VOID ViLdsp();
EXTERN_C VOID ViStip();
EXTERN_C VOID ViLdip();

EXTERN_C VOID ViSwapb();
EXTERN_C VOID ViSwapw();
EXTERN_C VOID ViSwapd();
EXTERN_C VOID ViSwapq();

EXTERN_C VOID ViDecb();
EXTERN_C VOID ViDecw();
EXTERN_C VOID ViDecd();
EXTERN_C VOID ViDecq();

EXTERN_C VOID ViIncb();
EXTERN_C VOID ViIncw();
EXTERN_C VOID ViIncd();
EXTERN_C VOID ViIncq();

EXTERN_C VOID ViSwapbq();
EXTERN_C VOID ViSwapwq();
EXTERN_C VOID ViSwapdq();

EXTERN_C VOID ViLdsb();
EXTERN_C VOID ViLdsw();
EXTERN_C VOID ViLdsd();
EXTERN_C VOID ViLdsq();

EXTERN_C VOID ViStsb();
EXTERN_C VOID ViStsw();
EXTERN_C VOID ViStsd();
EXTERN_C VOID ViStsq();

EXTERN_C VOID ViLdbpb();
EXTERN_C VOID ViLdbpw();
EXTERN_C VOID ViLdbpd();
EXTERN_C VOID ViLdbpq();

EXTERN_C VOID ViStbpb();
EXTERN_C VOID ViStbpw();
EXTERN_C VOID ViStbpd();
EXTERN_C VOID ViStbpq();


#endif