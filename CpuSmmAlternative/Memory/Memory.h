#ifndef MEMORY_H_
#define MEMORY_H_

#pragma warning(push, 0)
#include <Uefi.h>
#pragma warning(pop)

typedef struct _PHYSICAL_MEMORY
{
	UINT64 Start;
	UINT64 End;
}PHYSICAL_MEMORY;

#define SMM_KEY 0x4A2F

// Maximum read size, 63KB, maximum size that can fit into a UINT16, this is purely because of how we pass in arguments
#define MAX_SIZE 0xFC00 

#define READ  0x0001
#define WRITE 0x0002

#define DRIVER_STATUS UINT8

#define SUCCESS 0x0

// Firmware error, we probably didn't do anything wrong here?
// Maybe? Hopefully?
#define ERROR_FIRMWARE   0x1

// One of the params passed in is invalid,
// did not translate, some other issue
#define ERROR_INVALID			    0x2
#define ERROR_INVALID_PARAMETER1	0x21
#define ERROR_INVALID_PARAMETER2	0x22
#define ERROR_INVALID_PARAMETER3	0x23
#define ERROR_INVALID_DIRECTORYBASE 0x24

// Size is greater then SIZE_4KB
#define ERROR_OVERFLOW		  0x3

// General memory access failures, should only fail
// if we're outside of the physical memory range
#define ERROR_FAIL       0x4
#define ERROR_READ_FAIL  0x41
#define ERROR_WRITE_FAIL 0x42

VOID Cache(UINT64 DirectoryBase);

BOOLEAN QueryPhysicalMemory();
BOOLEAN VirtualCopy(UINT64 Address, VOID* Buffer, UINT64 Size);

UINT64 Translate(UINT64 DirectoryBase, UINT64 VirtualAddress);

DRIVER_STATUS CopyVirtualMemory(UINT64 SourceDirectoryBase, VOID* SourceVirtualAddress, UINT64 DestinationDirectoryBase, VOID* DestinationVirtualAddress, UINT64 Size);


#endif