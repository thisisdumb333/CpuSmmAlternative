#include "Firmware/Smm.h"
#include "Memory/Memory.h"

UINTN SetProtectionCallback()
{
	UINTN Result = AsmReadMsr(0xC0010015) | 0x80000001;
	AsmWriteMsr(0xC0010015, Result);
	return Result;
}

EFI_STATUS HandlerEntryPoint(UNUSED CONST EFI_HANDLE Handle, UNUSED CONST VOID* Context, CONST VOID* CommBuffer, UNUSED CONST UINTN* CommBufferSize)
{
	if (!CommBuffer)
		return EFI_SUCCESS;

	CONST COMM_BUFFER* Buffer = (COMM_BUFFER*)CommBuffer;

	if (Buffer->SWI_VAL != EFI_SWI_VAL)
		return EFI_SUCCESS;

	UINTN MSR = AsmReadMsr(0xC0010015);

	if ((MSR & 1) != 0)
		return EFI_SUCCESS;

	AsmWriteMsr(0xC0010015, MSR | 0x80000001);

	for ( UINTN i = 1; i < gSMST->NumberOfCpus; ++i)
		(gSMST->SmmStartupThisAp)((EFI_AP_PROCEDURE)SetProtectionCallback, i, NULL);

	return EFI_SUCCESS;
}


EFI_STATUS SmiHandler(UNUSED CONST EFI_HANDLE Handle, UNUSED CONST VOID* Context, UNUSED CONST VOID* CommBuffer, UNUSED CONST UINTN* CommBufferSize)
{

	CONST XMM Operands1 = ReadXmm5();

	XMM Response;
	Response.Value.Low  = 0x0;
	Response.Value.High = 0x0;

	// Only the core that initiated the 
	// SMI should have the key in XMM5
	if (Operands1.Buffer1.Key != SMM_KEY)
		return EFI_SUCCESS;

	UINT64 DirectoryBase = 0;

	if (!ReadCpu64(EFI_MM_SAVE_STATE_REGISTER_CR3, &DirectoryBase))
	{
		// Firmware bug, AMD64 gurantees CR3 is saved, we could 
		// read From the Save State ourselves if this happens
		Response.Buffer1.Status  = ERROR_FIRMWARE;

		WriteXmm5(Response);

		return EFI_SUCCESS;
	}

	Cache(DirectoryBase);

	CONST XMM Operands2 = ReadXmm6();

	switch (Operands1.Buffer1.Operation)
	{
	case READ:
		break;
	case WRITE:
		break;
	default:
		Response.Buffer1.Status = ERROR_INVALID;
		break;
	}

	WriteXmm5(Response);

	return EFI_SUCCESS;
}

EFI_STATUS CpuSmmSetup()
{
	CONST EFI_SMM_SW_DISPATCH2_PROTOCOL* SmmSwDispatchProtocol;

	if (EFI_ERROR(gSMST->SmmLocateProtocol(&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**) & SmmSwDispatchProtocol)))
		return EFI_SUCCESS;

	SmmSwDispatchProtocol->Register(SmmSwDispatchProtocol, (EFI_SMM_HANDLER_ENTRY_POINT2)HandlerEntryPoint, (VOID*)&EFI_SWI_VAL, NULL);

	gSMST->SmiHandlerRegister((EFI_SMM_HANDLER_ENTRY_POINT2)SmiHandler, NULL, &gCallbackHandle);

	return EFI_SUCCESS;	
}

EFI_STATUS EfiMain(UNUSED IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
	gST = SystemTable;
	gBS = SystemTable->BootServices;

	if (!QueryPhysicalMemory())
		return EFI_SUCCESS;

	if (EFI_ERROR(gBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&gSMMB2P)))
		return EFI_SUCCESS;

	if (EFI_ERROR(gSMMB2P->GetSmstLocation(gSMMB2P, &gSMST)))
		return EFI_SUCCESS;

	return CpuSmmSetup();
}