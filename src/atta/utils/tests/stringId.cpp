//--------------------------------------------------
// Atta Utils Tests
// stringId.cpp
// Date: 2021-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/stringId.h>
#include <gtest/gtest.h>

using namespace atta;

namespace {
TEST(Utils_StringId, ConstexprLiteral) {
    EXPECT_EQ(SID("test string"), "test string"_sid);
    EXPECT_NE(SID("test"), SID("string"));
    EXPECT_NE("test"_sid, "string"_sid);
}

TEST(Utils_StringId, StringTable) {
    StringId sid0{"test0"};
    StringId sid1{"test1"};

    EXPECT_EQ(sid0.getId(), SID("test0"));
    EXPECT_NE(sid0.getId(), SID("test1"));

    EXPECT_EQ(sid1.getId(), SID("test1"));
    EXPECT_NE(sid1.getId(), SID("test0"));

    EXPECT_NE(sid0.getId(), sid1.getId());
    EXPECT_NE(sid0.getString(), sid1.getString());

    EXPECT_EQ(sid0.getString(), std::string("test0"));
    EXPECT_EQ(sid1.getString(), std::string("test1"));
}
} // namespace
