#pragma once
#include <ntddk.h>

NTSTATUS OakDriverCreateClose(PDEVICE_OBJECT, PIRP);
NTSTATUS OakDriverDeviceControl(PDEVICE_OBJECT, PIRP);
