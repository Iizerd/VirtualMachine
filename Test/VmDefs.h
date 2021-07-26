#ifndef _VMDEFS_H
#define _VMDEFS_H

#define INLINE inline

#include <Windows.h>

/*
* OK so layout time....
* I'm thinking 3 sections:
*	.vmd
*		-read/write: the data section for the vm
*		-contains the VM_HEADER structure which describes the .vmh and .vmc sections
*		-also contains data.... if i get to that...................(really thinking about just doing "shellcode" support xD)
*		-has the symbol table for the vm right after the header and unpacker stub
*		-has unpacker stub
*	.vmc
*		-read only memory that contains the virtual instructions/code itself
*	.vmh
*		-read/execute memory that contains the vmhandlers
*
* VM_HEADER
*	-Some magic to make sure it worked
*	-SymbolTableSize: The number of VM_SYMBOL structures that immediatly follow unpacked program after VM_HEADER in the .vmd section
*	-UnpackerSize: the size of the unpacker program(written in the vm itself)
*	-HandlerTable: Offsets from the base of the .vmh section to describe the handlers.
*/

//Every label...

#define VM_SYMBOL_NAME_LENGTH 31

#pragma pack(push, 1)
typedef struct _VM_SYMBOL
{
	CHAR	Name[VM_SYMBOL_NAME_LENGTH + 1];
	ULONG	Offset;					//relative to base of .vmc	(vm code)
}VM_SYMBOL, * PVM_SYMBOL;

typedef struct _VM_HEADER
{
	ULONG	Magic;					// = 'kctS';
	ULONG	SymbolTableSize;
	ULONG	UnpackerSize;			//Size of the stored unpacker program stored here in .vmh
	ULONG	HandlerTable[256];		//relative to base of .vmh	(vm handlers)
	UCHAR	VmEnterOpCode;
}VM_HEADER, * PVM_HEADER;
//			Unpacker Code			//BASE+sizeof(VM_HEADER)
//			Symbol Table			//BASE+sizeof(VM_HEADER)+UnpackerSize
#pragma pack(pop)


typedef struct _VM_DATA
{
	PVOID		HandlerCodeBase;
	PVOID		VmCode;
	PVM_SYMBOL	SymbolTable;
	PVM_HEADER	VmHeader;
	PVOID		Unpacker;
}VM_DATA, * PVM_DATA;

typedef union _VM_CONTEXT
{
	PVOID BP;
	PVOID Num[32];
}VM_CONTEXT, * PVM_CONTEXT;

typedef struct _VM_HANDLER_TABLE
{
	PVOID HandlerTable[256];
}VM_HANDLER_TABLE, * PVM_HANDLER_TABLE;

typedef struct _VM_AUX_STORAGE
{
	PVOID Storage[32];
}VM_AUX_STORAGE, * PVM_AUX_STORAGE;

enum REGISTER_ID : UCHAR
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

enum INSTRUCTION_ID : UCHAR
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

																							MAX_INSTRUCTION_ID,

};


#endif