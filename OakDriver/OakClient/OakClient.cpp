#include <Windows.h>
#include <stdio.h>
#include "..\OakDriver\OakDriverCommon.h"


int main()
{

    HANDLE hDevice = CreateFile(L"\\\\.\\OakSecurity", GENERIC_WRITE | GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("Error Opening Device (%u)", GetLastError());
        return 1;
    }

    printf("Current Process Id : (%u)", 25);

    OAKSECURITY_PROCESSID_INPUT input;
    input.ProcessId = 25;

    DWORD returned_bytes = 0;

    if (!DeviceIoControl(hDevice, IOCTL_PROTECT_PROCESS, &input, sizeof input, nullptr, 0, &returned_bytes, nullptr)) {
        printf("Error in DeviceIoControl (%u)", GetLastError());
        CloseHandle(hDevice);
        return 1;
    }

    printf("Success Securing.");

    CloseHandle(hDevice);
    
}


