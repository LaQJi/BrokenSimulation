#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cmath>
#include <cassert>
#include <chrono>

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <array>
#include <set>
#include <tuple>
#include <map>
#include <unordered_map>

#ifdef BS_PLATFORM_WINDOWS
	#include <Windows.h>
#else
	#error "Broken Simulation only supports Windows!"
#endif