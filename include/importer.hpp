#pragma once 

#include "Toolkit/StringToolkit.hpp"
#include "Toolkit/CustomAPI.hpp"

#include <functional>
#include <string_view>
#include <map>
#include <iostream>

namespace ii {
	class Importer final {

	public:
		Importer(const std::string_view& mod) : m_moduleName(mod), m_module(CustomAPI::ModuleA(mod.data())) {

			if (!m_module)
				throw std::runtime_error(std::string("We could not export from module, because it was not loaded. Module: ") + m_moduleName.data());

			m_dosHeader = (PIMAGE_DOS_HEADER)m_module;

			if (m_dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
				throw std::runtime_error(std::string("Dos header is wrong of module ") + m_moduleName.data());

			m_ntHeader = (PIMAGE_NT_HEADERS)((LPBYTE)m_module + m_dosHeader->e_lfanew);

			if (m_ntHeader->Signature != IMAGE_NT_SIGNATURE)
				throw std::runtime_error(std::string("Could not found nt header for ") + m_moduleName.data());

			if (m_ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == 0)
				throw std::runtime_error(std::string("Export directory could not read for ") + m_moduleName.data());

			m_exportDirectory = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)m_module + m_ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

			m_fetchedFunctions = Fetch();

			if (!m_fetchedFunctions.size())
				throw std::runtime_error(std::string("Export directory was empty for ") + m_moduleName.data());
		}

		template<typename T>
		__forceinline auto Invoke(const char* fn) {

			auto it = m_fetchedFunctions.find(fn);
			if (it == m_fetchedFunctions.end())
				throw std::runtime_error(std::string("Function ") + fn + " not found in module " + m_moduleName.data());

			return [&](...) {
				using fn_t = T(WINAPI*)(...);
				return (fn_t)it->second;
			}();
		}

	private:

		__forceinline std::map<std::string, uint64_t> Fetch() {
			std::map<std::string, uint64_t> res;

			PDWORD addr, name;
			PWORD ordinal;
			addr = (PDWORD)((LPBYTE)m_module + m_exportDirectory->AddressOfFunctions);
			name = (PDWORD)((LPBYTE)m_module + m_exportDirectory->AddressOfNames);

			ordinal = (PWORD)((LPBYTE)m_module + m_exportDirectory->AddressOfNameOrdinals);

			for (int i = 0; i < m_exportDirectory->AddressOfFunctions; i++) {
				if (!StringToolkit::IsReadable((char*)m_module + name[i]).GetAs<bool>())
					return res;
				if (StringToolkit::IsAlphaNumeric((char*)m_module + name[i]).GetAs<bool>())
					res.insert({ std::move(std::string((char*)m_module + name[i])), (uint64_t)m_module + addr[ordinal[i]] });
			}

			return res;
		}

		PIMAGE_DOS_HEADER m_dosHeader;
		PIMAGE_NT_HEADERS m_ntHeader;
		PIMAGE_EXPORT_DIRECTORY m_exportDirectory;

		HANDLE m_module = 0;
		std::map<std::string, uint64_t> m_fetchedFunctions;
		const std::string_view m_moduleName;
	};
}
