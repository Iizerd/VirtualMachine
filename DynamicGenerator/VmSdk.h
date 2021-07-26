#ifndef _VMSDK_H
#define _VMSDK_H

#include "VmDefs.h"


BOOL VMSDKInit(PVM_DATA VmData, PVOID ModuleBase);
PVOID VMSDKGetSymbol(PVM_DATA VmData, CONST CHAR* SymbolName);
PVM_HANDLER_TABLE VMSDKBuildHandlerTable(PVM_DATA VmData);
VOID VMSDKBuildHandlerTableInPlace(PVM_DATA VmData, PVM_HANDLER_TABLE Table);
VOID VMSDKEnterAtSymbol(PVM_DATA VmData, PVM_CONTEXT Context, PVOID Symbol, PVM_HANDLER_TABLE HandlerTable, PVM_AUX_STORAGE Storage);

#endif