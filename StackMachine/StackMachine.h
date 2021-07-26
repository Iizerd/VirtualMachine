#ifndef __STACK_MACHINE_H
#define __STACK_MACHINE_H

#include <Windows.h>

typedef union _VM_CONTEXT
{
	PVOID BP;
	PVOID Num[32];
}VM_CONTEXT, *PVM_CONTEXT;

typedef struct _VM_HANDLER_TABLE
{
	PVOID HandlerTable[256];
}VM_HANDLER_TABLE, *PVM_HANDLER_TABLE;

typedef struct _VM_AUX_STORAGE
{
	PVOID Storage[32];
}VM_AUX_STORAGE, *PVM_AUX_STORAGE;

enum REGISTERID : UCHAR
{
	BP,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15,
	R16,
	R17,
	R18,
	R19,
	R20,
	R21,
	R22,
	R23,
	R24,
	R25,
	R26,
	R27,
	R28,
	R29,
	R30,
	R31
};

enum OPCODES : UCHAR
{
	VMENTER,
	VMEXIT,	//sets vip to 0
	NOP,		//xD waste timme

	LDIB,		//put immediate values on stack
	LDIW,
	LDID,
	LDIQ,

	LDIBSXW,	//load immediate and sign extend
	LDIBSXD,
	LDIBSXQ,
	LDIWSXD,
	LDIWSXQ,
	LDIDSXQ,

	LDIBZXW,	//load immediate and zero extend
	LDIBZXD,
	LDIBZXQ,
	LDIWZXD,
	LDIWZXQ,
	LDIDZXQ,

	LDAB,		//reads value from the address stored on top of stack, stores said value on stack
	LDAW,
	LDAD,
	LDAQ,

	STAB,		//pops value form stack, stores it at address in the next 8 bytes
	STAW,
	STAD,
	STAQ,

				//	sign extend value on top of stack
	SXBW,		//  sx	byte to word
	SXBD,		//	sx	byte to dword
	SXBQ,		//	sx	byte to qword
	SXWD,		//	sx	word to dword
	SXWQ,		//	sx	word to qword
	SXDQ,		//	sx	double to qword

				//	zero extend value on top of stack
	ZXBW,		//  zx	byte to word
	ZXBD,		//	zx	byte to dword
	ZXBQ,		//	zx	byte to qword
	ZXWD,		//	zx	word to dword
	ZXWQ,		//	zx	word to qword
	ZXDQ,		//	zx	double to qword

	PUSHB,		//push value in reg onto stack
	PUSHW,
	PUSHD,
	PUSHQ,

	POPB,		//pop values from stack to reg
	POPW,
	POPD,
	POPQ,

	NPOPB,		//pop values to nowhere
	NPOPW,
	NPOPD,
	NPOPQ,

	ADDB,		//add two values on stack, push rflags
	ADDW,
	ADDD,
	ADDQ,

	SUBB,		//sub two values on stack, push rflags
	SUBW,
	SUBD,
	SUBQ,

	MULB,		//mul two values on stack, push rflags
	MULW,		//Stack After:	[LOW BITS]
	MULD,		//				[HIGH BITS]
	MULQ,		//				[RFLAGS]

	DIVB,		//div two values on stack, push rflags
	DIVW,		//stack order bottom up, Divisor(+0), DividendHIGH(+Size), DividendLOW(+Size*2h)
	DIVD,		//Stack After:	[Quotient]
	DIVQ,		//				[Remainder]
				//				[RFLAGS]

	ANDB,		//and two values on stack, push rflags
	ANDW,
	ANDD,
	ANDQ,

	ORB,		//or two values on stack, push rflags
	ORW,
	ORD,
	ORQ,

	XORB,		//xor two values on stack, push rflags
	XORW,
	XORD,
	XORQ,

	SHLB,		//shift left, push rflags
	SHLW,
	SHLD,
	SHLQ,

	SHRB,		//shift right, push rflags
	SHRW,
	SHRD,
	SHRQ,

	CMPB,		//compares two values on stack, pushes flags
	CMPW,		//DOES NOT POP THE VALUES OFF THE STACK
	CMPD,
	CMPQ,

	JMP,		//adds 8 byte value on top of stack to vip
	JMPI,		//jmps by immediate
	JL,			//conditional jumps pop rflags from stack then
	JLE,		//	add the next 8 bytes on stack to vip based on rflags 
	JG,
	JGE,
	JZ,
	JNZ,

	CALL,		//saves IP+1, does IP=(8 bytes on top of stack), pushes IP+1 
				//convention will be stdcall so arguments passed left to right on stack
				//return value put into r31
				

				//caller cleans the stack..............
	RET,		//exchanges the current top value and value below it(the return address)
				//then sets IP to the return address

	X86CALL,	//convention:
				//prior to call push all arguments u want to pass onto the stack right to left
				//AT LEAST 4 ARGUMENTS MUST BE PUSHED ONTO STACK(THEY CAN BE ZERO IF U DONT WANT TO PASS ANYTHING)
				//this means arg1 is at the bottom of the stack
				//then push the address u want to call onto the stack
				//the address u want to call is popped off temporarily, as are the first 4 arguments
				//the arguments are put into their respective x86 registers(rcx,rdx,r8,r9)
				//then the address is called
	
	EDITSP,		//one byte for the op code, a signed short to represent the stack change
				//

	STSP,		//store/load stack pointer to a register
	LDSP,
	STIP,		//store/load instruction pointer to a register
	LDIP,

	SWAPB,
	SWAPW,
	SWAPD,
	SWAPQ,

	DECB,
	DECW,
	DECD,
	DECQ,
	INCB,
	INCW,
	INCD,
	INCQ,

	SWAPBQ,			//helpful after arithmetic operations
	SWAPWQ,			//pop an 8 byte value(usually rflags) from the stack,
	SWAPDQ,			//then pop the specified size, and push them back in each others positions
					//swapping them....

	
	LDSB,			//load something from [rsp+offset]
	LDSW,
	LDSD,
	LDSQ,

	STSB,			//store something at [rsp+offset]
	STSW,
	STSD,
	STSQ,

	LDBPB,
	LDBPW,
	LDBPD,
	LDBPQ,

	STBPB,
	STBPW,
	STBPD,
	STBPQ,

};

EXTERN_C VOID VmEnter(PVM_CONTEXT Context, PVOID Code, PVM_HANDLER_TABLE HandlerTable, PVM_AUX_STORAGE Storage);

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

VOID InitHandlerTable(PVM_HANDLER_TABLE Table)
{
	Table->HandlerTable[VMEXIT] = ViVmExit;
	Table->HandlerTable[NOP] = ViNop;

	Table->HandlerTable[LDIB] = ViLdib;
	Table->HandlerTable[LDIW] = ViLdiw;
	Table->HandlerTable[LDID] = ViLdid;
	Table->HandlerTable[LDIQ] = ViLdiq;

	Table->HandlerTable[LDIBSXW] = ViLdibsxw;
	Table->HandlerTable[LDIBSXD] = ViLdibsxd;
	Table->HandlerTable[LDIBSXQ] = ViLdibsxq;
	Table->HandlerTable[LDIWSXD] = ViLdiwsxd;
	Table->HandlerTable[LDIWSXQ] = ViLdiwsxq;
	Table->HandlerTable[LDIDSXQ] = ViLdidsxq;

	Table->HandlerTable[LDIBZXW] = ViLdibzxw;
	Table->HandlerTable[LDIBZXD] = ViLdibzxd;
	Table->HandlerTable[LDIBZXQ] = ViLdibzxq;
	Table->HandlerTable[LDIWZXD] = ViLdiwzxd;
	Table->HandlerTable[LDIWZXQ] = ViLdiwzxq;
	Table->HandlerTable[LDIDZXQ] = ViLdidzxq;

	Table->HandlerTable[LDAB] = ViLdab;
	Table->HandlerTable[LDAW] = ViLdaw;
	Table->HandlerTable[LDAD] = ViLdad;
	Table->HandlerTable[LDAQ] = ViLdaq;

	Table->HandlerTable[STAB] = ViStab;
	Table->HandlerTable[STAW] = ViStaw;
	Table->HandlerTable[STAD] = ViStad;
	Table->HandlerTable[STAQ] = ViStaq;

	Table->HandlerTable[SXBW] = ViSxbw;
	Table->HandlerTable[SXBD] = ViSxbd;
	Table->HandlerTable[SXBQ] = ViSxbq;
	Table->HandlerTable[SXWD] = ViSxwd;
	Table->HandlerTable[SXWQ] = ViSxwq;
	Table->HandlerTable[SXDQ] = ViSxdq;

	Table->HandlerTable[ZXBW] = ViZxbw;
	Table->HandlerTable[ZXBD] = ViZxbd;
	Table->HandlerTable[ZXBQ] = ViZxbq;
	Table->HandlerTable[ZXWD] = ViZxwd;
	Table->HandlerTable[ZXWQ] = ViZxwq;
	Table->HandlerTable[ZXDQ] = ViZxdq;

	Table->HandlerTable[PUSHB] = ViPushb;
	Table->HandlerTable[PUSHW] = ViPushw;
	Table->HandlerTable[PUSHD] = ViPushd;
	Table->HandlerTable[PUSHQ] = ViPushq;

	Table->HandlerTable[POPB] = ViPopb;
	Table->HandlerTable[POPW] = ViPopw;
	Table->HandlerTable[POPD] = ViPopd;
	Table->HandlerTable[POPQ] = ViPopq;

	Table->HandlerTable[NPOPB] = ViNpopb;
	Table->HandlerTable[NPOPW] = ViNpopw;
	Table->HandlerTable[NPOPD] = ViNpopd;
	Table->HandlerTable[NPOPQ] = ViNpopq;

	Table->HandlerTable[ADDB] = ViAddb;
	Table->HandlerTable[ADDW] = ViAddw;
	Table->HandlerTable[ADDD] = ViAddd;
	Table->HandlerTable[ADDQ] = ViAddq;

	Table->HandlerTable[SUBB] = ViSubb;
	Table->HandlerTable[SUBW] = ViSubw;
	Table->HandlerTable[SUBD] = ViSubd;
	Table->HandlerTable[SUBQ] = ViSubq;

	Table->HandlerTable[MULB] = ViMulb;
	Table->HandlerTable[MULW] = ViMulw;
	Table->HandlerTable[MULD] = ViMuld;
	Table->HandlerTable[MULQ] = ViMulq;

	Table->HandlerTable[DIVB] = ViDivb;
	Table->HandlerTable[DIVW] = ViDivw;
	Table->HandlerTable[DIVD] = ViDivd;
	Table->HandlerTable[DIVQ] = ViDivq;

	Table->HandlerTable[ANDB] = ViAndb;
	Table->HandlerTable[ANDW] = ViAndw;
	Table->HandlerTable[ANDD] = ViAndd;
	Table->HandlerTable[ANDQ] = ViAndq;

	Table->HandlerTable[ORB] = ViOrb;
	Table->HandlerTable[ORW] = ViOrw;
	Table->HandlerTable[ORD] = ViOrd;
	Table->HandlerTable[ORQ] = ViOrq;

	Table->HandlerTable[XORB] = ViXorb;
	Table->HandlerTable[XORW] = ViXorw;
	Table->HandlerTable[XORD] = ViXord;
	Table->HandlerTable[XORQ] = ViXorq;

	Table->HandlerTable[SHLB] = ViShlb;
	Table->HandlerTable[SHLW] = ViShlw;
	Table->HandlerTable[SHLD] = ViShld;
	Table->HandlerTable[SHLQ] = ViShlq;

	Table->HandlerTable[SHRB] = ViShrb;
	Table->HandlerTable[SHRW] = ViShrw;
	Table->HandlerTable[SHRD] = ViShrd;
	Table->HandlerTable[SHRQ] = ViShrq;

	Table->HandlerTable[CMPB] = ViCmpb;
	Table->HandlerTable[CMPW] = ViCmpw;
	Table->HandlerTable[CMPD] = ViCmpd;
	Table->HandlerTable[CMPQ] = ViCmpq;

	Table->HandlerTable[JMP] = ViJmp;
	Table->HandlerTable[JMPI] = ViJmpi;
	Table->HandlerTable[JL] = ViJl;
	Table->HandlerTable[JLE] = ViJle;
	Table->HandlerTable[JG] = ViJg;
	Table->HandlerTable[JGE] = ViJge;
	Table->HandlerTable[JZ] = ViJz;
	Table->HandlerTable[JNZ] = ViJnz;

	Table->HandlerTable[CALL] = ViCall;
	Table->HandlerTable[RET] = ViRet;
	Table->HandlerTable[X86CALL] = ViX86Call;

	Table->HandlerTable[EDITSP] = ViEditsp;

	Table->HandlerTable[STSP] = ViStsp;
	Table->HandlerTable[LDSP] = ViLdsp;
	Table->HandlerTable[STIP] = ViStip;
	Table->HandlerTable[LDIP] = ViLdip;

	Table->HandlerTable[SWAPB] = ViSwapb;
	Table->HandlerTable[SWAPW] = ViSwapw;
	Table->HandlerTable[SWAPD] = ViSwapd;
	Table->HandlerTable[SWAPQ] = ViSwapq;

	Table->HandlerTable[DECB] = ViDecb;
	Table->HandlerTable[DECW] = ViDecw;
	Table->HandlerTable[DECD] = ViDecd;
	Table->HandlerTable[DECQ] = ViDecq;

	Table->HandlerTable[INCB] = ViIncb;
	Table->HandlerTable[INCW] = ViIncw;
	Table->HandlerTable[INCD] = ViIncd;
	Table->HandlerTable[INCQ] = ViIncq;

	Table->HandlerTable[SWAPBQ] = ViSwapbq;
	Table->HandlerTable[SWAPWQ] = ViSwapwq;
	Table->HandlerTable[SWAPDQ] = ViSwapdq;

	Table->HandlerTable[LDSB] = ViLdsb;
	Table->HandlerTable[LDSW] = ViLdsw;
	Table->HandlerTable[LDSD] = ViLdsd;
	Table->HandlerTable[LDSQ] = ViLdsq;

	Table->HandlerTable[STSB] = ViStsb;
	Table->HandlerTable[STSW] = ViStsw;
	Table->HandlerTable[STSD] = ViStsd;
	Table->HandlerTable[STSQ] = ViStsq;

	Table->HandlerTable[LDBPB] = ViLdbpb;
	Table->HandlerTable[LDBPW] = ViLdbpw;
	Table->HandlerTable[LDBPD] = ViLdbpd;
	Table->HandlerTable[LDBPQ] = ViLdbpq;

	Table->HandlerTable[STBPB] = ViStbpb;
	Table->HandlerTable[STBPW] = ViStbpw;
	Table->HandlerTable[STBPD] = ViStbpd;
	Table->HandlerTable[STBPQ] = ViStbpq;
}

#endif