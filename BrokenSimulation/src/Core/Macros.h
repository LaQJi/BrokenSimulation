#pragma once

#include "Core/Log.h"

#define BS_BIND_EVENT_FN(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

#define BIT(x) (1 << x)

#ifdef _DEBUG
	#define BS_CORE_ASSERT(x, ...) { if(!(x)) { BS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); assert(x); } }
	#define BS_ASSERT(x, ...) { if(!(x)) { BS_ERROR("Assertion Failed: {0}", __VA_ARGS); assert(x); } }
#else
	#define BS_CORE_ASSERT(x, ...)
	#define BS_ASSERT(x, ...)
#endif