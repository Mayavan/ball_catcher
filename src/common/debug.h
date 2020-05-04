#pragma once
#include <iostream>

#ifdef DEBUG
#define LOG_DEBUG(x) std::cout << x << std::endl
#else
#define LOG_DEBUG(x) \
  do {               \
  } while (0)
#endif

#define LOG_INFO(x) std::cout << x << std::endl
#define LOG_ERROR(x) std::cerr << x << std::endl


#ifdef DEBUG
#define IF_DEBUG(x) x
#else
#define IF_DEBUG(x) \
  do {              \
  } while (0)
#endif