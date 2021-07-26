#include "VmAssembler.h"

VECTOR<STRING> VmTokenizeLine(STRING CONST& Str, STRING CONST& Div)
{
	VECTOR<STRING> Values;
	size_t PrevPos = 0, CurPos = 0;
	do
	{
		CurPos = Str.find(Div, PrevPos);
		if (CurPos == std::string::npos)
			CurPos = Str.length();
		std::string token = Str.substr(PrevPos, CurPos - PrevPos);
		if (!token.empty())
			Values.push_back(token);
		PrevPos = CurPos + Div.length();
	} while (CurPos < Str.length() && PrevPos < Str.length());
	return Values;
}
BOOL VmGetInstructionId(STRING CONST& Instruction, PUCHAR OpCode)
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
BOOL VmGetRegisterId(STRING CONST& Register, PUCHAR Id)
{
	for (INT i = 0; i < ARRAYSIZE(RegisterBindings); i++)
	{
		if (Register == RegisterBindings[i])
		{
			*Id = i;
			return TRUE;
		}
	}
	return FALSE;
}
INT VmGetInstructionLength(UCHAR InstructionId)
{
	switch (InstructionId)
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
VOID VmLoadDefaultInstructions(PVM_ASSEMBLER Assembler)
{
	PVOID HandlerTable[256];
	{
		HandlerTable[VMENTER] = VmEnter;
		HandlerTable[VMEXIT] = ViVmExit;
		HandlerTable[NOP] = ViNop;

		HandlerTable[LDIB] = ViLdib;
		HandlerTable[LDIW] = ViLdiw;
		HandlerTable[LDID] = ViLdid;
		HandlerTable[LDIQ] = ViLdiq;

		HandlerTable[LDIBSXW] = ViLdibsxw;
		HandlerTable[LDIBSXD] = ViLdibsxd;
		HandlerTable[LDIBSXQ] = ViLdibsxq;
		HandlerTable[LDIWSXD] = ViLdiwsxd;
		HandlerTable[LDIWSXQ] = ViLdiwsxq;
		HandlerTable[LDIDSXQ] = ViLdidsxq;

		HandlerTable[LDIBZXW] = ViLdibzxw;
		HandlerTable[LDIBZXD] = ViLdibzxd;
		HandlerTable[LDIBZXQ] = ViLdibzxq;
		HandlerTable[LDIWZXD] = ViLdiwzxd;
		HandlerTable[LDIWZXQ] = ViLdiwzxq;
		HandlerTable[LDIDZXQ] = ViLdidzxq;

		HandlerTable[LDAB] = ViLdab;
		HandlerTable[LDAW] = ViLdaw;
		HandlerTable[LDAD] = ViLdad;
		HandlerTable[LDAQ] = ViLdaq;

		HandlerTable[STAB] = ViStab;
		HandlerTable[STAW] = ViStaw;
		HandlerTable[STAD] = ViStad;
		HandlerTable[STAQ] = ViStaq;

		HandlerTable[SXBW] = ViSxbw;
		HandlerTable[SXBD] = ViSxbd;
		HandlerTable[SXBQ] = ViSxbq;
		HandlerTable[SXWD] = ViSxwd;
		HandlerTable[SXWQ] = ViSxwq;
		HandlerTable[SXDQ] = ViSxdq;

		HandlerTable[ZXBW] = ViZxbw;
		HandlerTable[ZXBD] = ViZxbd;
		HandlerTable[ZXBQ] = ViZxbq;
		HandlerTable[ZXWD] = ViZxwd;
		HandlerTable[ZXWQ] = ViZxwq;
		HandlerTable[ZXDQ] = ViZxdq;

		HandlerTable[PUSHB] = ViPushb;
		HandlerTable[PUSHW] = ViPushw;
		HandlerTable[PUSHD] = ViPushd;
		HandlerTable[PUSHQ] = ViPushq;

		HandlerTable[POPB] = ViPopb;
		HandlerTable[POPW] = ViPopw;
		HandlerTable[POPD] = ViPopd;
		HandlerTable[POPQ] = ViPopq;

		HandlerTable[NPOPB] = ViNpopb;
		HandlerTable[NPOPW] = ViNpopw;
		HandlerTable[NPOPD] = ViNpopd;
		HandlerTable[NPOPQ] = ViNpopq;

		HandlerTable[ADDB] = ViAddb;
		HandlerTable[ADDW] = ViAddw;
		HandlerTable[ADDD] = ViAddd;
		HandlerTable[ADDQ] = ViAddq;

		HandlerTable[SUBB] = ViSubb;
		HandlerTable[SUBW] = ViSubw;
		HandlerTable[SUBD] = ViSubd;
		HandlerTable[SUBQ] = ViSubq;

		HandlerTable[MULB] = ViMulb;
		HandlerTable[MULW] = ViMulw;
		HandlerTable[MULD] = ViMuld;
		HandlerTable[MULQ] = ViMulq;

		HandlerTable[DIVB] = ViDivb;
		HandlerTable[DIVW] = ViDivw;
		HandlerTable[DIVD] = ViDivd;
		HandlerTable[DIVQ] = ViDivq;

		HandlerTable[ANDB] = ViAndb;
		HandlerTable[ANDW] = ViAndw;
		HandlerTable[ANDD] = ViAndd;
		HandlerTable[ANDQ] = ViAndq;

		HandlerTable[ORB] = ViOrb;
		HandlerTable[ORW] = ViOrw;
		HandlerTable[ORD] = ViOrd;
		HandlerTable[ORQ] = ViOrq;

		HandlerTable[XORB] = ViXorb;
		HandlerTable[XORW] = ViXorw;
		HandlerTable[XORD] = ViXord;
		HandlerTable[XORQ] = ViXorq;

		HandlerTable[SHLB] = ViShlb;
		HandlerTable[SHLW] = ViShlw;
		HandlerTable[SHLD] = ViShld;
		HandlerTable[SHLQ] = ViShlq;

		HandlerTable[SHRB] = ViShrb;
		HandlerTable[SHRW] = ViShrw;
		HandlerTable[SHRD] = ViShrd;
		HandlerTable[SHRQ] = ViShrq;

		HandlerTable[CMPB] = ViCmpb;
		HandlerTable[CMPW] = ViCmpw;
		HandlerTable[CMPD] = ViCmpd;
		HandlerTable[CMPQ] = ViCmpq;

		HandlerTable[JMP] = ViJmp;
		HandlerTable[JMPI] = ViJmpi;
		HandlerTable[JL] = ViJl;
		HandlerTable[JLE] = ViJle;
		HandlerTable[JG] = ViJg;
		HandlerTable[JGE] = ViJge;
		HandlerTable[JZ] = ViJz;
		HandlerTable[JNZ] = ViJnz;

		HandlerTable[CALL] = ViCall;
		HandlerTable[RET] = ViRet;
		HandlerTable[X86CALL] = ViX86Call;

		HandlerTable[EDITSP] = ViEditsp;

		HandlerTable[STSP] = ViStsp;
		HandlerTable[LDSP] = ViLdsp;
		HandlerTable[STIP] = ViStip;
		HandlerTable[LDIP] = ViLdip;

		HandlerTable[SWAPB] = ViSwapb;
		HandlerTable[SWAPW] = ViSwapw;
		HandlerTable[SWAPD] = ViSwapd;
		HandlerTable[SWAPQ] = ViSwapq;

		HandlerTable[DECB] = ViDecb;
		HandlerTable[DECW] = ViDecw;
		HandlerTable[DECD] = ViDecd;
		HandlerTable[DECQ] = ViDecq;

		HandlerTable[INCB] = ViIncb;
		HandlerTable[INCW] = ViIncw;
		HandlerTable[INCD] = ViIncd;
		HandlerTable[INCQ] = ViIncq;

		HandlerTable[SWAPBQ] = ViSwapbq;
		HandlerTable[SWAPWQ] = ViSwapwq;
		HandlerTable[SWAPDQ] = ViSwapdq;

		HandlerTable[LDSB] = ViLdsb;
		HandlerTable[LDSW] = ViLdsw;
		HandlerTable[LDSD] = ViLdsd;
		HandlerTable[LDSQ] = ViLdsq;

		HandlerTable[STSB] = ViStsb;
		HandlerTable[STSW] = ViStsw;
		HandlerTable[STSD] = ViStsd;
		HandlerTable[STSQ] = ViStsq;

		HandlerTable[LDBPB] = ViLdbpb;
		HandlerTable[LDBPW] = ViLdbpw;
		HandlerTable[LDBPD] = ViLdbpd;
		HandlerTable[LDBPQ] = ViLdbpq;

		HandlerTable[STBPB] = ViStbpb;
		HandlerTable[STBPW] = ViStbpw;
		HandlerTable[STBPD] = ViStbpd;
		HandlerTable[STBPQ] = ViStbpq;
	}

	for (INT i = NOP; i < MAX_INSTRUCTION_ID; i++)
	{
		if (i == JL || i == JLE || i == JG || i == JGE || i == JZ || i == JNZ)
		{
			Assembler->Instructions[i].Code = malloc(JCC_INSTRUCTION_LENGTH);
			memcpy(Assembler->Instructions[i].Code, HandlerTable[i], JCC_INSTRUCTION_LENGTH);
			Assembler->Instructions[i].Size = JCC_INSTRUCTION_LENGTH;
		}
		else
		{
			ULONG Size = 0;
			while (*(PVOID*)((PUCHAR)HandlerTable[i] + Size) != (PVOID)INSTRUCTION_JMP_NUMBER_R)
				++Size;
			Assembler->Instructions[i].Code = malloc(Size);
			memcpy(Assembler->Instructions[i].Code, HandlerTable[i], Size);
			Assembler->Instructions[i].Size = Size;
		}
	}

	//SETUP VMEXIT
	Assembler->Instructions[VMEXIT].Code = malloc(4);
	Assembler->Instructions[VMEXIT].Size = 4;
	Assembler->Instructions[VMEXIT].IsUsed = TRUE;
	*((PULONG)Assembler->Instructions[VMEXIT].Code) = 0xC3218B4D;

	//SETUP VMENTER
	UCHAR VmEnterCode[] = { 0x4D, 0x89, 0x21 };
	Assembler->Instructions[VMENTER].Code = malloc(sizeof(VmEnterCode));
	memcpy(Assembler->Instructions[VMENTER].Code, VmEnterCode, sizeof(VmEnterCode));
	Assembler->Instructions[VMENTER].Size = sizeof(VmEnterCode);
	Assembler->Instructions[VMENTER].IsUsed = TRUE;

}
INT VmGetInverseStackChange(PCODE_LINK Link)
{
	switch (Link->InstructionId)
	{
	case LDIB:
	case LDAB:
	case SXBW:
	case ZXBW:
	case PUSHB:
	case LDSB:
	case LDBPB:
		return 1;
	case LDIW:
	case LDIBSXW:
	case LDIBZXW:
	case LDAW:
	case SXWD:
	case ZXWD:
	case PUSHW:
	case LDSW:
	case LDBPW:
		return 2;
	case SXBD:
	case ZXBD:
		return 3;
	case LDID:
	case LDIBSXD:
	case LDIWSXD:
	case LDIBZXD:
	case LDIWZXD:
	case LDAD:
	case SXDQ:
	case ZXDQ:
	case PUSHD:
	case ADDD:
	case SUBD:
	case ANDD:
	case ORD:
	case XORD:
	case LDSD:
	case LDBPD:
		return 4;
	case SXWQ:
	case ZXWQ:
	case ADDW:
	case SUBW:
	case ANDW:
	case ORW:
	case XORW:
		return 6;
	case SXBQ:
	case ZXBQ:
	case ADDB:
	case SUBB:
	case ANDB:
	case ORB:
	case XORB:
	case SHLB:
	case SHLW:
	case SHLD:
	case SHLQ:
	case SHRB:
	case SHRW:
	case SHRD:
	case SHRQ:
		return 7;
	case LDIQ:
	case LDIBSXQ:
	case LDIWSXQ:
	case LDIDSXQ:
	case LDIBZXQ:
	case LDIWZXQ:
	case LDIDZXQ:
	case LDAQ:
	case PUSHQ:
	case CMPB:
	case CMPW:
	case CMPD:
	case CMPQ:
	case JMP:
	case CALL:
	case DECB:
	case DECW:
	case DECD:
	case DECQ:
	case INCB:
	case INCW:
	case INCD:
	case INCQ:
	case LDSQ:
	case LDBPQ:
		return 8;

	case STAB:
	case POPB:
	case NPOPB:
	case STSB:
	case STBPB:
		return -1;
	case STAW:
	case POPW:
	case NPOPW:
	case STSW:
	case STBPW:
		return -2;
	case STAD:
	case POPD:
	case NPOPD:
	case STSD:
	case STBPD:
		return -4;
	case STAQ:
	case POPQ:
	case NPOPQ:
	case RET:
	case STSQ:
	case STBPQ:
	case JL:
	case JLE:
	case JG:
	case JGE:
	case JZ:
	case JNZ:
		return -8;
	case EDITSP:
		return ((SHORT)Link->Data) * -1;
	default:
		return 0;
	}
}
INT VmGetInverseStackChangeForRange(PVM_ASSEMBLER Assembler, PCODE_LINK Start, INT Count)
{
	INT StackChange = 0;
	INT i = 0;
	for (PCODE_LINK Link = Start; Link && (i < Count); Link = Link->Next)
	{
		if (!Link->IsLabel)
		{
			StackChange += VmGetInverseStackChange(Link);
			++i;
		}
	}
	return StackChange;
}
BOOL VmAddOpaqueBranch(PVM_ASSEMBLER Assembler, PCODE_LINK Start, INT InstructionsToInclude, INT JunkCount)
{
	PCODE_LINK Take = NULL, NotTake = NULL;


	return FALSE;
}
BOOL VmAddInstruction(PVM_ASSEMBLER Assembler, VECTOR<STRING> CONST& Tokens)
{
	PCODE_LINK Link = new CODE_LINK;
	if (Tokens.size() == 1 && Tokens[0][Tokens[0].length() - 1] == ':')
	{
		Link->IsLabel = TRUE;
		Link->Name = Tokens[0];
		Link->Name.pop_back();
	}
	else
	{
		UCHAR Iid;
		if (!VmGetInstructionId(Tokens[0], &Iid))
		{
			delete Link;
			return FALSE;
		}
		Assembler->Instructions[Iid].IsUsed = TRUE;
		Link->IsLabel = FALSE;
		Link->InstructionId = Iid;

		switch (Iid)
		{
		case LDIB:
		case LDIBSXW:
		case LDIBSXD:
		case LDIBSXQ:
		case LDIBZXW:
		case LDIBZXD:
		case LDIBZXQ:
		{
			Link->Data = (PVOID)(CHAR)std::stoi(Tokens[1]);
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
			Link->Data = (PVOID)(SHORT)std::stoi(Tokens[1]);
			break;
		}
		case LDID:
		case LDIDSXQ:
		case LDIDZXQ:
		{
			Link->Data = (PVOID)(INT)std::stoi(Tokens[1]);
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
			Link->Data = (PVOID)(LONG64)std::stoi(Tokens[1]);
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
			if (!VmGetRegisterId(Tokens[1], (PUCHAR)&Link->Data))
			{
				delete Link;
				return FALSE;
			}
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
			Link->Name = Tokens[1];
			break;
		}
		default:
			break;
		}
	}

	Link->Prev = Assembler->CodeEnd;
	Link->Next = NULL;
	Assembler->CodeEnd->Next = Link;
	Assembler->CodeEnd = Link;
	return TRUE;
}
BOOL VmLoadFile(PVM_ASSEMBLER Assembler, STRING CONST& Filepath)
{
	for (INT i = 0; i < 256; i++)
		Assembler->Instructions[i].IsUsed = FALSE;

	std::ifstream InputFile(Filepath);
	if (InputFile.fail())
		return FALSE;

	Assembler->CodeStart = new CODE_LINK;
	Assembler->CodeEnd = Assembler->CodeStart;
	Assembler->CodeStart->Data = (PVOID)0xDEADBEEF00000000;
	Assembler->CodeStart->Name = "THIS IS THE START XDDDDDD!!!!";
	Assembler->CodeStart->Prev = Assembler->CodeStart->Next = NULL;

	for (STRING Line; std::getline(InputFile, Line); )
	{
		if (Line.length() < 1)
			continue;

		ULONG i;
		for (i = 0; i < Line.length(); i++) if (Line[i] == ';') break; //REMOVE COMMENTS
		while (Line.length() > i) Line.pop_back();
		while (!Line.empty() && (Line[Line.length() - 1] == (CHAR)9 || Line[Line.length() - 1] == ' ')) Line.pop_back(); //remove whitespace
		while (!Line.empty() && !std::isalpha(Line[0])) Line.erase(Line.begin());

		if (Line.length() < 1)
			continue;

		std::transform(Line.begin(), Line.end(), Line.begin(), ::toupper);

		VECTOR<STRING> Tokens = VmTokenizeLine(Line, " ");

		if (Tokens.size() < 1)
			continue;

		if (!VmAddInstruction(Assembler, Tokens))
		{
			InputFile.close();
			return FALSE;
		}
	}

	InputFile.close();
	return TRUE;
}
VOID VmAppendJumpsToUsed(PVM_ASSEMBLER Assembler)
{
	for (PVM_INSTRUCTION Instruction : Assembler->UsedInstructions)
	{
		if (Instruction->Code)
		{
			PVOID T = realloc(Instruction->Code, Instruction->Size + 8);
			if (!T)
			{
				printf("Ur out of memory son.\n");
				return;
			}
			Instruction->Code = T;
			*(PVOID*)((PUCHAR)Instruction->Code + Instruction->Size) = (PVOID)INSTRUCTION_JMP_NUMBER_R;
			Instruction->Size = Instruction->Size + 8;
		}
	}
}
VOID VmShuffleUsedHandlerOder(PVM_ASSEMBLER Assembler)
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(Assembler->UsedInstructions.begin(), Assembler->UsedInstructions.end(), g);
}
VOID VmFoldTable(PVM_ASSEMBLER Assembler)
{
	INT Increment = 256 / Assembler->UsedInstructions.size();
	for (INT i = 0; i < Increment; i++)
	{
		INT FoldBase = Assembler->UsedInstructions.size() * i;
		VmShuffleUsedHandlerOder(Assembler);
		for (INT j = 0; j < Assembler->UsedInstructions.size(); j++)
		{
			Assembler->UsedInstructions[j]->OpCodes.push_back(j + FoldBase);
		}
	}
}
VOID VmGenerateRandomOpCodes(PVM_ASSEMBLER Assembler)
{
	Assembler->UsedInstructions.clear();
	for (INT i = 0; i < 256; i++)
	{
		if (Assembler->Instructions[i].IsUsed)
		{
			Assembler->UsedInstructions.push_back(&Assembler->Instructions[i]);
		}
	}
	VmFoldTable(Assembler);
}
ULONG VmGetCodeSize(PVM_ASSEMBLER Assembler)
{
	ULONG Size = 0;
	for (PCODE_LINK Link = Assembler->CodeStart->Next; Link; Link = Link->Next)
	{
		if (!Link->IsLabel)
			Size += VmGetInstructionLength(Link->InstructionId);
	}
	return Size;
}
ULONG VmGetVmSize(PVM_ASSEMBLER Assembler)
{
	ULONG Size = 0;
	for (PVM_INSTRUCTION Inst : Assembler->UsedInstructions)
	{
		Size += Inst->Size;
	}
	return Size;
}
BOOL VmGetJumpDelta(PCODE_LINK Jmp)
{
	//FIRST ITERATE AND CHECK BEHIND THE JMP
	PCODE_LINK Temp;
	LONGLONG Delta = 0;
	for (Temp = Jmp; Temp && Temp->Prev; Temp = Temp->Prev)
	{
		if (Temp->Prev->IsLabel && Temp->Prev->Name == Jmp->Name)
		{
			Jmp->Data = (PVOID)Delta;
			return TRUE;
		}
		Delta -= VmGetInstructionLength(Temp->Prev->InstructionId);
	}

	//NOW LOOK IN FRONT
	Delta = VmGetInstructionLength(Jmp->InstructionId); //9
	for (Temp = Jmp; Temp && Temp->Next; Temp = Temp->Next)
	{
		if (Temp->Next->IsLabel && Temp->Next->Name == Jmp->Name)
		{
			Jmp->Data = (PVOID)Delta;
			return TRUE;
		}
		Delta += VmGetInstructionLength(Temp->Next->InstructionId);
	}

	return FALSE;
}
PVOID VmAssembleCodeToBuffer(PVM_ASSEMBLER Assembler, PULONG Size)
{
	*Size = VmGetCodeSize(Assembler);
	PVOID BuffBase = malloc(*Size);
	if (!BuffBase)
		return NULL;

	PUCHAR BuffOff = (PUCHAR)BuffBase;

	for (PCODE_LINK Link = Assembler->CodeStart->Next; Link; Link = Link->Next)
	{
		if (!Link->IsLabel)
		{
			UCHAR OpCode = 0;
			OpCode = Assembler->Instructions[Link->InstructionId].OpCodes[rand() % Assembler->Instructions[Link->InstructionId].OpCodes.size()]; //rand() % Assembler->Instructions[Link->InstructionId].OpCodes.size()

			switch (Link->InstructionId)
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
				T.v1 = OpCode;
				T.v2 = (CHAR)Link->Data;
				memcpy(BuffOff, &T, sizeof(T));
				BuffOff += sizeof(T);
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
				T.v1 = OpCode;
				T.v2 = (SHORT)Link->Data;
				memcpy(BuffOff, &T, sizeof(T));
				BuffOff += sizeof(T);
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
				T.v1 = OpCode;
				T.v2 = (INT)Link->Data;
				memcpy(BuffOff, &T, sizeof(T));
				BuffOff += sizeof(T);
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
				T.v1 = OpCode;
				T.v2 = (LONG64)Link->Data;
				memcpy(BuffOff, &T, sizeof(T));
				BuffOff += sizeof(T);
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
				T.v1 = OpCode;
				T.v2 = (UCHAR)Link->Data;
				memcpy(BuffOff, &T, sizeof(T));
				BuffOff += sizeof(T);
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

				if (!VmGetJumpDelta(Link))
				{
					system("pause");
					free(BuffBase);
				}
				T.v1 = OpCode;
				T.v2 = (LONG64)Link->Data;
				memcpy(BuffOff, &T, sizeof(T));
				BuffOff += sizeof(T);

				break;
			}
			default:
				*(PUCHAR)BuffOff = OpCode;
				BuffOff++;
			}

		}
	}

	return BuffBase;
}
PVOID VmAssembleVmToBuffer(PVM_ASSEMBLER Assembler, PULONG Size)
{
	*Size = VmGetVmSize(Assembler);
	PVOID VmBuffBase = malloc(*Size);
	if (!VmBuffBase)
		return NULL;

	PUCHAR VmBuffOff = (PUCHAR)VmBuffBase;
	for (PVM_INSTRUCTION Instruction : Assembler->UsedInstructions)
	{
		memcpy(VmBuffOff, Instruction->Code, Instruction->Size);
		VmBuffOff += Instruction->Size;
	}

	return VmBuffBase;
}
VOID VmBuildHandlerTable(PVM_ASSEMBLER Assembler, PULONG Table)
{
	ULONG RunningOffset = 0;

	for (PVM_INSTRUCTION Instruction : Assembler->UsedInstructions)
	{
		for (UCHAR OpCode : Instruction->OpCodes)
		{
			Table[OpCode] = RunningOffset;
		}
		RunningOffset += Instruction->Size;
	}
}
PVOID VmBuildSymbolTable(PVM_ASSEMBLER Assembler, PULONG TotalTableSize)
{
	VECTOR<VM_SYMBOL> SymbolTable;
	ULONG RunningOffset = 0;
	for (PCODE_LINK Link = Assembler->CodeStart->Next; Link; Link = Link->Next)
	{
		if (Link->IsLabel)
		{
			SymbolTable.emplace_back();
			memset(&SymbolTable[SymbolTable.size() - 1], 0, sizeof(VM_SYMBOL));
			INT CopyLength = min(Link->Name.length(), VM_SYMBOL_NAME_LENGTH);
			for (INT i = 0; i < CopyLength; i++)
				SymbolTable[SymbolTable.size() - 1].Name[i] = Link->Name[i];

			SymbolTable[SymbolTable.size() - 1].Offset = RunningOffset;
		}
		else
		{
			RunningOffset += VmGetInstructionLength(Link->InstructionId);
		}
	}
	*TotalTableSize = SymbolTable.size() * sizeof(VM_SYMBOL);
	PVM_SYMBOL RetTable = (PVM_SYMBOL)malloc(*TotalTableSize);
	memcpy(RetTable, SymbolTable.data(), *TotalTableSize);
	return RetTable;
}
UCHAR VmGetVmEnterOpCode(PVM_ASSEMBLER Assembler)
{
	return Assembler->Instructions[0].OpCodes[rand() % Assembler->Instructions[0].OpCodes.size()];
}
