#pragma once
#include <ntddk.h>

void CreateProcessNotifyRoutine(PEPROCESS, HANDLE, PPS_CREATE_NOTIFY_INFO);