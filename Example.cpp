#include "include/Importer.hpp"

#include <Windows.h>

#include <thread>
#include <iostream>

using namespace std;

DWORD WINAPI TheThread(void*)
{
	/*
	* With compile time string obfuscation
	* optionable
	*/
	auto ntdll = II_IMPORTER("ntdll.dll");

	for (;;) {
		cout << II_STRING("Current processor number: ") << ntdll.Invoke<int>(II_STRING("NtGetCurrentProcessorNumber"))() << endl;

		this_thread::sleep_for(chrono::milliseconds(500));
	}

	return 0;
}

int main()
{
	/*
	* Without compile time string obfuscation
	* optional
	*/
	auto kernel32 = ii::Importer("Kernel32.dll");
	const void* tHandle = kernel32.Invoke<void*>("CreateThread")(NULL, 0, TheThread, NULL, 0, NULL);

	cout << "CreateThread returned: " << tHandle << endl;

	return getchar();
}
