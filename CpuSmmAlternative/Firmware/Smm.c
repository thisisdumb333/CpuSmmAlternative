#include "Smm.h"

EFI_SYSTEM_TABLE* gST;

EFI_BOOT_SERVICES* gBS;

EFI_SMM_SYSTEM_TABLE2* gSMST;

EFI_SMM_BASE2_PROTOCOL* gSMMB2P;

EFI_SMM_CPU_PROTOCOL* gSmmCpu;

EFI_GUID gEfiSmmSwDispatch2ProtocolGuid = { 0x18a3c6dc, 0x5eea, 0x48c8, { 0xa1, 0xc1, 0xb5, 0x33, 0x89, 0xf9, 0x89, 0x99 } };
EFI_GUID gEfiSmmBase2ProtocolGuid       = { 0xf4ccbfb7, 0xf6e0, 0x47fd, { 0x9d, 0xd4, 0x10, 0xa8, 0xf1, 0x50, 0xc1, 0x91 } };
EFI_GUID gEfiSmmCpuProtocolGuid         = { 0xeb346b97, 0x975f, 0x4a9f, { 0x8b, 0x22, 0xf8, 0xe9, 0x2b, 0xb3, 0xd5, 0x69 } };

CONST UINTN EFI_SWI_VAL = 0x56;

EFI_HANDLE gCallbackHandle;

UINTN AsmReadMsr(CONST UINTN Register)
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

VOID AsmWriteMsr(CONST UINTN Register, CONST UINTN Value)
{
    __asm 
    {
        mov ecx, Register
        mov eax, dword ptr[Value]
        mov edx, dword ptr[Value + 4]
        wrmsr
    }
}

BOOLEAN ReadCpu64(CONST EFI_MM_SAVE_STATE_REGISTER Register, CONST UINT32* Out)
{
    return 0;
}

BOOLEAN ReadCpu32(CONST EFI_MM_SAVE_STATE_REGISTER Register, CONST UINT32* Out)
{
    return 0;
}

BOOLEAN WriteCpu64(CONST EFI_MM_SAVE_STATE_REGISTER Register, CONST UINT64 Value)
{
    return 0;
}

BOOLEAN WriteCpu32(CONST EFI_MM_SAVE_STATE_REGISTER Register, CONST UINT32 Value)
{
    return 0;
}

XMM ReadXmm5()
{
    XMM result;

    __asm
    {
        movdqu result, xmm5
    }

    return result;
}

void WriteXmm5(CONST XMM Value)
{
    __asm
    {
        movdqu xmm5, Value
    }
}

XMM ReadXmm6() 
{
    XMM result;

    __asm 
    {
        movdqu result, xmm6
    }

    return result;
}

void WriteXmm6(CONST XMM Value)
{
    __asm
    {
        movdqu xmm6, Value
    }
}

BOOLEAN MemCpy(VOID* Source, VOID* Destination, UINT64 Size)
{
    // Null checks and zero size check
    if (!Source || !Destination || !Size)
        return FALSE;

    // Check for overlapping memory regions
    UINT8* SrcByte = (UINT8*)Source;
    UINT8* DestByte = (UINT8*)Destination;

    if ((SrcByte < DestByte && (SrcByte + Size) > DestByte) || (DestByte < SrcByte && (DestByte + Size) > SrcByte))
        return FALSE;

    UINT64* SrcLong  = (UINT64*)SrcByte;
    UINT64* DestLong = (UINT64*)DestByte;
    UINT64 LongCount = Size / sizeof(UINT64);

    for (UINT64 i = 0; i < LongCount; i++)
        DestLong[i] = SrcLong[i];
    
    UINT64 RemainingBytes = Size % sizeof(UINT64);
    SrcByte  += LongCount * sizeof(UINT64);
    DestByte += LongCount * sizeof(UINT64);

    for (UINT64 i = 0; i < RemainingBytes; i++)
        DestByte[i] = SrcByte[i];
    

    return TRUE;
}
