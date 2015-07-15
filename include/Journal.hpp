#include <cassert>

#ifndef NDEBUG
  #define ASSERT(expr, msg) { \
    if (!expr) {              \
      std::cout << msg;       \
      assert(false);          \
    }                         \
  }
#else
  #define ASSERT(expr, msg) (void*)0;
#endif

#define LOG(flag) \
    if (flag) std::cout << "(" << __FILE__ << ":" << __LINE__ << "): "
