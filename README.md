

# ii-importer

*importer is a tiny tool i coded for use later in larger project.*

## What does this importer do?

This little friend saves you from the burden of defining the function template while importing the internal function you need, as you can see in Example.cpp.
Also, this tool is very useful for those who want to use functions as hidden on import table based on simple reason. At the compile time, compiler cant solve your execution of target. In this case, i advice use compile time string obfuscation methods as like https://github.com/adamyaxley/Obfuscate.

In general, it is planned to be used for undocumented and user mode definitions, aka value nt and zw functions.

**You can call these functions without using WinAPI functions like GetProcAddress, GetModuleHandle thanks to CustomAPI.**

## What did we get rid of?

### Case: We want to use NtQueryInformationThread.
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

### Case: We want to use functions that are not shown in the import table.

As you can see on our Example.cpp we used CreateThread function for creating new thread.

Without Importer, our import table view would be like this:

![image](https://raw.githubusercontent.com/illegal-instruction-co/ii-importer/main/assets/import_table_1.png)

With Importer, our import table view will be like this:

![image](https://raw.githubusercontent.com/illegal-instruction-co/ii-importer/main/assets/import_table_2.png)

## Build the example
Just run build.cmd, your sln or what ever will be appears in "build" directory.
*Required cmake > 3.20*


credits:
custom_api.h
