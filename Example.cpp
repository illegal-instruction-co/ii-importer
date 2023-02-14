#include "include/importer.hpp"

#include <Windows.h>

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

	auto Kernel32 = ii::importer("Kernel32.dll");
	std::cout << Kernel32.invoke<HMODULE>("GetModuleHandleA")("Kernelbase.dll") << std::endl;

	TCHAR buf[MAX_PATH];

	HANDLE tHandle = Kernel32.invoke<HANDLE>(NULL, 0, TheThread, NULL, 0, NULL);

	std::cout << "CreateThread returned: " << tHandle << std::endl;

	auto Ntdll = ii::importer("ntdll.dll");
	std::cout << "Current processor number: " << Ntdll.invoke<int>("NtGetCurrentProcessorNumber")() << std::endl;

	getchar();
}
