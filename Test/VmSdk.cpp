#include "VmSdk.h"

BOOL VMSDKInit(PVM_DATA VmData, PVOID ModuleBase)
{
	if (!ModuleBase)
		return FALSE;

	VmData->VmHeader = NULL;
	VmData->VmCode = NULL;
	VmData->SymbolTable = NULL;
	VmData->Unpacker = NULL;
	VmData->HandlerCodeBase = NULL;

	PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)ModuleBase;
	if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return FALSE;

	PIMAGE_NT_HEADERS64 NtHeader = (PIMAGE_NT_HEADERS64)((PUCHAR)DosHeader + DosHeader->e_lfanew);
	if (NtHeader->Signature != IMAGE_NT_SIGNATURE)
		return FALSE;

	PIMAGE_SECTION_HEADER SectionHeaderTable = (PIMAGE_SECTION_HEADER)((PUCHAR)NtHeader + sizeof(IMAGE_NT_HEADERS64));
	for (WORD i = 0; i < NtHeader->FileHeader.NumberOfSections; i++)
	{
		if (0 == memcmp(SectionHeaderTable[i].Name, ".vmd", 4))
		{
			VmData->VmHeader = (PVM_HEADER)((ULONG64)ModuleBase + (ULONG64)SectionHeaderTable[i].VirtualAddress);
			if (VmData->VmHeader->Magic != 'kctS')
			{
				return FALSE;
			}
			VmData->Unpacker = (PVOID)((ULONG64)VmData->VmHeader + sizeof(VM_HEADER));
			VmData->SymbolTable = (PVM_SYMBOL)((ULONG64)VmData->VmHeader + sizeof(VM_HEADER) + VmData->VmHeader->UnpackerSize);
		}
		else if (0 == memcmp(SectionHeaderTable[i].Name, ".vmc", 4))
		{
			VmData->VmCode = (PVOID)((ULONG64)ModuleBase + (ULONG64)SectionHeaderTable[i].VirtualAddress);
		}
		else if (0 == memcmp(SectionHeaderTable[i].Name, ".vmh", 4))
		{
			if (!(SectionHeaderTable[i].Characteristics & IMAGE_SCN_MEM_EXECUTE))
				return FALSE;

			VmData->HandlerCodeBase = (PVOID)((ULONG64)ModuleBase + (ULONG64)SectionHeaderTable[i].VirtualAddress);
		}
	}

	if (VmData->VmHeader == NULL ||
		VmData->VmCode == NULL ||
		VmData->SymbolTable == NULL ||
		VmData->Unpacker == NULL ||
		VmData->HandlerCodeBase == NULL)
		return FALSE;

	return TRUE;
}
PVOID VMSDKGetSymbol(PVM_DATA VmData, CONST CHAR* SymbolName)
{
	for (INT i = 0; i < VmData->VmHeader->SymbolTableSize; i++)
	{
		if (0 == strcmp(SymbolName, VmData->SymbolTable[i].Name))
		{
			return (PVOID)((ULONG64)VmData->VmCode + (ULONG64)VmData->SymbolTable[i].Offset);
		}
	}
	return NULL;
}
PVM_HANDLER_TABLE VMSDKBuildHandlerTable(PVM_DATA VmData)
{
	PVM_HANDLER_TABLE Table = (PVM_HANDLER_TABLE)malloc(sizeof(VM_HANDLER_TABLE));
	if (!Table)
		return NULL;
	VMSDKBuildHandlerTableInPlace(VmData, Table);
	return Table;
}
VOID VMSDKBuildHandlerTableInPlace(PVM_DATA VmData, PVM_HANDLER_TABLE Table)
{
	for (INT i = 0; i < 256; i++)
	{
		Table->HandlerTable[i] = (PVOID)((ULONG64)VmData->HandlerCodeBase + (ULONG64)VmData->VmHeader->HandlerTable[i]);
	}
}
VOID VMSDKEnterAtSymbol(PVM_DATA VmData, PVM_CONTEXT Context, PVOID Symbol, PVM_HANDLER_TABLE HandlerTable, PVM_AUX_STORAGE Storage)
{
	typedef VOID(*FnVmEnter)(PVM_CONTEXT Context, PVOID Symbol, PVM_HANDLER_TABLE HandlerTable, PVM_AUX_STORAGE Storage);
	((FnVmEnter)(HandlerTable->HandlerTable[VmData->VmHeader->VmEnterOpCode]))(Context, Symbol, HandlerTable, Storage);
}
