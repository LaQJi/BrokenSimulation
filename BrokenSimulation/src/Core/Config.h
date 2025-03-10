#pragma once

#ifdef  BS_PLATFORM_WINDOWS
	#ifdef BS_BUILD_DLL
		#define BS_API __declspec(dllexport)
	#else
		#define BS_API __declspec(dllimport)
	#endif
#else
	#error "Broken Simulation only supports Windows!"
#endif
