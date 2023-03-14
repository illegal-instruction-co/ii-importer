#pragma once

#include <Windows.h>

#include <functional>

namespace ii {
	class CustomAPI final {
	public:

		CustomAPI() = delete;

	private:

		static wchar_t* GetFileNameFromPath(wchar_t* Path)
		{
			wchar_t* LastSlash = NULL;
			for (DWORD i = 0; Path[i] != NULL; i++)
			{
				if (Path[i] == '\\')
					LastSlash = &Path[i + 1];
			}
			return LastSlash;
		}

		static wchar_t* RemoveFileExtension(wchar_t* FullFileName, wchar_t* OutputBuffer, DWORD OutputBufferSize)
		{
			wchar_t* LastDot = NULL;
			for (DWORD i = 0; FullFileName[i] != NULL; i++)
				if (FullFileName[i] == '.')
					LastDot = &FullFileName[i];

			for (DWORD j = 0; j < OutputBufferSize; j++)
			{
				OutputBuffer[j] = FullFileName[j];
				if (&FullFileName[j] == LastDot)
				{
					OutputBuffer[j] = NULL;
					break;
				}
			}
			OutputBuffer[OutputBufferSize - 1] = NULL;
			return OutputBuffer;
		}

		static HMODULE __stdcall GetModuleW(_In_opt_ LPCWSTR lpModuleName)
		{
			struct CLIENT_ID
			{
				HANDLE UniqueProcess;
				HANDLE UniqueThread;
			};

			//https://processhacker.sourceforge.io/doc/ntpebteb_8h_source.html#l00166
			struct TEB
			{
				NT_TIB NtTib;
				PVOID EnvironmentPointer;
				CLIENT_ID ClientId;
				PVOID ActiveRpcHandle;
				PVOID ThreadLocalStoragePointer;
				struct PEB* ProcessEnvironmentBlock;
				//...
			};

			//https://processhacker.sourceforge.io/doc/ntpsapi_8h_source.html#l00063
			struct PEB_LDR_DATA
			{
				ULONG Length;
				BOOLEAN Initialized;
				HANDLE SsHandle;
				LIST_ENTRY InLoadOrderModuleList;
				LIST_ENTRY InMemoryOrderModuleList;
				LIST_ENTRY InInitializationOrderModuleList;
				PVOID EntryInProgress;
				BOOLEAN ShutdownInProgress;
				HANDLE ShutdownThreadId;
			};
			//https://processhacker.sourceforge.io/doc/ntpebteb_8h_source.html#l00008
			struct PEB
			{
				BOOLEAN InheritedAddressSpace;
				BOOLEAN ReadImageFileExecOptions;
				BOOLEAN BeingDebugged;
				union
				{
					BOOLEAN BitField;
					struct
					{
						BOOLEAN ImageUsesLargePages : 1;
						BOOLEAN IsProtectedProcess : 1;
						BOOLEAN IsImageDynamicallyRelocated : 1;
						BOOLEAN SkipPatchingUser32Forwarders : 1;
						BOOLEAN IsPackagedProcess : 1;
						BOOLEAN IsAppContainer : 1;
						BOOLEAN IsProtectedProcessLight : 1;
						BOOLEAN SpareBits : 1;
					};
				};
				HANDLE Mutant;
				PVOID ImageBaseAddress;
				PEB_LDR_DATA* Ldr;
				//...
			};
			struct UNICODE_STRING
			{
				USHORT Length;
				USHORT MaximumLength;
				PWCH Buffer;
			};
			//https://processhacker.sourceforge.io/doc/ntldr_8h_source.html#l00102
			struct LDR_DATA_TABLE_ENTRY
			{
				LIST_ENTRY InLoadOrderLinks;
				LIST_ENTRY InMemoryOrderLinks;
				union
				{
					LIST_ENTRY InInitializationOrderLinks;
					LIST_ENTRY InProgressLinks;
				};
				PVOID DllBase;
				PVOID EntryPoint;
				ULONG SizeOfImage;
				UNICODE_STRING FullDllName;
				UNICODE_STRING BaseDllName;
				//...
			};

			PEB* ProcessEnvironmentBlock = ((PEB*)((TEB*)((TEB*)NtCurrentTeb())->ProcessEnvironmentBlock));
			if (lpModuleName == nullptr)
				return (HMODULE)(ProcessEnvironmentBlock->ImageBaseAddress);

			PEB_LDR_DATA* Ldr = ProcessEnvironmentBlock->Ldr;

			LIST_ENTRY* ModuleLists[3] = { 0,0,0 };
			ModuleLists[0] = &Ldr->InLoadOrderModuleList;
			ModuleLists[1] = &Ldr->InMemoryOrderModuleList;
			ModuleLists[2] = &Ldr->InInitializationOrderModuleList;
			for (int j = 0; j < 3; j++)
			{
				for (LIST_ENTRY* pListEntry = ModuleLists[j]->Flink;
					pListEntry != ModuleLists[j];
					pListEntry = pListEntry->Flink)
				{
					LDR_DATA_TABLE_ENTRY* pEntry = (LDR_DATA_TABLE_ENTRY*)((BYTE*)pListEntry - sizeof(LIST_ENTRY) * j); //= CONTAINING_RECORD( pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

					//MessageBoxW(0, pEntry->BaseDllName.Buffer, 0, 0);

					if (_wcsicmp(pEntry->BaseDllName.Buffer, lpModuleName) == 0)
						return (HMODULE)pEntry->DllBase;

					wchar_t* FileName = GetFileNameFromPath(pEntry->FullDllName.Buffer);
					if (!FileName)
						continue;

					if (_wcsicmp(FileName, lpModuleName) == 0)
						return (HMODULE)pEntry->DllBase;

					wchar_t FileNameWithoutExtension[256];
					RemoveFileExtension(FileName, FileNameWithoutExtension, 256);

					if (_wcsicmp(FileNameWithoutExtension, lpModuleName) == 0)
						return (HMODULE)pEntry->DllBase;
				}
			}
			return nullptr;
		}

	public:
		static HMODULE __stdcall ModuleA(_In_opt_ LPCSTR lpModuleName) {
			DWORD ModuleNameLength = (DWORD)strlen(lpModuleName) + 1;

			////allocate buffer for the string on the stack:
			DWORD NewBufferSize = sizeof(wchar_t) * ModuleNameLength;
			wchar_t* W_ModuleName = (wchar_t*)alloca(NewBufferSize);
			for (DWORD i = 0; i < ModuleNameLength; i++)
				W_ModuleName[i] = lpModuleName[i];

			HMODULE hReturnModule = GetModuleW(W_ModuleName);

			RtlSecureZeroMemory(W_ModuleName, NewBufferSize);

			return hReturnModule;
		}
	};
}
