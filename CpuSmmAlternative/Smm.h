#ifndef SMM_H_
#define SMM_H_

#pragma warning(push, 0)
#include <Uefi.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
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


///
/// GUID's for various SMM protocols used
///
extern EFI_GUID gEfiSmmBase2ProtocolGuid;
extern EFI_GUID gEfiSmmSwDispatch2ProtocolGuid;

///
/// SWI_VAL
/// 
extern CONST UINTN EFI_SWI_VAL;

///
/// GUID's for various SMM protocols used
///
typedef struct  _COMM_BUFFER
{
	UINT8 gap0[8];
	UINT8 SWI_VAL;
}COMM_BUFFER;

UINTN AsmReadMsr(UINTN Register);

VOID AsmWriteMsr(UINTN Register, UINTN Value);

#endif