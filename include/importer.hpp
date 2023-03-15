/*
* MIT License
*
* Copyright (c) 2023 illegal instruction
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma optimize("", on) // set optimization to full
#pragma comment(linker,"/SECTION:.ii,RE") // set section to read and execute
#pragma code_seg(push, ".ii") // set code segment to .ii
#pragma inline_depth(255) // set inline depth to max

#pragma once 

#ifndef AY_OBFUSCATE_DEFAULT_KEY
#define AY_OBFUSCATE_DEFAULT_KEY ay::generate_key(AY_LINE * __TIME__[1] * __TIME__[2] * __TIME__[3]) // generate key from line and time
#endif

#include "Toolkit/Obfuscate.h"
#include "Toolkit/CustomAPI.hpp"

#include "Error.hpp"

#include <stdexcept>
#include <functional>
#include <string_view>
#include <unordered_map>

namespace ii {
	class Importer final {

	public:
		Importer(const std::string_view& mod) : m_moduleName(mod), m_module(CustomAPI::ModuleA(mod.data())) {

			if (!m_module)
				throw std::runtime_error(err::runtime1 + m_moduleName.data());

			m_dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(m_module);

			if (m_dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
				throw std::runtime_error(err::runtime2 + m_moduleName.data());

			m_ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<LPBYTE>(m_module) + m_dosHeader->e_lfanew);

			if (m_ntHeader->Signature != IMAGE_NT_SIGNATURE)
				throw std::runtime_error(err::runtime3 + m_moduleName.data());

			if (m_ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == 0)
				throw std::runtime_error(err::runtime4 + m_moduleName.data());

			m_exportDirectory = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(reinterpret_cast<LPBYTE>(m_module) + m_ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

			m_fetchedFunctions = Fetch();

			if (m_fetchedFunctions.empty())
				throw std::runtime_error(err::runtime5 + m_moduleName.data());
		}

		template<typename T>
		__forceinline auto Invoke(const char* fn) {
			auto it = m_fetchedFunctions.find(fn);
			if (it == m_fetchedFunctions.end())
				throw std::runtime_error(fn + err::runtime6 + m_moduleName.data());

			return reinterpret_cast<T(__stdcall*)(...)>(it->second);
		}

	private:

		std::unordered_map<std::string_view, uint64_t> Fetch() {

			std::unordered_map<std::string_view, uint64_t> res;
			PDWORD addr, name;
			PWORD ordinal;

			addr = reinterpret_cast<PDWORD>(reinterpret_cast<LPBYTE>(m_module) + m_exportDirectory->AddressOfFunctions);
			name = reinterpret_cast<PDWORD>(reinterpret_cast<LPBYTE>(m_module) + m_exportDirectory->AddressOfNames);
			ordinal = reinterpret_cast<PWORD>(reinterpret_cast<LPBYTE>(m_module) + m_exportDirectory->AddressOfNameOrdinals);

			for (int i = 0; i < m_exportDirectory->NumberOfNames; i++)
				res.insert({ std::move(std::string_view(static_cast<char*>(m_module) + name[i])), (uint64_t)m_module + addr[ordinal[i]] });

			return res;
		}

	private:

		PIMAGE_DOS_HEADER m_dosHeader;

		PIMAGE_NT_HEADERS m_ntHeader;

		PIMAGE_EXPORT_DIRECTORY m_exportDirectory;

		HANDLE m_module = 0;

		std::unordered_map<std::string_view, uint64_t> m_fetchedFunctions;

		const std::string_view m_moduleName;
	};
}

#ifndef II_IMPORTER
#define II_IMPORTER(name) ii::Importer(std::string_view(AY_OBFUSCATE(name)))
#endif

#ifndef II_STRING
#define II_STRING AY_OBFUSCATE
#endif
