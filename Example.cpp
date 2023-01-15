#include "winthreads-exporter/winthreads.hpp"
#include <iostream>

DWORD WINAPI TheThread(LPVOID lpParam)
{

    for (;;) {
        std::cout << "I am the thread" << std::endl;
        Sleep(1000); 
    }

    return 0;
}

int main()
{
    std::cout << "Kernelbase base address is: ";

    auto exportKernel32 = ii::winthreads::core::exporter("Kernel32.dll");
    std::cout << exportKernel32.invoke("GetModuleHandleA")("Kernelbase.dll") << std::endl;

    TCHAR buf[MAX_PATH];

    HANDLE tHandle = (HANDLE)exportKernel32.invoke("CreateThread")(NULL, 0, TheThread, NULL, 0, NULL);
    
    std::cout << "CreateThread returned: " << tHandle << std::endl;

    auto exportNtdll = ii::winthreads::core::exporter("ntdll.dll");
    std::cout << "Current processor number: " << exportNtdll.invoke("NtGetCurrentProcessorNumber")() << std::endl;

    getchar();
}

