

# ii-importer

*importer is a tiny tool i coded for use later in larger project.*

## What does this importer do?

This little friend saves you from the burden of defining the function template while importing the internal function you need, as you can see in Example.cpp.

In general, it is planned to be used for undocumented and user mode definitions, aka value nt and zw functions.

**You can call these functions without using WinAPI functions like GetProcAddress, GetModuleHandle thanks to CustomAPI.**

## What did we get rid of?

**Case: We want to use NtQueryInformationThread.**
Our usage should have been like this:

    HANDLE hThread;
    typedef NTSTATUS(NTAPI* NtQueryInformationThread_t)(HANDLE, LONG, PVOID, ULONG, PULONG);
    auto NtQueryInformationThread = (NtQueryInformationThread_t)GetProcAddress(hNtdll, "NtQueryInformationThread");
    NTSTATUS ntStat = 0;
    DWORD_PTR dwStartAddress = 0;
    ntStat = NtQueryInformationThread(hThread, 9, &dwStartAddress, sizeof(dwStartAddress), NULL);

With the importer it will be like this:
	  
     HANDLE hThread;
     NTSTATUS ntStat = 0;
     DWORD_PTR dwStartAddress = 0;
     auto exportNtdll = ii::Importer("ntdll.dll");
     ntStat = exportNtdll.Invoke<NTSTATUS>("NtQueryInformationThread")(hThread, 9, &dwStartAddress, sizeof(dwStartAddress), NULL);


## Build the example
Just run build.cmd, your sln or what ever will be appears in "build" directory.
*Required cmake > 3.20*

## Explain the example
I wanted to run GetModuleHandleA function from Kernel32 dynamic library. In this case, I didn't have the GetModuleHandleA template, but I looked online and gave the parameters he wanted, I just focused on the result. I think it's great convenience!

credits:
custom_api.h
