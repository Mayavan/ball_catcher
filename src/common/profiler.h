#pragma once
#include <chrono>

#define IN_uS(x) std::chrono::duration_cast<std::chrono::microseconds>(x).count()
#define IN_MS(x) std::chrono::duration_cast<std::chrono::microseconds>(x).count()/1000.0
#define GET_TIME() std::chrono::high_resolution_clock::now()