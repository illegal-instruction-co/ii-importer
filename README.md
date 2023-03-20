

# ii-importer ![GitHub](https://img.shields.io/github/license/illegal-instruction-co/ii-importer?logo=ii-importer) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/illegal-instruction-co/ii-importer)

*importer is a tiny tool designed to dodge reverse engineers that i coded for use later in larger project.*

## What does this importer do?

This little friend saves you from the burden of defining the function template while importing the internal function you need, as you can see in Example.cpp.
Also, this tool is very useful for those who want to use functions as hidden on import table based on simple reason. At the compile time, compiler cant solve your execution of target. In this case, i advice use compile time string obfuscation methods as like https://github.com/adamyaxley/Obfuscate. The tool also supports compile-time string obfuscation, which further increases the difficulty for reverse engineers to understand the code.

In general, it is planned to be used for undocumented and user mode definitions, aka value nt and zw functions.

**You can call these functions without using WinAPI functions like GetProcAddress, GetModuleHandle thanks to CustomAPI.**

## What did we get rid of?

### Case: We want to use NtQueryInformationThread.
Our usage should have been like this:

```cpp
HANDLE hThread;
typedef NTSTATUS(NTAPI* NtQueryInformationThread_t)(HANDLE, LONG, PVOID, ULONG, PULONG);
uint64_t hNtdll = GetModuleHandleA("ntdll.dll");
NtQueryInformationThread_t NtQueryInformationThread = (NtQueryInformationThread_t)GetProcAddress(hNtdll, "NtQueryInformationThread");
NTSTATUS ntStat = 0;
DWORD_PTR dwStartAddress = 0;
ntStat = NtQueryInformationThread(hThread, 9, &dwStartAddress, sizeof(dwStartAddress), NULL);
```

With the importer it will be like this:

```cpp
HANDLE hThread;
NTSTATUS ntStat = 0;
DWORD_PTR dwStartAddress = 0;
auto exportNtdll = ii::Importer("ntdll.dll");
ntStat = exportNtdll.Invoke<NTSTATUS>("NtQueryInformationThread")(hThread, 9, &dwStartAddress, sizeof(dwStartAddress), NULL);
```

### Case: We want to use functions that are not shown in the import table.

As you can see on our Example.cpp we used CreateThread function for creating new thread.

Without Importer, our import table view would be like this:

![image](https://raw.githubusercontent.com/illegal-instruction-co/ii-importer/main/assets/import_table_1.png)

With Importer, our import table view will be like this:

![image](https://raw.githubusercontent.com/illegal-instruction-co/ii-importer/main/assets/import_table_2.png)

## Disassembled pseudo code of main function without ii-importer and compile time string obfuscation
```cpp
int __cdecl main(int argc, const char **argv, const char **envp)
{
  HANDLE v3; // rbx
  __int64 v4; // rax
  __int64 v5; // rax

  v3 = CreateThread(0i64, 0i64, StartAddress, 0i64, 0, 0i64);
  v4 = sub_1400012F0(std::cout, "CreateThread returned: ");
  v5 = std::ostream::operator<<(v4, v3);
  std::ostream::operator<<(v5, sub_140001B50);
  return getchar();
}
```

## Disassembled pseudo code of main function with ii-importer and without compile time string obfuscation

```cpp
int __cdecl main(int argc, const char **argv, const char **envp)
{
  __int64 v3; // rdx
  unsigned __int64 i; // rcx
  __int64 v5; // rbx
  __int64 v6; // rdi
  __int64 v7; // rax
  __int64 v8; // rbx
  __int64 v9; // rax
  __int64 v10; // rax
  __int64 v12; // rbx
  __int64 v13; // rax
  __int64 v14; // rax
  __int64 pExceptionObject[4]; // [rsp+30h] [rbp-E8h] BYREF
  char v16[32]; // [rsp+50h] [rbp-C8h] BYREF
  char v17[8]; // [rsp+70h] [rbp-A8h] BYREF
  __int64 v18; // [rsp+78h] [rbp-A0h]
  __int64 v19; // [rsp+88h] [rbp-90h]
  __int64 v20; // [rsp+A0h] [rbp-78h]
  char v21[16]; // [rsp+B0h] [rbp-68h] BYREF
  char v22[32]; // [rsp+C0h] [rbp-58h] BYREF
  char v23[32]; // [rsp+E0h] [rbp-38h] BYREF

  pExceptionObject[0] = (__int64)"Kernel32.dll";
  pExceptionObject[1] = 12i64;
  sub_140001DD0(v16, pExceptionObject, envp);
  v3 = 0xCBF29CE484222325ui64;
  for ( i = 0i64; i < 0xC; ++i )
    v3 = 0x100000001B3i64 * ((unsigned __int8)aCreatethread[i] ^ (unsigned __int64)v3);
  v5 = *(_QWORD *)(v19 + 16 * (v3 & v20) + 8);
  if ( v5 == v18 )
  {
LABEL_9:
    v5 = 0i64;
  }
  else
  {
    v6 = *(_QWORD *)(v19 + 16 * (v3 & v20));
    while ( *(_QWORD *)(v5 + 24) != 12i64 || memcmp("CreateThread", *(const void **)(v5 + 16), 0xCui64) )
    {
      if ( v5 == v6 )
        goto LABEL_9;
      v5 = *(_QWORD *)(v5 + 8);
    }
  }
  v7 = v18;
  if ( v5 )
    v7 = v5;
  if ( v7 == v18 )
  {
    v12 = sub_1400033B0(v21);
    v13 = sub_1400015A0(v22);
    v14 = sub_1400014C0(v23, v13, v12);
    sub_140002310(pExceptionObject, v14);
    CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI2_AVruntime_error_std__);
  }
  v8 = (*(__int64 (__fastcall **)(_QWORD, _QWORD, void (__fastcall __noreturn *)(), _QWORD, _QWORD, _QWORD))(v7 + 32))(
         0i64,
         0i64,
         sub_140002980,
         0i64,
         0i64,
         0i64);
  v9 = sub_1400012F0(std::cout, "CreateThread returned: ");
  v10 = std::ostream::operator<<(v9, v8);
  std::ostream::operator<<(v10, sub_140001B50);
  LODWORD(v8) = getchar();
  sub_140002390(v17);
  return v8;
}
```

## Disassembled pseudo code of main function with ii-importer and compile time string obfuscation
```cpp
int __cdecl main(int argc, const char **argv, const char **envp)
{
  __int64 v3; // r14
  int v4; // eax
  _DWORD *v5; // rbx
  size_t v6; // rsi
  __int64 v7; // rax
  __int64 v8; // r8
  int v9; // eax
  _BYTE *v10; // rbx
  __int64 v11; // rdx
  size_t i; // rcx
  __int64 v13; // rdi
  __int64 v14; // r15
  __int64 v15; // rax
  __int64 v16; // rdi
  __int64 v17; // r8
  __m128i v18; // xmm0
  int v19; // eax
  __m128i *v20; // rbx
  __int64 v21; // rax
  __int64 v22; // rax
  int v23; // ebx
  __int64 v25; // rdi
  __int64 v26; // rax
  __int64 v27; // rax
  char v28[32]; // [rsp+30h] [rbp-89h] BYREF
  char v29[8]; // [rsp+50h] [rbp-69h] BYREF
  __int64 v30; // [rsp+58h] [rbp-61h]
  __int64 v31; // [rsp+68h] [rbp-51h]
  __int64 v32; // [rsp+80h] [rbp-39h]
  char v33[16]; // [rsp+90h] [rbp-29h] BYREF
  __int64 pExceptionObject[2]; // [rsp+A0h] [rbp-19h] BYREF
  __int64 v35; // [rsp+B0h] [rbp-9h] BYREF
  char v36[32]; // [rsp+B8h] [rbp-1h] BYREF
  char v37[32]; // [rsp+D8h] [rbp+1Fh] BYREF

  v3 = *((_QWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + (unsigned int)TlsIndex);
  v4 = *(_DWORD *)(v3 + 120);
  v5 = (_DWORD *)(v3 + 104);
  if ( (v4 & 1) == 0 )
  {
    *(_DWORD *)(v3 + 120) = v4 | 1;
    *(_DWORD *)(v3 + 114) = 28437901;
    *v5 = -275562842;
    *(_DWORD *)(v3 + 108) = 1306575316;
    *(_WORD *)(v3 + 112) = 16323;
    _tlregdtor(qword_140004FA0, argv, envp);
  }
  if ( *(_BYTE *)(v3 + 117) )
  {
    *(_BYTE *)v5 ^= 0xEDu;
    *(_BYTE *)(v3 + 105) ^= 0x5Bu;
    *(_BYTE *)(v3 + 106) ^= 0xE1u;
    *(_BYTE *)(v3 + 107) ^= 0x81u;
    *(_BYTE *)(v3 + 108) ^= 0xB1u;
    *(_BYTE *)(v3 + 109) ^= 0xADu;
    *(_BYTE *)(v3 + 110) ^= 0xD3u;
    *(_BYTE *)(v3 + 111) ^= 0x7Fu;
    *(_BYTE *)(v3 + 112) ^= 0xEDu;
    *(_BYTE *)(v3 + 113) ^= 0x5Bu;
    *(_BYTE *)(v3 + 114) ^= 0xE1u;
    *(_BYTE *)(v3 + 115) ^= 0x81u;
    *(_BYTE *)(v3 + 116) ^= 0xB1u;
    *(_BYTE *)(v3 + 117) = 0;
  }
  pExceptionObject[0] = v3 + 104;
  v6 = -1i64;
  v7 = -1i64;
  do
    ++v7;
  while ( *((_BYTE *)v5 + v7) );
  pExceptionObject[1] = v7;
  sub_140001DD0(v28, pExceptionObject, envp);
  v9 = *(_DWORD *)(v3 + 144);
  if ( (v9 & 1) != 0 )
  {
    v10 = (_BYTE *)(v3 + 128);
  }
  else
  {
    *(_DWORD *)(v3 + 144) = v9 | 1;
    v10 = (_BYTE *)(v3 + 128);
    *(_DWORD *)(v3 + 138) = 27855640;
    *(_DWORD *)(v3 + 128) = 236773648;
    *(_DWORD *)(v3 + 132) = -371746595;
    *(_WORD *)(v3 + 136) = -2527;
    _tlregdtor(qword_140004FD0, 128i64, v8);
  }
  if ( v10[13] )
  {
    *v10 ^= 0x53u;
    v10[1] ^= 0x93u;
    v10[2] ^= 0x79u;
    v10[3] ^= 0x6Fu;
    v10[4] ^= 0xA9u;
    v10[5] ^= 0xFDu;
    v10[6] ^= 0x83u;
    v10[7] ^= 0x81u;
    v10[8] ^= 0x53u;
    v10[9] ^= 0x93u;
    v10[10] ^= 0x79u;
    v10[11] ^= 0x6Fu;
    v10[12] ^= 0xA9u;
    v10[13] = 0;
  }
  do
    ++v6;
  while ( v10[v6] );
  v11 = 0xCBF29CE484222325ui64;
  for ( i = 0i64; i < v6; ++i )
    v11 = 0x100000001B3i64 * ((unsigned __int8)v10[i] ^ (unsigned __int64)v11);
  v13 = *(_QWORD *)(v31 + 16 * (v11 & v32) + 8);
  if ( v13 == v30 )
  {
LABEL_21:
    v13 = 0i64;
  }
  else
  {
    v14 = *(_QWORD *)(v31 + 16 * (v11 & v32));
    while ( v6 != *(_QWORD *)(v13 + 24) || memcmp(v10, *(const void **)(v13 + 16), v6) )
    {
      if ( v13 == v14 )
        goto LABEL_21;
      v13 = *(_QWORD *)(v13 + 8);
    }
  }
  v15 = v30;
  if ( v13 )
    v15 = v13;
  if ( v15 == v30 )
  {
    v25 = sub_1400036A0(v33);
    v26 = sub_1400015A0(v36);
    v27 = sub_1400014C0(v37, v26, v25);
    sub_140002310(pExceptionObject, v27);
    CxxThrowException(pExceptionObject, (_ThrowInfo *)&_TI2_AVruntime_error_std__);
  }
  v16 = (*(__int64 (__fastcall **)(_QWORD, _QWORD, void (__fastcall __noreturn *)(), _QWORD, _QWORD, _QWORD))(v15 + 32))(
          0i64,
          0i64,
          sub_140002980,
          0i64,
          0i64,
          0i64);
  v18 = _mm_load_si128((const __m128i *)&xmmword_1400065A0);
  v35 = 0xC149E9C56C553144ui64;
  v19 = *(_DWORD *)(v3 + 180);
  if ( (v19 & 1) != 0 )
  {
    v20 = (__m128i *)(v3 + 152);
  }
  else
  {
    *(_DWORD *)(v3 + 180) = v19 | 1;
    v20 = (__m128i *)(v3 + 152);
    *(_BYTE *)(v3 + 176) = 1;
    if ( v3 + 152 > (unsigned __int64)&v35 + 7 || v3 + 175 < (unsigned __int64)pExceptionObject )
    {
      *v20 = v18;
      *(_QWORD *)(v3 + 168) = v35;
    }
    else
    {
      *v20 = v18;
      *(_QWORD *)(v3 + 168) = v35;
    }
    _tlregdtor(qword_140005030, 152i64, v17);
  }
  if ( v20[1].m128i_i8[8] )
  {
    v20->m128i_i8[0] ^= 0x31u;
    v20->m128i_i8[1] ^= 0x43u;
    v20->m128i_i8[2] ^= 0x3Bu;
    v20->m128i_i8[3] ^= 9u;
    v20->m128i_i8[4] ^= 0xA1u;
    v20->m128i_i8[5] ^= 0xD3u;
    v20->m128i_i8[6] ^= 0x69u;
    v20->m128i_i8[7] ^= 0xC1u;
    v20->m128i_i8[8] ^= 0x31u;
    v20->m128i_i8[9] ^= 0x43u;
    v20->m128i_i8[10] ^= 0x3Bu;
    v20->m128i_i8[11] ^= 9u;
    v20->m128i_i8[12] ^= 0xA1u;
    v20->m128i_i8[13] ^= 0xD3u;
    v20->m128i_i8[14] ^= 0x69u;
    v20->m128i_i8[15] ^= 0xC1u;
    v20[1].m128i_i8[0] ^= 0x31u;
    v20[1].m128i_i8[1] ^= 0x43u;
    v20[1].m128i_i8[2] ^= 0x3Bu;
    v20[1].m128i_i8[3] ^= 9u;
    v20[1].m128i_i8[4] ^= 0xA1u;
    v20[1].m128i_i8[5] ^= 0xD3u;
    v20[1].m128i_i8[6] ^= 0x69u;
    v20[1].m128i_i8[7] ^= 0xC1u;
    v20[1].m128i_i8[8] = 0;
  }
  v21 = sub_1400012F0(std::cout, v20);
  v22 = std::ostream::operator<<(v21, v16);
  std::ostream::operator<<(v22, sub_140001B50);
  v23 = getchar();
  sub_140002390(v29);
  return v23;
}
```

## Build the example
Just run build.cmd, your sln or what ever will be appears in "build" directory.
*Required cmake > 3.20*


## Attention
ii-importer does not to present lazy importing option. if you are looking a tool for lazy-importing check:
https://github.com/JustasMasiulis/lazy_importer

credits:
custom_api.h
obfuscate.h
