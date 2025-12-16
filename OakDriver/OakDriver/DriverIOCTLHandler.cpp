#include <ntddk.h>
#include "OakDriverCommon.h"
#include "DriverIOCTLHandler.h"


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