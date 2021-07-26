#ifndef _VMFILEAPPENDER_H
#define _VMFILEAPPENDER_H

#include <Windows.h>

//good shares
//https://github.com/hMihaiDavid/addscn
#define P2ALIGNDOWN(x, align) ((x) & -(align))
#define P2ALIGNUP(x, align) (-(-((LONG64)x) & -((LONG64)align)))

//ty good shar
//https ://github.com/hMihaiDavid/addscn
inline VOID MapFile(HANDLE FileHandle, PHANDLE FileMapping, PVOID* FileView, ULONG Size)
{
	*FileMapping = CreateFileMappingA(FileHandle, NULL, PAGE_READWRITE, 0, Size, NULL);
	*FileView = MapViewOfFile(*FileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
}
inline VOID UnMapFile(HANDLE FileMapping, HANDLE FileView)
{
	UnmapViewOfFile(FileView);
	CloseHandle(FileMapping);
}
inline BOOL AddSection(CONST CHAR* Filename, ULONG64 Name, ULONG SectionSize, ULONG Characteristics, PVOID DataToCopyIn)
{
	HANDLE FileHandle;
	HANDLE FileMapping;
	PUCHAR FileView;
	ULONG FileSizeLo, FileSizeHi = 0;
	PIMAGE_DOS_HEADER DosHeader;
	PIMAGE_NT_HEADERS NtHeaders;
	ULONG FileAlignment;

	FileHandle = CreateFileA(Filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		return FALSE;

	FileSizeLo = GetFileSize(FileHandle, &FileSizeHi);
	if (FileSizeHi != NULL)
		return FALSE;

	MapFile(FileHandle, &FileMapping, (PVOID*)&FileView, FileSizeLo);

	DosHeader = (PIMAGE_DOS_HEADER)FileView;
	if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		UnMapFile(FileMapping, FileView);
		return FALSE;
	}

	NtHeaders = (PIMAGE_NT_HEADERS64)(FileView + DosHeader->e_lfanew);
	if (NtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		UnMapFile(FileMapping, FileView);
		return FALSE;
	}

	FileAlignment = NtHeaders->OptionalHeader.FileAlignment;

	UnMapFile(FileMapping, FileView);
	MapFile(FileHandle, &FileMapping, (PVOID*)&FileView, P2ALIGNUP(FileSizeLo + SectionSize, FileAlignment));

	//good paste
	DosHeader = (PIMAGE_DOS_HEADER)FileView;
	NtHeaders = (PIMAGE_NT_HEADERS)((UINT_PTR)FileView + DosHeader->e_lfanew);
	PIMAGE_FILE_HEADER FileHeader = &(NtHeaders->FileHeader);
	PIMAGE_SECTION_HEADER FirstSectionHeader = (PIMAGE_SECTION_HEADER)(((UINT_PTR)FileHeader) + sizeof(IMAGE_FILE_HEADER) + NtHeaders->FileHeader.SizeOfOptionalHeader);

	PIMAGE_SECTION_HEADER NewSectionHeader = &FirstSectionHeader[NtHeaders->FileHeader.NumberOfSections]; // Right after last section header.

	PIMAGE_SECTION_HEADER LastSectionHeader = &FirstSectionHeader[NtHeaders->FileHeader.NumberOfSections - 1];

	memset(NewSectionHeader, 0, sizeof(IMAGE_SECTION_HEADER));
	memcpy(NewSectionHeader->Name, &Name, 8);
	NewSectionHeader->Misc.VirtualSize = SectionSize;
	NewSectionHeader->VirtualAddress = P2ALIGNUP(LastSectionHeader->VirtualAddress + LastSectionHeader->Misc.VirtualSize, NtHeaders->OptionalHeader.SectionAlignment);
	NewSectionHeader->SizeOfRawData = P2ALIGNUP(SectionSize, NtHeaders->OptionalHeader.FileAlignment);
	NewSectionHeader->PointerToRawData = FileSizeLo;
	NewSectionHeader->Characteristics = Characteristics;

	NtHeaders->FileHeader.NumberOfSections++;
	NtHeaders->OptionalHeader.SizeOfImage = P2ALIGNUP(NewSectionHeader->VirtualAddress + NewSectionHeader->Misc.VirtualSize, NtHeaders->OptionalHeader.SectionAlignment);


	std::cout << NewSectionHeader->VirtualAddress << ' ' << NewSectionHeader->Misc.VirtualSize << '\n';
	std::cout << NewSectionHeader->PointerToRawData << ' ' << NewSectionHeader->SizeOfRawData << '\n';
	//SOMETHING IS BREAKING HERE
	memcpy(FileView + NewSectionHeader->PointerToRawData, DataToCopyIn, SectionSize);

	UnMapFile(FileMapping, FileView);
	printf("Finished.\n");

	CloseHandle(FileHandle);
}



#endif