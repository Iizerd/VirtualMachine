#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "StackMachine.h"
#include "StackMachineAssembler.h"
#include <assert.h>

#include <chrono>

//this project for static vm, ignore


void CXorStr(PVOID Arr, CHAR XorKey, ULONG ArraySize)
{
	for (int i = 0; i < ArraySize; i++)
		((PUCHAR)Arr)[i] ^= XorKey;
}

LONG64 TestFunc(LONG64 v1, LONG64 v2, LONG64 v3, LONG64 v4, LONG64 v5, LONG64 v6)
{
	printf("Got here. %i\n", v6);
	//std::cout << v1 << ' ' << v2 << ' ' << v3 << ' ' << v4 << '\n';
	return v1 + v2 + v3 + v4 + v5 + v6;
}

int main()
{
	
	SMCB Block;
	SmAllocCodeBlock(&Block, 0x10000);
	SmLoadFromFile("C:\\Users\\Iizerd\\source\\repos\\StackMachine\\Code\\Example.txt");
	SmParseLoadedFile();
	SmAssembleToBlock(&Block);
	ULONG64 Offset = 0;
	INT Size = 0;
	while (*(PUCHAR)((PUCHAR)Block.Base + Offset) != VMEXIT)
	{
		std::cout << SmDecodeInstruction(((PUCHAR)Block.Base + Offset), &Size);
		Offset += Size;
	}
	std::cout << SmDecodeInstruction(((PUCHAR)Block.Base + Offset), NULL);

	system("pause");

	VM_HANDLER_TABLE HandlerTable;
	VM_CONTEXT Context;
	VM_AUX_STORAGE Storage;

	//InitHandlerTable(&HandlerTable);
	//Context.Num[R1].Addr64 = TestFunc;

	//printf("%llu is the meme.\n", Context.Num[R1].Addr64);
	//VmExecute(&Context, Block.Base, &HandlerTable, &Storage);
	//printf("%llu is the meme.\n", Context.Num[R1].Addr64);
	//system("pause");

	while (TRUE)
	{
		system("pause");
		
		std::string Ex = "Hello this is a test string. It is really long because I want to really get a good idea for how \
long this is actually going to take for both methods. Did you know that while my username looks like lizerd, it is actually Iizerd \
with a capitol i at the start instead of an L. I think this is cool and unique and separates me from other people :D. Now im making \
this string even longer and talking more about lizards. Did you know there are some lizards that can shoot blood from their eyes? \
when I found this out I thought it was super cool. A sick defense mechanism that allows them to scare off predators.";


		char XorKey = 'A';
		ULONG ArraySize = Ex.size();
		InitHandlerTable(&HandlerTable);
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		Context.Num[R13] = Ex.data();
		Context.Num[R12] = (PVOID)(CHAR)XorKey;
		Context.Num[R11] = (PVOID)(ULONG64)ArraySize;
		VmEnter(&Context, Block.Base, &HandlerTable, &Storage);
		Context.Num[R13] = Ex.data();
		Context.Num[R12] = (PVOID)(CHAR)XorKey;
		Context.Num[R11] = (PVOID)(ULONG64)ArraySize;
		VmEnter(&Context, Block.Base, &HandlerTable, &Storage);
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		//std::cout << "\n\n" << Ex << " Was the string.\n";
		std::cout << "My Machine took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " microseconds.\n";

		start = std::chrono::high_resolution_clock::now();
		CXorStr(Ex.data(), 'A', Ex.size());
		CXorStr(Ex.data(), 'A', Ex.size());
		end = std::chrono::high_resolution_clock::now();

		//std::cout << "\n\n" << Ex << " Was the string.\n";
		std::cout << "The C One took  " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " microseconds.\n";
	}
}

//typedef struct _CODE_BLOCK
//{
//	PVOID Base;
//	PVOID Offset;
//	ULONG Capacity;
//}CODE_BLOCK, * PCODE_BLOCK;
//
//VOID AllocCodeBlock(PCODE_BLOCK Block, ULONG Size)
//{
//	Block->Base = malloc(Size);
//	Block->Offset = Block->Base;
//	Block->Capacity = Size;
//}
//
//VOID PushBack(PCODE_BLOCK Block, PVOID Item, ULONG ItemSize)
//{
//	assert(Block->Capacity - (Block->Offset - Block->Base) > ItemSize);
//	memcpy(Block->Offset, Item, ItemSize);
//	Block->Offset = (PUCHAR)Block->Offset + ItemSize;
//}
//
//#pragma pack(push, 1)
//struct __LDIQ
//{
//	UCHAR OpCode;
//	PVOID Imm;
//};
//
//struct __LIDB
//{
//	UCHAR OpCode;
//	CHAR Imm;
//};
//
//struct __POPR
//{
//	UCHAR OpCode;
//	UCHAR RegId;
//};
//
//struct __ONEB
//{
//	UCHAR Byte;
//};
//
//struct __EDITSP
//{
//	UCHAR OpCode;
//	SHORT Change;
//};
//#pragma pack(pop)
//
//LONG64 TestFunc(LONG64 v1, LONG64 v2, LONG64 v3, LONG64 v4)
//{
//	printf("Got here.\n");
//	//std::cout << v1 << ' ' << v2 << ' ' << v3 << ' ' << v4 << '\n';
//	return v1 + v2 + v3 + v4;
//}
//
//int main()
//{
//	CODE_BLOCK Block;
//	__LDIQ LQ;
//	__LIDB LB;
//	__POPR P;
//	__ONEB O;
//	__EDITSP ESP;
//	AllocCodeBlock(&Block, 1000);
//
//	LQ.OpCode = OPCODES::LDIQ;
//	LQ.Imm = (PVOID)1;
//	PushBack(&Block, &LQ, sizeof(LQ));
//
//	LQ.OpCode = OPCODES::LDIQ;
//	LQ.Imm = (PVOID)2;
//	PushBack(&Block, &LQ, sizeof(LQ));
//
//	LQ.OpCode = OPCODES::LDIQ;
//	LQ.Imm = (PVOID)3;
//	PushBack(&Block, &LQ, sizeof(LQ));
//
//	LQ.OpCode = OPCODES::LDIQ;
//	LQ.Imm = (PVOID)4;
//	PushBack(&Block, &LQ, sizeof(LQ));
//
//	LQ.OpCode = OPCODES::LDIQ;
//	LQ.Imm = (PVOID)TestFunc;
//	PushBack(&Block, &LQ, sizeof(LQ));
//
//	O.Byte = OPCODES::X86CALL;
//	PushBack(&Block, &O, sizeof(O));
//
//	P.OpCode = OPCODES::POPQ;
//	P.RegId = R1;
//	PushBack(&Block, &P, sizeof(P));
//
//	ESP.OpCode = OPCODES::EDITSP;
//	ESP.Change = 32;
//	PushBack(&Block, &ESP, sizeof(ESP));
//
//	O.Byte = OPCODES::ENDOFCODE;
//	PushBack(&Block, &O, sizeof(O));
//
//	STACK_MACHINE Machine;
//	InitStackMachine(&Machine);
//	
//	std::cout << (LONGLONG)(Machine.RegisterFile[R1]) << " Is meme.\n";
//	//printf("%ll is the meme.\n", Machine.RegisterFile[R1]);
//	Machine.RegisterFile[IP] = Block.Base;
//	Machine.RegisterFile[R1] = (PVOID)2021;
//	std::cout << (LONGLONG)(Machine.RegisterFile[R1]) << " Is meme.\n";
//	//printf("%ll is the meme.\n", Machine.RegisterFile[R1]);
//	VmExecute(&Machine);
//	std::cout << (LONGLONG)(Machine.RegisterFile[R1]) << " Is meme.\n";
//	//printf("%ll is the meme.\n", Machine.RegisterFile[R1]);
//	system("pause");
//	return 1;
//}
