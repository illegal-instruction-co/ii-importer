#pragma once 

#include "string_toolkit.hpp"
#include "custom_api.hpp"
#include <functional>
#include <string_view>
#include <map>
#include <iostream>

namespace ii {
	namespace winthreads {
		namespace core {
			class exporter final {

			public:

				exporter(const std::string_view& mod) noexcept : m_moduleName(std::move(mod)) {
					m_module = (HANDLE)CustomAPI::GetModuleA(m_moduleName.data());
					
					if (!m_module)
						printf("We cannot export from %s, because it is not already loaded.", m_moduleName.data()),
						abort();

					m_dosHeader = (PIMAGE_DOS_HEADER)m_module;
					
					if (m_dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
						printf("Dos header is wrong of module %s", m_moduleName.data()), 
						abort();

					m_ntHeader = (PIMAGE_NT_HEADERS)((LPBYTE)m_module + m_dosHeader->e_lfanew);

					if (m_ntHeader->Signature != IMAGE_NT_SIGNATURE)
						printf("Could not found nt header for %s", m_moduleName.data()), 
						abort;

					if (m_ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == 0)
						printf("Export directory could not read for %s", m_moduleName);

					m_exportDirectory = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)m_module + m_ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

					m_fetchedFunctions = fetch();
					if (!m_fetchedFunctions.size())
						printf("Export directory was empty for %s", m_moduleName.data()),
						abort();
				}

				virtual ~exporter() {}

				__forceinline auto invoke(const char* fn) {

					return [&](...) {
						using fn_t = void* (WINAPI*)(...);
						return (fn_t)m_fetchedFunctions.find(fn)->second;
					}();
				}

			private:

				__forceinline std::map<std::string, __int64> fetch() {
					std::map<std::string, __int64> res;

					PDWORD addr, name;
					PWORD ordinal;
					addr = (PDWORD)((LPBYTE)m_module + m_exportDirectory->AddressOfFunctions);
					name = (PDWORD)((LPBYTE)m_module + m_exportDirectory->AddressOfNames);

					ordinal = (PWORD)((LPBYTE)m_module + m_exportDirectory->AddressOfNameOrdinals);

					for (int i = 0; i < m_exportDirectory->AddressOfFunctions; i++) {
						if (!string_toolkit::isReadable((char*)m_module + name[i]))
							return res;
						if (string_toolkit::isAlphaNumeric((char*)m_module + name[i]))
							res.insert({ std::move(std::string((char*)m_module + name[i])), (__int64)m_module + addr[ordinal[i]] });
					}

					return res;
				}

				PIMAGE_DOS_HEADER m_dosHeader;
				PIMAGE_NT_HEADERS m_ntHeader;
				PIMAGE_EXPORT_DIRECTORY m_exportDirectory;

				HANDLE m_module = 0;
				std::map<std::string, __int64> m_fetchedFunctions;
				const std::string_view m_moduleName;
			};
		}
	}
}