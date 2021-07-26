#include "VmSdk.h"
#include <iostream>
#include <string>

void CXorStr(PVOID Arr, CHAR XorKey, ULONG ArraySize)
{
	for (int i = 0; i < ArraySize; i++)
		((PUCHAR)Arr)[i] ^= XorKey;
}

int main()
{
	system("pause");
	VM_DATA VmData;
	if (!VMSDKInit(&VmData, GetModuleHandle(NULL)))
	{
		printf("VMSDKInit failed.\n");
		system("pause");
		return 0;
	}
	PVOID XorArray = VMSDKGetSymbol(&VmData, "XORARRAY");
	if (!XorArray)
	{
		printf("failed to get symbol.\n");
		system("pause");
		return 0;
	}


	VM_CONTEXT Context;
	VM_AUX_STORAGE Storage;
	VM_HANDLER_TABLE HandlerTable;
	std::string TestStr = "Hello there this is a test string.";
	char XorKey = 'A';
	ULONG ArraySize = TestStr.size();

	VMSDKBuildHandlerTableInPlace(&VmData, &HandlerTable);

	system("pause");

	Context.Num[R13] = (PVOID)TestStr.data();
	Context.Num[R12] = (PVOID)(CHAR)XorKey;
	Context.Num[R11] = (PVOID)(ULONG64)ArraySize;
	VMSDKEnterAtSymbol(&VmData, &Context, XorArray, &HandlerTable, &Storage);
	std::cout << "The string after vm:   \"" << TestStr << "\"\n";
	CXorStr((PVOID)TestStr.data(), XorKey, ArraySize);
	std::cout << "The string after CXor: \"" << TestStr << "\"\n";

	system("pause");
}