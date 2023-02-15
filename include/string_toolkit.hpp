#pragma once 

#include "toolkit.hpp"
#include <Windows.h>
#include <iostream>

namespace ii {
	class StringToolkit : public Toolkit {

	public:

		__forceinline static bool IsReadable(char* str) {
			if (str == NULL)
				return false;
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(str, &mbi, sizeof(mbi));
			return mbi.Protect & PAGE_READONLY || mbi.Protect & PAGE_READWRITE;
		}

		__forceinline static bool IsAlphaNumeric(char* str) {
			if (str == NULL)
				return false;
			int len = sizeof(str);
			for (int i = 0; i < len; i++) {
				if (!(isalpha(str[i]) || isdigit(str[i]) && (str[i] >= 'A' && str[i] <= 'Z' || str[i] >= 'a' && str[i] <= 'z')) || str[i] < 0 || str[i] > 127)
					return false;
			}
			return true;
		}
	};
}
