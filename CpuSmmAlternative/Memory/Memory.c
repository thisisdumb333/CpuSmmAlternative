#include "Memory.h"

#include "../Firmware/Smm.h"

#include "../Amd64/Page.h"

STATIC PHYSICAL_MEMORY PhysicalMemoryRange;
STATIC UINT64 CacheDirectoryBase;

VOID Cache(CONST UINT64 CR3)
{
	CacheDirectoryBase = CR3;
}

BOOLEAN QueryPhysicalMemory()
{
	UINT32 Version;
	UINTN MapSize = 0;
	EFI_MEMORY_DESCRIPTOR* Map = NULL;
	UINTN MapKey;
	UINTN DescriptorSize;

	EFI_STATUS Status = gBS->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &Version);
	do
	{
		Status = gBS->AllocatePool(EfiBootServicesData, MapSize, (VOID**)&Map);

		if (Map == NULL)
			return FALSE;
		

		Status = gBS->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &Version);

		if (EFI_ERROR(Status))
		{
			gBS->FreePool(Map);
			Map = 0;
		}

	} while (Status == EFI_BUFFER_TOO_SMALL);

	if (Map == 0)
		return FALSE;

	UINT32 DescriptorCount = (UINT32)MapSize / (UINT32)DescriptorSize;

	EFI_MEMORY_DESCRIPTOR* EntryLast = (EFI_MEMORY_DESCRIPTOR*)((char*)Map + ((DescriptorCount - 1) * DescriptorSize));

	PhysicalMemoryRange.Start = 0x1000;

	PhysicalMemoryRange.End = EntryLast->PhysicalStart + EFI_PAGES_TO_SIZE(EntryLast->NumberOfPages);

	gBS->FreePool(Map);

	return TRUE;
}

BOOLEAN VirtualCopy(UINT64 Address, VOID* Buffer, UINT64 Size)
{
	if (Address < PhysicalMemoryRange.Start || (Address + Size) > PhysicalMemoryRange.End)
		return FALSE;

	return MemCpy((VOID*)Address, Buffer, Size);
}

UINT64 Translate(UNUSED UINT64 DirectoryBase, UNUSED UINT64 VirtualAddress)
{

	// It's fine to return 0 for failure since we don't even allow reads between 0x0-0x1000, as no OS uses that physical memory (and can't)
	return 0;
}

DRIVER_STATUS CopyVirtualMemory(UINT64 SourceDirectoryBase, VOID* SourceVirtualAddress, UINT64 DestinationDirectoryBase, VOID* DestinationVirtualAddress, UINT64 Size)
{
	if (Size > MAX_SIZE || Size == 0)
		return ERROR_OVERFLOW;

	UINT64 BytesToCopy = Size;

	while (BytesToCopy > 0)
	{
		// Grab the starting PFNs of the source and destination
		UINT64 PhysicalSourceAddress	  = Translate(SourceDirectoryBase, (UINT64)SourceVirtualAddress);
		UINT64 PhysicalDestinationAddress = Translate(DestinationDirectoryBase, (UINT64)DestinationVirtualAddress);

		if (PhysicalSourceAddress == 0 || PhysicalDestinationAddress == 0)
			return ERROR_INVALID_DIRECTORYBASE;

		// Calculate how many bytes can be copied from the current page for source and destination
		UINT64 BytesInsideSourcePage = EFI_PAGE_SIZE - ((UINT64)SourceVirtualAddress & EFI_PAGE_MASK);
		UINT64 BytesInsideDestinationPage = EFI_PAGE_SIZE - ((UINT64)DestinationVirtualAddress & EFI_PAGE_MASK);

		// Determine the smallest value among the bytes inside the source page, destination page, and remaining bytes to copy
		UINT64 CurrentCopySize = MIN(BytesInsideSourcePage, BytesInsideDestinationPage);
		CurrentCopySize = MIN(CurrentCopySize, BytesToCopy);

		if (!MemCpy((VOID*)PhysicalDestinationAddress, (VOID*)PhysicalSourceAddress, CurrentCopySize))
			return ERROR_FAIL;

		SourceVirtualAddress += CurrentCopySize;
		DestinationVirtualAddress += CurrentCopySize;
		BytesToCopy -= CurrentCopySize;

	}

	return SUCCESS;
}
