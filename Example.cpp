#include "include/Importer.hpp"

#include <Windows.h>

#include <thread>
#include <iostream>

using namespace std;

DWORD WINAPI TheThread(void*)
{
	auto ntdll = ii::Importer("ntdll.dll");

	for (;;) {
		cout << "Current processor number: " << ntdll.Invoke<int>("NtGetCurrentProcessorNumber")() << endl;

		this_thread::sleep_for(chrono::milliseconds(500));
	}

	return 0;
}

int main()
{
	auto kernel32 = ii::Importer("Kernel32.dll");
	const void* tHandle = kernel32.Invoke<void*>("CreateThread")(NULL, 0, TheThread, NULL, 0, NULL);

	cout << "CreateThread returned: " << tHandle << endl;

	return getchar();
}
