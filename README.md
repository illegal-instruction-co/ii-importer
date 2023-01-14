
# minthreads-exporter

*exporter is a tiny tool i coded for use later in a larger project that I called "minthreads".*

 **what does this exporter do?**

This little friend saves you from the burden of defining the function template while exporting the internal function you need, as you can see in Example.cpp.

In general, it is planned to be used for undocumented and user mode definitions, aka value nt and zw functions.

**Build the example**
Just run build.cmd, your sln or what ever will be appears in "build" directory.
*Required cmake > 3.20*

**Explain the example**
I wanted to run GetModuleHandleA function from Kernel32 dynamic library. In this case, I didn't have the GetModuleHandleA template, but I looked online and gave the parameters he wanted, I just focused on the result. I think it's great convenience!


credits:
custom_api.h
