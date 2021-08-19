//--------------------------------------------------
// Atta Core Tests
// stringId.cpp
// Date: 2021-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/core/stringId.h>

using namespace atta;

TEST(Core_StringId, ConstexprLiteral)
{
	EXPECT_EQ(SID("test string"), "test string"_id);
	EXPECT_NE(SID("test"), SID("string"));
	EXPECT_NE("test"_id, "string"_id);
}

TEST(Core_StringId, StringTable)
{
	StringId sid0 {"test0"};
	StringId sid1 {"test1"};

	EXPECT_EQ(sid0.getId(), SID("test0"));
	EXPECT_NE(sid0.getId(), SID("test1"));

	EXPECT_EQ(sid1.getId(), SID("test1"));
	EXPECT_NE(sid1.getId(), SID("test0"));

	EXPECT_NE(sid0.getId(), sid1.getId());
	EXPECT_NE(sid0.getString(), sid1.getString());

	EXPECT_EQ(sid0.getString(), std::string("test0"));
	EXPECT_EQ(sid1.getString(), std::string("test1"));
}
