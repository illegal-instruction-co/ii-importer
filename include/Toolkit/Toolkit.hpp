#pragma once

#include <iostream>

namespace ii {
	class Toolkit {
	public:
		template<typename T>
		__forceinline static T GetAs() noexcept {
			return (T)(g_result);
		}

	protected:
		template<typename T>
		__forceinline static void SetAs(T result) noexcept {
			g_result = (uint64_t)result;
		}

	private:
		inline static uint64_t g_result = 0;
	};
}
