#include "Smm.h"

EFI_SYSTEM_TABLE* gST;

EFI_BOOT_SERVICES* gBS;

EFI_SMM_SYSTEM_TABLE2* gSMST;

EFI_SMM_BASE2_PROTOCOL* gSMMB2P;

EFI_GUID gEfiSmmBase2ProtocolGuid = { 0xf4ccbfb7, 0xf6e0, 0x47fd, { 0x9d, 0xd4, 0x10, 0xa8, 0xf1, 0x50, 0xc1, 0x91 } };
EFI_GUID gEfiSmmSwDispatch2ProtocolGuid = { 0x18a3c6dc, 0x5eea, 0x48c8, { 0xa1, 0xc1, 0xb5, 0x33, 0x89, 0xf9, 0x89, 0x99 } };

CONST UINTN EFI_SWI_VAL = 0x56;

UINTN AsmReadMsr(UINTN Register)
{
    UINTN result = 0;

    __asm 
    {
        mov ecx, Register
        rdmsr
        mov dword ptr[result], eax
        mov dword ptr[result + 4], edx
    }
    return result;
}

VOID AsmWriteMsr(UINTN Register, UINTN Value)
{
    __asm 
    {
        mov ecx, Register
        mov eax, dword ptr[Value]
        mov edx, dword ptr[Value + 4]
        wrmsr
    }
}
