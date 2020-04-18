#pragma once

#ifdef DEBUG
#define LOG(x) cout << x
#else
#define LOG(x) \
  do {         \
  } while (0)
#endif
