

#include <Windows.h>

#include "VmAssembler.h"
#include "VmSdk.h"
#include "VmSectionAppender.h"

#define test_size 0x1423

INT main()
{
	VM_ASSEMBLER Asm;
	VmLoadFile(&Asm, "C:\\Users\\Iizerd\\source\\repos\\StackMachine\\Code\\Example.txt");

	PVOID CodeBlock;
	ULONG CodeSize;

	PVOID VmBlock;
	ULONG VmSize;

	PVM_SYMBOL SymbolTable;
	ULONG SymbolTableSize;

	VM_HEADER VmHeader;
	VmHeader.Magic = 'kctS';
	VmHeader.UnpackerSize = 0;

	std::cout << "Stack change is :" << VmGetInverseStackChangeForRange(&Asm, Asm.CodeStart->Next, 17) << '\n';

	VmLoadDefaultInstructions(&Asm);
	VmGenerateRandomOpCodes(&Asm);
	VmAppendJumpsToUsed(&Asm);
	VmShuffleUsedHandlerOder(&Asm);

	VmBlock = VmAssembleVmToBuffer(&Asm, &VmSize);
	CodeBlock = VmAssembleCodeToBuffer(&Asm, &CodeSize);

	VmBuildHandlerTable(&Asm, VmHeader.HandlerTable);
	SymbolTable = (PVM_SYMBOL)VmBuildSymbolTable(&Asm, &SymbolTableSize);

	VmHeader.SymbolTableSize = SymbolTableSize / sizeof(VM_SYMBOL);
	VmHeader.VmEnterOpCode = VmGetVmEnterOpCode(&Asm);

	std::cout << "\n VmEnter is:\n";
	for (INT i = 0; i < Asm.Instructions[0].OpCodes.size(); i++)
	{
		std::cout << i << ' ' << (INT)Asm.Instructions[0].OpCodes[i] << '\n';
	}
	std::cout << '\n';


	PVOID meme = malloc(sizeof(VM_HEADER) + SymbolTableSize);
	memcpy(meme, &VmHeader, sizeof(VM_HEADER));
	memcpy((PVOID)((ULONG64)meme + sizeof(VM_HEADER)), SymbolTable, SymbolTableSize);
	ULONG64 Name = 0;
	CONST CHAR* VMD = ".vmd";
	memcpy(&Name, VMD, 4);
	AddSection("C:\\$Fanta\\VirtualMachine\\x64\\Release\\Test.exe", Name, sizeof(VM_HEADER) + SymbolTableSize, IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE, meme);
	CONST CHAR* VMC = ".vmc";
	memcpy(&Name, VMC, 4);
	AddSection("C:\\$Fanta\\VirtualMachine\\x64\\Release\\Test.exe", Name, CodeSize, IMAGE_SCN_MEM_READ, CodeBlock);
	CONST CHAR* VMH = ".vmh";
	memcpy(&Name, VMH, 4);
	AddSection("C:\\$Fanta\\VirtualMachine\\x64\\Release\\Test.exe", Name, VmSize, IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE, VmBlock);

	system("pause");
	return 1;
}