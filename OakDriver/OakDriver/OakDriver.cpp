#include <ntddk.h>

void OakDriverUnload(PDRIVER_OBJECT);
NTSTATUS OakDriverCreateClose(PDEVICE_OBJECT, PIRP);
NTSTATUS OakDriverDeviceControl(PDEVICE_OBJECT, PIRP);

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


	DriverObject->MajorFunction[IRP_MJ_CREATE] = OakDriverCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = OakDriverCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = OakDriverDeviceControl;


	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName, L"\\Device\\OakSecurity");
	PDEVICE_OBJECT DeviceObject;
	status = IoCreateDevice(
		DriverObject,
		0,
		&devName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&DeviceObject
	);
	if (!NT_SUCCESS(status)) {
		KdPrint(("Faild in IoCreateDevice (0x%X)\n", status));
		return status;
	}


	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\OakSecurity");
	status = IoCreateSymbolicLink(&symLink, &devName);
	if (!NT_SUCCESS(status)) {
		IoDeleteDevice(DeviceObject);
		KdPrint(("Faild in IoCreateSymboliclink (0x%X)\n", status));
		return status;
	}


	return STATUS_SUCCESS;
}



void OakDriverUnload(PDRIVER_OBJECT DriverObject) {

	//UNREFERENCED_PARAMETER(DriverObject);

	KdPrint(("OakDriver: Unload\n"));
	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\OakSecurity");
	IoDeleteSymbolicLink(&symLink);

	IoDeleteDevice(DriverObject->DeviceObject);
}