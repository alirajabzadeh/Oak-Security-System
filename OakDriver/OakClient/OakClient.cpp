#include <Windows.h>
#include <stdio.h>

int main()
{

    HANDLE hDevice = CreateFile(L"\\\\.\\OakSecurity", GENERIC_WRITE | GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("Error Opening Device (%u)", GetLastError());
        return 1;
    }

    CloseHandle(hDevice);
    
}


