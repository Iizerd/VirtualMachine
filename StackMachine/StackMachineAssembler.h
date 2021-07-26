#ifndef _STACK_MACHINE_ASSEMVLER_H
#define _STACK_MACHINE_ASSEMVLER_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "StackMachine.h"

inline std::vector<std::string> SplitString(std::string const& Str, std::string const& Sep)
{
	std::vector<std::string> Values;
	size_t PrevPos = 0, CurPos = 0;
	do
	{
		CurPos = Str.find(Sep, PrevPos);
		if (CurPos == std::string::npos) CurPos = Str.length();
		std::string token = Str.substr(PrevPos, CurPos - PrevPos);
		if (!token.empty()) Values.push_back(token);
		PrevPos = CurPos + Sep.length();
	} while (CurPos < Str.length() && PrevPos < Str.length());
	return Values;
}

typedef struct _SYMBOL
{
	BOOL IsLabel;
	UCHAR OpCode;
	INT Offset;
	INT InstructionSize;
	std::string Line;
	std::vector<std::string> Ele;
	_SYMBOL(BOOL Il = FALSE, std::string const& L = "")
		: Offset(0), IsLabel(Il), Line(L), Ele() 
	{
		std::transform(Line.begin(), Line.end(), Line.begin(), ::toupper);
		Ele = SplitString(Line, " ");
	}
}SYMBOL, * PSYMBOL;

typedef struct _SMCB
{
	PVOID Base;
	PVOID Offset;
	ULONG Capacity;
}SMCB, *PSMCB;

VOID SmAllocCodeBlock(PSMCB Block, ULONG Size)
{
	Block->Base = malloc(Size);
	Block->Offset = Block->Base;
	Block->Capacity = Size;
}

VOID SmPushBackBlock(PSMCB Block, PVOID Item, ULONG ItemSize)
{
	memcpy(Block->Offset, Item, ItemSize);
	Block->Offset = (PUCHAR)Block->Offset + ItemSize;
}

std::vector<SYMBOL> Symbols;
std::vector<PSYMBOL> Labels;
std::vector<PSYMBOL> Instructions;

CONST CHAR* InstructionBindings[]{
	"VMENTER",
	"VMEXIT",	//sets vip to 0
	"NOP",		//where that label gets stored

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

CONST CHAR* RegisterBindings[]{
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

INT SmGetInstructionSize(UCHAR OpCode)
{
	switch (OpCode)
	{
	case VMEXIT:
	case NOP:

	case LDAB:
	case LDAW:
	case LDAD:
	case LDAQ:

	case STAB:
	case STAW:
	case STAD:
	case STAQ:

	case SXBW:
	case SXBD:
	case SXBQ:
	case SXWD:
	case SXWQ:
	case SXDQ:

	case ZXBW:
	case ZXBD:
	case ZXBQ:
	case ZXWD:
	case ZXWQ:
	case ZXDQ:

	case ADDB:
	case ADDW:
	case ADDD:
	case ADDQ:

	case SUBB:
	case SUBW:
	case SUBD:
	case SUBQ:

	case MULB:
	case MULW:
	case MULD:
	case MULQ:

	case DIVB:
	case DIVW:
	case DIVD:
	case DIVQ:

	case ANDB:
	case ANDW:
	case ANDD:
	case ANDQ:

	case ORB:
	case ORW:
	case ORD:
	case ORQ:

	case XORB:
	case XORW:
	case XORD:
	case XORQ:

	case SHLB:
	case SHLW:
	case SHLD:
	case SHLQ:

	case SHRB:
	case SHRW:
	case SHRD:
	case SHRQ:
	case CMPB:
	case CMPW:
	case CMPD:
	case CMPQ:
	case RET:
	case X86CALL:
	case SWAPB:
	case SWAPW:
	case SWAPD:
	case SWAPQ:
	case DECB:
	case DECW:
	case DECD:
	case DECQ:
	case INCB:
	case INCW:
	case INCD:
	case INCQ:
	case NPOPB:
	case NPOPW:
	case NPOPD:
	case NPOPQ:
	case SWAPBQ:
	case SWAPWQ:
	case SWAPDQ:
	case JMP:
	{
		return 1;
	}
	case LDIB:
	case PUSHB:
	case PUSHW:
	case PUSHD:
	case PUSHQ:
	case POPB:
	case POPW:
	case POPD:
	case POPQ:
	case STSP:
	case LDSP:
	case STIP:
	case LDIP:
	case LDIBSXW:
	case LDIBSXD:
	case LDIBSXQ:
	case LDIBZXW:
	case LDIBZXD:
	case LDIBZXQ:
	{
		return 2;
	}
	case LDIW:
	case EDITSP:
	case LDIWSXD:
	case LDIWSXQ:
	case LDIWZXD:
	case LDIWZXQ:
	case LDBPB:
	case LDBPW:
	case LDBPD:
	case LDBPQ:
	case STBPB:
	case STBPW:
	case STBPD:
	case STBPQ:
	{
		return 3;
	}
	case LDID: 
	case LDIDSXQ:
	case LDIDZXQ:
	{
		return 5;
	}
	case LDIQ: 
	case JMPI:
	case JL:
	case JLE:
	case JG:
	case JGE:
	case JZ:
	case JNZ:
	case CALL:
	case LDSB:
	case LDSW:
	case LDSD:
	case LDSQ:
	case STSB:
	case STSW:
	case STSD:
	case STSQ:
	{
		return 9;
	}

	}
	return 0;
}

BOOL SmGetOpCode(std::string CONST& Instruction, PUCHAR OpCode)
{
	for (INT i = 0; i < ARRAYSIZE(InstructionBindings); i++)
	{
		if (Instruction == InstructionBindings[i])
		{
			*OpCode = i;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL SmGetRegisterId(std::string CONST& Register, PUCHAR Id)
{
	for (INT i = 0; i < 32; i++)
	{
		if (Register == RegisterBindings[i])
		{
			*Id = i;
			return TRUE;
		}
	}
	return FALSE;
}

//LOLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
std::string SmPrintByteAsHex(UCHAR Byte)
{
	std::stringstream stream;
	stream << std::setw(2) << std::setfill('0') << std::hex << (ULONG)Byte;
	return stream.str();
}

VOID SmLoadFromFile(std::string CONST& File)
{
	Symbols.clear();
	std::ifstream InputFile(File);

	if (InputFile.bad())
	{
		printf("Bad File.\n");
		return;
	}
	for (std::string Line; std::getline(InputFile, Line); )
	{
		if (Line.length() < 1)
			continue;

		ULONG i;
		for (i = 0; i < Line.length(); i++) if (Line[i] == ';') break; //REMOVE COMMENTS
		while (Line.length() > i) Line.pop_back();
		while (!Line.empty() && (Line[Line.length() - 1] == (CHAR)9 || Line[Line.length() - 1] == ' ')) Line.pop_back();
		while (!Line.empty() && !std::isalpha(Line[0])) Line.erase(Line.begin());

		if (Line.length() < 1)
			continue;

		if (Line[Line.length() - 1] == ':') // IS LABEL
		{
			Line.pop_back();
			Symbols.emplace_back(TRUE, Line);
		}
		else
		{
			Symbols.emplace_back(FALSE, Line);
		}
	}
}

VOID SmParseLoadedFile()
{
	Labels.clear();
	Instructions.clear();
	INT Offset = 0;
	for (INT i = 0; i < Symbols.size(); i++)
	{
		/*std::cout << Offset << "\t" << Symbols[i].Line << '\n';
		for (std::string str : Symbols[i].Ele)
			std::cout << '\'' << str << '\'' << '\n';*/
		Symbols[i].Offset = Offset;
		if (!Symbols[i].IsLabel)
		{
			if (!Symbols[i].Ele.size() || !SmGetOpCode(Symbols[i].Ele[0], &Symbols[i].OpCode))
			{
				printf("Failed to get opcode for line %s.\n", Symbols[i].Line.c_str());
				return;
			}
			Symbols[i].InstructionSize = SmGetInstructionSize(Symbols[i].OpCode);
			Instructions.push_back(&Symbols[i]);
			Offset += Symbols[i].InstructionSize;
		}
		else
		{
			Labels.push_back(&Symbols[i]);
		}
	}
}

VOID SmAssembleToBlock(PSMCB Block)
{
	for (PSYMBOL Instruction : Instructions)
	{
		if (Instruction->InstructionSize == 1)
		{
			UCHAR OPC = Instruction->OpCode;
			SmPushBackBlock(Block, &OPC, 1);
		}
		else
		{
			switch (Instruction->OpCode)
			{
			case LDIB:
			case LDIBSXW:
			case LDIBSXD:
			case LDIBSXQ:
			case LDIBZXW:
			case LDIBZXD:
			case LDIBZXQ:
			{
#pragma pack(push, 1)
				struct
				{
					UCHAR v1;
					CHAR v2;
				}T;
#pragma pack(pop)
				T.v1 = Instruction->OpCode;
				T.v2 = std::stoi(Instruction->Ele[1]);
				SmPushBackBlock(Block, &T, 2);
				break;
			}
			case LDIW:
			case EDITSP:
			case LDIWSXD:
			case LDIWSXQ:
			case LDIWZXD:
			case LDIWZXQ:
			case LDBPB:
			case LDBPW:
			case LDBPD:
			case LDBPQ:
			case STBPB:
			case STBPW:
			case STBPD:
			case STBPQ:
			{
#pragma pack(push, 1)
				struct
				{
					UCHAR v1;
					SHORT v2;
				}T;
#pragma pack(pop)
				T.v1 = Instruction->OpCode;
				T.v2 = std::stoi(Instruction->Ele[1]);
				SmPushBackBlock(Block, &T, 3);
				break;
			}
			case LDID:
			case LDIDSXQ:
			case LDIDZXQ:
			{
#pragma pack(push, 1)
				struct
				{
					UCHAR v1;
					INT v2;
				}T;
#pragma pack(pop)
				T.v1 = Instruction->OpCode;
				T.v2 = std::stoi(Instruction->Ele[1]);
				SmPushBackBlock(Block, &T, 5);
				break;
			}
			case LDIQ:
			case LDSB:
			case LDSW:
			case LDSD:
			case LDSQ:
			case STSB:
			case STSW:
			case STSD:
			case STSQ:
			{
#pragma pack(push, 1)
				struct
				{
					UCHAR v1;
					LONG64 v2;
				}T;
#pragma pack(pop)
				T.v1 = Instruction->OpCode;
				T.v2 = std::stoll(Instruction->Ele[1]);
				SmPushBackBlock(Block, &T, 9);
				break;
			}
			case POPB:
			case POPW:
			case POPD:
			case POPQ:
			case PUSHB:
			case PUSHW:
			case PUSHD:
			case PUSHQ:
			case STSP:
			case LDSP:
			case STIP:
			case LDIP:
			{
#pragma pack(push, 1)
				struct
				{
					UCHAR v1;
					UCHAR v2;
				}T;
#pragma pack(pop)
				T.v1 = Instruction->OpCode;
				//Instruction->Ele[1].erase(0, 1);
				//std::cout << "\n\n\n\n breaking \n\'" << Instruction->Ele[1] << "\'\n\n\n\n\n";
				if (!SmGetRegisterId(Instruction->Ele[1], &T.v2)) // T.v2 = (UCHAR)std::stoi(Instruction->Ele[1]);
				{
					std::cout << "bad register \"" << Instruction->Ele[1] << "\"\n";
					return;
				}
				SmPushBackBlock(Block, &T, 2);
				break;
			}
			case JMPI:
			case JL:
			case JLE:
			case JG:
			case JGE:
			case JZ:
			case JNZ:
			case CALL:
			{
#pragma pack(push, 1)
				struct
				{
					UCHAR v1;
					LONG64 v2;
				}T;
#pragma pack(pop)
				T.v1 = Instruction->OpCode;

				for (PSYMBOL Label : Labels)
				{
					if (Label->Line == Instruction->Ele[1])
					{
						T.v2 = (LONG64)Label->Offset - (LONG64)Instruction->Offset;
						//std::cout << (LONG64)Label->Offset - (LONG64)Instruction->Offset << " IS THE VALUE\n";
						break;
					}
				}

				SmPushBackBlock(Block, &T, 9);
				break;
			}
			default:
				break;
			}
		}
	}
}

VOID SmAssembleToFile(std::string CONST& File)
{

}

std::string SmDecodeInstruction(PVOID Instruction, PINT ReturnedSize)
{
	INT InstructionSize = SmGetInstructionSize(*(PUCHAR)Instruction);
	std::string T = "";
	for (INT i = 0; i < InstructionSize; i++)
	{
		T += SmPrintByteAsHex(*(PUCHAR)((PUCHAR)Instruction + i));
		T += ' ';
	}
	for (INT i = 0; i < 9 - InstructionSize; i++)
	{
		T += "   ";
	}
	T += '\t';
	T += InstructionBindings[*(PUCHAR)Instruction];
	if (InstructionSize != 1)
	{
		T += '\t';
		switch (*(PUCHAR)Instruction)
		{
		case LDIB:
		case LDIBSXW:
		case LDIBSXD:
		case LDIBSXQ:
		case LDIBZXW:
		case LDIBZXD:
		case LDIBZXQ:
		{
			T += std::to_string((INT)(*(PCHAR)((PUCHAR)Instruction + 1)));
			break;
		}
		case LDIW:
		case EDITSP:
		case LDIWSXD:
		case LDIWSXQ:
		case LDIWZXD:
		case LDIWZXQ:
		case LDBPB:
		case LDBPW:
		case LDBPD:
		case LDBPQ:
		case STBPB:
		case STBPW:
		case STBPD:
		case STBPQ:
		{
			T += std::to_string(*(PSHORT)((PUCHAR)Instruction + 1));
			break;
		}
		case LDIDSXQ:
		case LDIDZXQ:
		case LDID:
		{
			T += std::to_string(*(PINT)((PUCHAR)Instruction + 1));
			break;
		}
		case LDIQ:
		{
			T += std::to_string(*(PLONG64)((PUCHAR)Instruction + 1));
			break;
		}
		case POPB:
		case POPW:
		case POPD:
		case POPQ:
		case PUSHB:
		case PUSHW:
		case PUSHD:
		case PUSHQ:
		case STSP:
		case LDSP:
		case STIP:
		case LDIP:
		{
			T += RegisterBindings[*(PUCHAR)((PUCHAR)Instruction + 1)];
			break;
		}
		case JMPI:
		case JL:
		case JLE:
		case JG:
		case JGE:
		case JZ:
		case JNZ:
		case CALL:
		case LDSB:
		case LDSW:
		case LDSD:
		case LDSQ:
		case STSB:
		case STSW:
		case STSD:
		case STSQ:
		{
			T += std::to_string(*(PLONG64)((PUCHAR)Instruction + 1));
			break;
		}
		default:
			break;
		}
	}

	if (ReturnedSize)
		*ReturnedSize = InstructionSize;

	T += '\n';
	return T;
}

#endif