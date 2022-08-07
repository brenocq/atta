//--------------------------------------------------
// Atta Assertion
// assert.h
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CORE_ASSERT_H
#define ATTA_CORE_ASSERT_H
#include <assert.h>

namespace atta {

// Assert without error message
#define ASSERT_1_ARG(x)                                                                                                                              \
    {                                                                                                                                                \
        if (!(x)) {                                                                                                                                  \
            LOG_ERROR("Assert", "Failed assert at file [w]$0[], line [w]$1[]", __FILE__, __LINE__);                                                  \
            assert(false);                                                                                                                           \
        }                                                                                                                                            \
    }

//LOG_ERROR("Assert", "Test Assert 1 arg $0", x); \
// Assert with error message
#define ASSERT_W_ARGS(x, ...)                                                                                                                        \
    {                                                                                                                                                \
        if (!(x)) {                                                                                                                                  \
            LOG_ERROR("Assert", "Failed assert at file [w]$0[], line [w]$1[]", __FILE__, __LINE__);                                                  \
            LOG_ERROR("Assert", __VA_ARGS__);                                                                                                        \
            assert(false);                                                                                                                           \
        }                                                                                                                                            \
    }

//LOG_ERROR("Assert", "Test Assert w args $0", x); \
// Check number of arguments and select which assert to call (up to 9 arguments)
#define ASSERT_SELECT(_1, _2, _3, _4, _5, _6, _7, _8, _9, NAME, ...) NAME
#define ASSERT(...)                                                                                                                                  \
    ASSERT_SELECT(__VA_ARGS__, ASSERT_W_ARGS, ASSERT_W_ARGS, ASSERT_W_ARGS, ASSERT_W_ARGS, ASSERT_W_ARGS, ASSERT_W_ARGS, ASSERT_W_ARGS,              \
                  ASSERT_W_ARGS, ASSERT_W_ARGS, ASSERT_1_ARG)                                                                                        \
    (__VA_ARGS__)

// Assert when using debug build
#ifdef ATTA_DEBUG_BUILD
#define DASSERT(...) ASSERT(__VA_ARGS__)
#else
#define DASSERT(...)
#endif

} // namespace atta
#endif // ATTA_CORE_ASSERT_H
