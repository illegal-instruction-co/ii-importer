#include "minthreads-exporter/winthreads.hpp"
#include <iostream>

int main()
{
    std::cout << "Kernelbase base address is: ";

    auto exportKernel32 = ii::winthreads::core::exporter("Kernel32.dll");
    std::cout << exportKernel32.invoke("GetModuleHandleA")("Kernelbase.dll") << std::endl;
}

