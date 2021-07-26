#ifndef _VMASSEMBLER_H
#define _VMASSEMBLER_H

#define INSTRUCTION_JMP_SIZE 0x8
#define INSTRUCTION_JMP_NUMBER 0x480FB60241FF24C0
#define INSTRUCTION_JMP_NUMBER_R 0xC024FF4102B60F48

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <fstream>

#include "VmSdk.h"
#include "VmDefs.h"
#include "VmHandlerDefs.h"

#define STRING std::string
#define VECTOR std::vector

#define JCC_INSTRUCTION_LENGTH 22

typedef struct _CODE_LINK
{
	_CODE_LINK* Next;
	_CODE_LINK* Prev;
	UCHAR IsLabel;			//XD
	UCHAR InstructionId;	//THE ID FOR ISNTRUCTION IN DEFAULT HANDLER TABLE
	PVOID Data;				//ANY POSSIBLE IMMEDIATE VALUE
	STRING Name;			//NAME OF LABEL IF USED(JUMPS OR LABELS THEMSELVES)
}CODE_LINK, *PCODE_LINK;

typedef struct _VM_INSTRUCTION
{
	VECTOR<UCHAR> OpCodes;
	UCHAR IsUsed;
	ULONG Size;
	PVOID Code;
}VM_INSTRUCTION, *PVM_INSTRUCTION;

typedef struct _VM_ASSEMBLER
{
	PCODE_LINK CodeStart;
	PCODE_LINK CodeEnd;
	VM_INSTRUCTION Instructions[256];
	VECTOR<PVM_INSTRUCTION> UsedInstructions;
}VM_ASSEMBLER, *PVM_ASSEMBLER;

INLINE CONST CHAR* InstructionBindings[]{
	"VMENTER",
	"VMEXIT",	//sets vip to 0
	"NOP",

	"LDIB",		//put immediate values on stack
	"LDIW",
	"LDID",
	"LDIQ",

	"LDIBSXW",	//load immediate and sign extend
	"LDIBSXD",
	"LDIBSXQ",
	"LDIWSXD",
	"LDIWSXQ",
	"LDIDSXQ",

	"LDIBZXW",	//load immediate and zero extend
	"LDIBZXD",
	"LDIBZXQ",
	"LDIWZXD",
	"LDIWZXQ",
	"LDIDZXQ",

	"LDAB",		//pops address from stack, reads and stores value onto stack
	"LDAW",
	"LDAD",
	"LDAQ",

	"STAB",		//pops address from stack, pops value from stack, puts value into address
	"STAW",
	"STAD",
	"STAQ",

	//	sign extend value on top of stack
	"SXBW",		//  sx	byte to word
	"SXBD",		//	sx	byte to dword
	"SXBQ",		//	sx	byte to qword
	"SXWD",		//	sx	word to dword
	"SXWQ",		//	sx	word to qword
	"SXDQ",		//	sx	double to qword

			//	zero extend value on top of stack
	"ZXBW",		//  zx	byte to word
	"ZXBD",		//	zx	byte to dword
	"ZXBQ",		//	zx	byte to qword
	"ZXWD",		//	zx	word to dword
	"ZXWQ",		//	zx	word to qword
	"ZXDQ",		//	zx	double to qword

	"PUSHB",		//push value in reg onto stack
	"PUSHW",
	"PUSHD",
	"PUSHQ",

	"POPB",		//pop values from stack to reg
	"POPW",
	"POPD",
	"POPQ",

	"NPOPB",		//pop values to nowhere
	"NPOPW",
	"NPOPD",
	"NPOPQ",

	"ADDB",		//add two values on stack, push rflags
	"ADDW",
	"ADDD",
	"ADDQ",

	"SUBB",		//sub two values on stack, push rflags
	"SUBW",
	"SUBD",
	"SUBQ",

	"MULB",		//mul two values on stack, push rflags
	"MULW",
	"MULD",
	"MULQ",

	"DIVB",		//div two values on stack, push rflags
	"DIVW",
	"DIVD",
	"DIVQ",

	"ANDB",		//and two values on stack
	"ANDW",
	"ANDD",
	"ANDQ",

	"ORB",		//or two values on stack
	"ORW",
	"ORD",
	"ORQ",

	"XORB",		//xor two values on stack
	"XORW",
	"XORD",
	"XORQ",

	"SHLB",		//shift left
	"SHLW",
	"SHLD",
	"SHLQ",

	"SHRB",		//shift right
	"SHRW",
	"SHRD",
	"SHRQ",

	"CMPB",		//compares two values on stack, pushes flags
	"CMPW",
	"CMPD",
	"CMPQ",

	"JMP",		//adds 8 byte value on top of stack to vip
	"JMPI",
	"JL",			//conditional jumps pop rflags from stack then
	"JLE",		//	add the next 8 bytes on stack to vip based on rflags 
	"JG",
	"JGE",
	"JZ",
	"JNZ",


	"CALL",		//saves IP+1, does IP+(8 bytes on top of stack), pushes IP+1 
				//convention will be stdcall so arguments passed left to right on stack
				//return value is also just pushed onto the stack at the end
	"RET",		//exchanges the current top value and value below it(the return address)
				//then sets IP to the return address

	"X86CALL",	//convention:
				//prior to call push all arguments u want to pass onto the stack right to left
				//AT LEAST 4 ARGUMENTS MUST BE PUSHED ONTO STACK(THEY CAN BE ZERO IF U DONT WANT TO PASS ANYTHING)
				//this means arg1 is at the bottom of the stack
				//then push the address u want to call onto the stack
				//the address u want to call is popped off temporarily, as are the first 4 arguments
				//the arguments are put into their respective x86 registers(rcx,rdx,r8,r9)
				//then the address is called

	"EDITSP",	//one byte for the op code, a signed short to represent the stack change

	"STSP",
	"LDSP",
	"STIP",
	"LDIP",

	"SWAPB",
	"SWAPW",
	"SWAPD",
	"SWAPQ",

	"DECB",
	"DECW",
	"DECD",
	"DECQ",
	"INCB",
	"INCW",
	"INCD",
	"INCQ",

	"SWAPBQ",
	"SWAPWQ",
	"SWAPDQ",

	"LDSB",			//LOAD AT STACK OFFSET <SIZE>
	"LDSW",
	"LDSD",
	"LDSQ",

	"STSB",			//STORE AT STACK OFFSET <SIZE>
	"STSW",
	"STSD",
	"STSQ",

	"LDBPB",
	"LDBPW",
	"LDBPD",
	"LDBPQ",

	"STBPB",
	"STBPW",
	"STBPD",
	"STBPQ",
};

INLINE CONST CHAR* RegisterBindings[]{
	"BP",
	"R1",
	"R2",
	"R3",
	"R4",
	"R5",
	"R6",
	"R7",
	"R8",
	"R9",
	"R10",
	"R11",
	"R12",
	"R13",
	"R14",
	"R15",
	"R16",
	"R17",
	"R18",
	"R19",
	"R20",
	"R21",
	"R22",
	"R23",
	"R24",
	"R25",
	"R26",
	"R27",
	"R28",
	"R29",
	"R30",
	"R31",
};


//good paste
VECTOR<STRING> VmTokenizeLine(STRING CONST& Str, STRING CONST& Div);
BOOL VmGetInstructionId(STRING CONST& Instruction, PUCHAR OpCode);
BOOL VmGetRegisterId(STRING CONST& Register, PUCHAR Id);
INT VmGetInstructionLength(UCHAR InstructionId);
VOID VmLoadDefaultInstructions(PVM_ASSEMBLER Assembler);
INT VmGetInverseStackChange(PCODE_LINK Link);
INT VmGetInverseStackChangeForRange(PVM_ASSEMBLER Assembler, PCODE_LINK Start, INT Count);
BOOL VmAddOpaqueBranch(PVM_ASSEMBLER Assembler, PCODE_LINK Start, INT InstructionsToInclude, INT JunkCount);
BOOL VmAddInstruction(PVM_ASSEMBLER Assembler, VECTOR<STRING> CONST& Tokens);
BOOL VmLoadFile(PVM_ASSEMBLER Assembler, STRING CONST& Filepath);
VOID VmAppendJumpsToUsed(PVM_ASSEMBLER Assembler);
VOID VmShuffleUsedHandlerOder(PVM_ASSEMBLER Assembler);
VOID VmFoldTable(PVM_ASSEMBLER Assembler);
VOID VmGenerateRandomOpCodes(PVM_ASSEMBLER Assembler);
ULONG VmGetCodeSize(PVM_ASSEMBLER Assembler);
ULONG VmGetVmSize(PVM_ASSEMBLER Assembler);
BOOL VmGetJumpDelta(PCODE_LINK Jmp);
PVOID VmAssembleCodeToBuffer(PVM_ASSEMBLER Assembler, PULONG Size);
PVOID VmAssembleVmToBuffer(PVM_ASSEMBLER Assembler, PULONG Size);
VOID VmBuildHandlerTable(PVM_ASSEMBLER Assembler, PULONG Table);
PVOID VmBuildSymbolTable(PVM_ASSEMBLER Assembler, PULONG TotalTableSize);
UCHAR VmGetVmEnterOpCode(PVM_ASSEMBLER Assembler);

#endif