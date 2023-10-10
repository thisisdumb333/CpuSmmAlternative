#ifndef SMM_H_
#define SMM_H_

#pragma warning(push, 0)
#include <Uefi.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmCpu.h>
#pragma warning(pop)

#define UNUSED __attribute__((unused))


///
/// EFI System Table, not for runtime usage
///
extern EFI_SYSTEM_TABLE* gST;

///
/// EFI Boot Services Table.
///
extern EFI_BOOT_SERVICES* gBS;

///
/// System Management System Table (SMST)
///
/// The System Management System Table (SMST) is a table that contains a collection of common
/// services for managing SMRAM allocation and providing basic I/O services. These services are
/// intended for both preboot and runtime usage.
///
extern EFI_SMM_SYSTEM_TABLE2* gSMST;

///
/// EFI SMM Base2 Protocol is utilized by all SMM drivers to locate the SMM infrastructure
/// services and determine whether the driver is being invoked inside SMRAM or outside of SMRAM.
///
extern EFI_SMM_BASE2_PROTOCOL* gSMMB2P;

extern EFI_SMM_CPU_PROTOCOL* gSmmCpu;

///
/// GUID's for various SMM protocols used
///
extern EFI_GUID gEfiSmmBase2ProtocolGuid;
extern EFI_GUID gEfiSmmSwDispatch2ProtocolGuid;
extern EFI_GUID gEfiSmmCpuProtocolGuid;
///
/// SWI_VAL
/// 
extern CONST UINTN EFI_SWI_VAL;

///
/// Callback Handle for SMI interrupt
///
extern EFI_HANDLE gCallbackHandle;

///
/// CommBuffer structure used by MSI
///
typedef struct  _COMM_BUFFER
{
	UINT8 gap0[8];
	UINT8 SWI_VAL;
}COMM_BUFFER;

UINTN AsmReadMsr(CONST UINTN Register);

VOID AsmWriteMsr(CONST UINTN Register, CONST UINTN Value);

BOOLEAN ReadCpu64(CONST EFI_MM_SAVE_STATE_REGISTER Register, CONST UINT64* Out);
BOOLEAN ReadCpu32(CONST EFI_MM_SAVE_STATE_REGISTER Register, CONST UINT32* Out);

BOOLEAN WriteCpu64(CONST EFI_MM_SAVE_STATE_REGISTER Register, CONST UINT64 Value);
BOOLEAN WriteCpu32(CONST EFI_MM_SAVE_STATE_REGISTER Register, CONST UINT32 Value);

typedef union _XMM
{
	struct
	{
		UINT64 Low;
		UINT64 High;
	} Value;

	struct
	{
		UINT16 Key;
		UINT8  Operation;
		UINT8  Status;
		UINT32 Parameter1;
		UINT64 Parameter2;
	}Buffer1;

	struct
	{
		UINT64 Parameter3;
		UINT64 Parameter4;
	}Buffer2;

}XMM;


XMM ReadXmm5();
VOID WriteXmm5(CONST XMM Value);

XMM ReadXmm6();
VOID WriteXmm6(CONST XMM Value);

BOOLEAN MemCpy(VOID* Source, VOID* Destination, UINT64 Size);

#endif