#pragma once

#include <iostream>

namespace ii {

	uint64_t g_toolkitResult;

	class Toolkit {
	public:
		template<typename T>
		__forceinline static T GetAs() noexcept {
			return (T)(g_toolkitResult);
		}

	protected:
		template<typename T>
		__forceinline static void SetAs(T result) noexcept {
			g_toolkitResult = (uint64_t)result;
		}
	};
}
