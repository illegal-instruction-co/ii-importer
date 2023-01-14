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

    DWORD ret = (DWORD)exportKernel32.invoke("CreateThread")(NULL, 0, TheThread, NULL, 0, NULL);
    
    std::cout << "CreateThread returned: " << ret << std::endl;

    getchar();
}

