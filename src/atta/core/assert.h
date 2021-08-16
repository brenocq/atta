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

#ifdef ATTA_DEBUG_BUILD

#define ATTA_ASSERT(x) { if(!(x)) { LOG_ERROR("Assert", "Failed assert at file [w]$0[], line [w]$1[]", __FILE__, __LINE__); exit(EXIT_FAILURE);} }

#else

#define ATTA_ASSERT(x)

#endif

}
#endif// ATTA_CORE_ASSERT_H
