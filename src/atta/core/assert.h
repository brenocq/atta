//--------------------------------------------------
// Atta Assertion
// assert.h
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CORE_ASSERT_H
#define ATTA_CORE_ASSERT_H

namespace atta
{

// Assert without error message
#define ASSERT_1ARG(x) { if(!(x)) { LOG_ERROR("Assert", "Failed assert at file [w]$0[], line [w]$1[]", __FILE__, __LINE__); exit(EXIT_FAILURE);} }
// Assert with error message
#define ASSERT_2ARGS(x, msg) { if(!(x)) { LOG_ERROR("Assert", "Failed assert at file [w]$0[], line [w]$1[]. $2", __FILE__, __LINE__, msg); exit(EXIT_FAILURE);} }
// Check number of arguments and select which assert to call
#define ASSERT_SELECT(_1, _2, NAME, ...) NAME
#define ASSERT(...) ASSERT_SELECT(__VA_ARGS__, ASSERT_2ARGS, ASSERT_1ARG)(__VA_ARGS__)

// Assert when using debug build
#ifdef ATTA_DEBUG_BUILD
#define DASSERT(...) ASSERT(__VA_ARGS__)
#else
#define DASSERT(...) 
#endif

}
#endif// ATTA_CORE_ASSERT_H
