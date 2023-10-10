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
	UINT32 version;
	UINTN map_size = 0;
	EFI_MEMORY_DESCRIPTOR* map = 0;
	UINTN map_key;
	UINTN descriptor_size;

	EFI_STATUS status = gBS->GetMemoryMap(&map_size, map, &map_key, &descriptor_size, &version);
	do
	{
		status = gBS->AllocatePool(EfiBootServicesData, map_size, (VOID**)&map);

		if (map == NULL)
		{
			return FALSE;
		}

		status = gBS->GetMemoryMap(&map_size, map, &map_key, &descriptor_size, &version);

		if (EFI_ERROR(status))
		{
			gBS->FreePool(map);
			map = 0;
		}

	} while (status == EFI_BUFFER_TOO_SMALL);

	if (map == 0)
		return FALSE;

	UINT32 descriptor_count = (UINT32)map_size / (UINT32)descriptor_size;

	EFI_MEMORY_DESCRIPTOR* entry_last = (EFI_MEMORY_DESCRIPTOR*)((char*)map + ((descriptor_count - 1) * descriptor_size));

	PhysicalMemoryRange.Start = 0x1000;

	PhysicalMemoryRange.End = entry_last->PhysicalStart + EFI_PAGES_TO_SIZE(entry_last->NumberOfPages);

	gBS->FreePool(map);

	return TRUE;
}

BOOLEAN Read(UINT64 Address, VOID* Buffer, UINT64 Size)
{
	if (Address < PhysicalMemoryRange.Start || (Address + Size) > PhysicalMemoryRange.End)
		return FALSE;

	return MemCpy((VOID*)Address, Buffer, Size);
}

BOOLEAN Write(UINT64 Address, VOID* Buffer, UINT64 Size)
{
	if (Address < PhysicalMemoryRange.Start || (Address + Size) > PhysicalMemoryRange.End)
		return FALSE;

	return MemCpy(Buffer, (VOID*)Address, Size);
}

UINT64 Translate(UINT64 DirectoryBase, UINT64 VirtualAddress)
{

	// It's fine to return 0 for failure since we don't even allow reads between 0x0-0x1000
	return 0;
}

DRIVER_STATUS ReadPhysicalToVirtual(VOID* Physical, VOID* VirtualBuffer, UINT64 Size)
{
	return SUCCESS;
}

DRIVER_STATUS WriteVirtualToPhysical(VOID* Physical, VOID* VirtualBuffer, UINT64 Size)
{
	return SUCCESS;
}

DRIVER_STATUS ReadVirtualToVirtual(UINT64 TargetDirectoryBase, VOID* SourceVirtualAddress, VOID* DestinationVirtualAddress, UINT64 Size)
{
	if (Size > MAX_SIZE || Size == 0)
		return ERROR_OVERFLOW;

	UINT64 BytesToRead = Size;

	while (BytesToRead > 0)
	{

	}

	return SUCCESS;
}

DRIVER_STATUS WriteVirtualToVirtual(UINT64 TargetDirectoryBase, VOID* DestinationVirtualAddress, VOID* SourceVirtualAddress, UINT64 Size)
{
	if (Size > MAX_SIZE || Size == 0)
		return ERROR_OVERFLOW;

	return SUCCESS;
}
