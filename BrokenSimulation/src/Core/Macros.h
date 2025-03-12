#pragma once

#define BS_BIND_EVENT_FN(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

#define BIT(x) (1 << x)
