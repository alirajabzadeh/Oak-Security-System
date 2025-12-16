
#include "ProcessNotificationHandler.h"


void CreateProcessNotifyRoutine(PEPROCESS Process, HANDLE Pid, PPS_CREATE_NOTIFY_INFO CreateInfo) {

	UNREFERENCED_PARAMETER(Process);
	UNREFERENCED_PARAMETER(Pid);


	if (CreateInfo != NULL) {

		if (wcsstr(CreateInfo->CommandLine->Buffer, L"notepad") != NULL) {
			DbgPrint("Process (%ws) is lunched \n", CreateInfo->CommandLine->Buffer);

			CreateInfo->CreationStatus = STATUS_ACCESS_DENIED;
		}
	}
	else {
		DbgPrint("Process Exited");
	}
}