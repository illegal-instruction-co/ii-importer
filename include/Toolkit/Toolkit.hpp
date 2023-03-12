#pragma once

#include <cstdint>

namespace ii {
	class Toolkit {
	public:
		template<typename T>
		__forceinline static T GetAs() noexcept {
			return static_cast<T>(g_result);
		}

	protected:
		template<typename T>
		__forceinline static void SetAs(T result) noexcept {
			g_result = static_cast<uint64_t>(result);
		}

	private:
		inline static uint64_t g_result = 0;
	};
}
