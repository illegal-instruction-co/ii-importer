#pragma once 

#include "Toolkit.hpp"

#include <Windows.h>
#include <iostream>

namespace ii {
	class StringToolkit final : public Toolkit {
	public:
		static StringToolkit IsReadable(char* str) {
			if (str == nullptr)
				SetAs<bool>(false);
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(str, &mbi, sizeof(mbi));
			SetAs<bool>(mbi.Protect & PAGE_READONLY || mbi.Protect & PAGE_READWRITE);
			return std::move(StringToolkit());
		}

		static StringToolkit IsAlphaNumeric(char* str) {
			if (str == nullptr)
				SetAs<bool>(false);
			int len = sizeof(str);
			for (int i = 0; i < len; i++)
				if (!(isalpha(str[i]) || isdigit(str[i]) && (str[i] >= 'A' && str[i] <= 'Z' || str[i] >= 'a' && str[i] <= 'z')) || str[i] < 0 || str[i] > 127)
					SetAs<bool>(false);

			SetAs<bool>(true);

			return std::move(StringToolkit());
		}
	};
}
