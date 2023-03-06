#include "include/Importer.hpp"

#include <Windows.h>

#include <iostream>

DWORD WINAPI TheThread(LPVOID)
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

	auto Kernel32 = ii::Importer("Kernel32.dll");
	std::cout << Kernel32.Invoke<HMODULE>("GetModuleHandleA")("Kernelbase.dll") << std::endl;

	const void* tHandle = Kernel32.Invoke<void*>("CreateThread")(NULL, 0, TheThread, NULL, 0, NULL);

	std::cout << "CreateThread returned: " << tHandle << std::endl;

	auto Ntdll = ii::Importer("ntdll.dll");
	std::cout << "Current processor number: " << Ntdll.Invoke<int>("NtGetCurrentProcessorNumber")() << std::endl;

	return getchar();
}
