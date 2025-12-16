#include <ntddk.h>

void OakDriverUnload(PDRIVER_OBJECT);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {

	//UNREFERENCED_PARAMETER(DriverObject);
	//UNREFERENCED_PARAMETER(RegistryPath);

	KdPrint(("OakDriver: DriverEntry\n"));
	KdPrint(("Registry Path: %wZ\n", RegistryPath));

	DriverObject->DriverUnload = OakDriverUnload;

	RTL_OSVERSIONINFOW vi = { sizeof(vi) };
	NTSTATUS status = RtlGetVersion(&vi);
	if (!NT_SUCCESS(status)) {
		KdPrint(("Error: Faild in RtlGetVersion (0x%X)", status));

		return status;
	}

	return STATUS_SUCCESS;
}

void OakDriverUnload(PDRIVER_OBJECT DriverObject) {

	UNREFERENCED_PARAMETER(DriverObject);


	KdPrint(("OakDriver: Unload\n"));
}