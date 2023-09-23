#include "Smm.h"

unsigned __int64 SetProtectionCallback()
{
	unsigned __int64 result; // rax

	result = AsmReadMsr(0xC0010015) | 0x80000001;
	AsmWriteMsr(0xC0010015, result);
	return result;

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

EFI_STATUS CpuSmmSetup()
{
	EFI_SMM_SW_DISPATCH2_PROTOCOL* SmmSwDispatchProtocol;

	if (EFI_ERROR(gSMST->SmmLocateProtocol(&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**) & SmmSwDispatchProtocol)))
		return EFI_SUCCESS;

	if (EFI_ERROR(SmmSwDispatchProtocol->Register(SmmSwDispatchProtocol, (EFI_SMM_HANDLER_ENTRY_POINT2)HandlerEntryPoint, (VOID*)&EFI_SWI_VAL, NULL )))
		return EFI_SUCCESS;

	return EFI_SUCCESS;
}

EFI_STATUS EfiMain(UNUSED IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
	gST = SystemTable;
	gBS = SystemTable->BootServices;

	if (EFI_ERROR(gBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&gSMMB2P)))
		return EFI_SUCCESS;

	if (EFI_ERROR(gSMMB2P->GetSmstLocation(gSMMB2P, &gSMST)))
		return EFI_SUCCESS;

	return CpuSmmSetup();
}