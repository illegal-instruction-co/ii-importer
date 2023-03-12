#include "include/Importer.hpp"

#include <Windows.h>

#include <thread>
#include <iostream>

using namespace std;

DWORD WINAPI TheThread(void*)
{
	for (;;) {
		cout << "I am the thread" << endl;
		this_thread::sleep_for(chrono::seconds(1));
	}

	return 0;
}

int main()
{
	cout << "Kernelbase base address is: ";

	auto kernel32 = ii::Importer("Kernel32.dll");
	cout << kernel32.Invoke<HMODULE>("GetModuleHandleA")("Kernelbase.dll") << endl;

	const void* tHandle = kernel32.Invoke<void*>("CreateThread")(NULL, 0, TheThread, NULL, 0, NULL);

	CreateThread(NULL, 0, TheThread, NULL, 0, NULL);

	cout << "CreateThread returned: " << tHandle << endl;

	auto ntdll = ii::Importer("ntdll.dll");
	cout << "Current processor number: " << ntdll.Invoke<int>("NtGetCurrentProcessorNumber")() << endl;

	return getchar();
}
