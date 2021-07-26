#ifndef _MVEC_H
#define _MVEC_H

#include <Windows.h>
#include <windef.h>

typedef struct _MVEC
{
	INT Size;
	INT Capacity;
	INT ItemSize;
	PUCHAR Base;
	PUCHAR Offset;
}MVEC, *PMVEC;

typedef BOOL(*FnMVecCompare)(PMVEC Vec, PVOID First, PVOID Second);

VOID MvInitializeVector(PMVEC Vec, INT InitialSize, INT ItemSize)
{
	Vec->ItemSize = ItemSize;
	Vec->Capacity = InitialSize;
	Vec->Size = 0;

	Vec->Base = (PUCHAR)malloc(InitialSize * ItemSize);
	Vec->Offset = Vec->Base;
}

VOID MvDestroyVector(PMVEC Vec)
{
	free(Vec->Base);
}

VOID MvGrow(PMVEC Vec, INT Multiple = 2)
{
	Vec->Capacity *= Multiple;
	PUCHAR NewVec = (PUCHAR)malloc(Vec->Capacity);
	memcpy(NewVec, Vec->Base, Vec->Size * Vec->ItemSize);
	free(Vec->Base);
	Vec->Base = NewVec;
	Vec->Offset = Vec->Base + Vec->Size;
}

VOID MvPush(PMVEC Vec, PVOID Item)
{
	if (Vec->Size >= Vec->Capacity)
		MvGrow(Vec);
	memcpy(Vec->Offset, Item, Vec->ItemSize);
	Vec->Offset += Vec->ItemSize;
	++Vec->Size;
}

VOID MvPop(PMVEC Vec, PVOID Res = NULL)
{
	if (Vec->Size == 0)
		return;
	--Vec->Size;
	Vec->Offset -= Vec->ItemSize;
	if (Res)
		memcpy(Res, Vec->Offset, Vec->ItemSize);
}

VOID MvRemove(PMVEC Vec, INT Index, PVOID Res = NULL)
{
	if (Index >= Vec->Size)
		return;
	if (Res)
		memcpy(Res, Vec->Base + (Index * Vec->ItemSize), Vec->ItemSize);
	memcpy(Vec->Base + (Index * Vec->ItemSize), Vec->Offset - Vec->ItemSize, Vec->ItemSize);
	--Vec->Size;
	Vec->Offset -= Vec->ItemSize;
}

VOID MvSort(PMVEC Vec, FnMVecCompare Compare)
{
	//maybe do quicksort.
}

PVOID MvEmplaceBack(PMVEC Vec)
{
	if (Vec->Size >= Vec->Capacity)
		MvGrow(Vec);

	PVOID Ret = Vec->Offset;
	++Vec->Size;
	Vec->Offset += Vec->ItemSize;
}

#endif