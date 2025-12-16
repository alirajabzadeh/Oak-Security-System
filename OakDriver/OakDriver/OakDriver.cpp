#include <ntddk.h>
#include "OakDriverCommon.h"

void OakDriverUnload(PDRIVER_OBJECT);
NTSTATUS OakDriverCreateClose(PDEVICE_OBJECT, PIRP);
NTSTATUS OakDriverDeviceControl(PDEVICE_OBJECT, PIRP);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {

	//UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	KdPrint(("OakDriver: DriverEntry\n"));

	DriverObject->DriverUnload = OakDriverUnload;

	NTSTATUS status;


	//Dispatch Routine Defenition
	DriverObject->MajorFunction[IRP_MJ_CREATE] = OakDriverCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = OakDriverCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = OakDriverDeviceControl;


	// Create Device
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


	
	//Create Symbolic link
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


NTSTATUS OakDriverCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, 0);

	return STATUS_SUCCESS;
}

NTSTATUS OakDriverDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

	UNREFERENCED_PARAMETER(DeviceObject);

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	size_t input_buffer_length = stack->Parameters.DeviceIoControl.InputBufferLength;

	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	switch (stack->Parameters.DeviceIoControl.IoControlCode) {
		case IOCTL_PROTECT_PROCESS: {
			if (input_buffer_length != sizeof OAKSECURITY_PROCESSID_INPUT) {
				KdPrint(("INVALID PARAMETER"));
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			auto input = (OAKSECURITY_PROCESSID_INPUT*)Irp->AssociatedIrp.SystemBuffer;


			DbgPrint("Your input Process id is : (%u)", input->ProcessId);


			status = STATUS_SUCCESS;
			break;

		}
	}


	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, 0);

	return status;

}

