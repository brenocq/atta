//--------------------------------------------------
// Atta File Module Tests
// section.cpp
// Date: 2022-06-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/section.h>
#include <atta/utils/stringId.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace atta;
using namespace atta::file;
using namespace testing;

namespace {

TEST(File_Section, SectionData_ValueToString) {
    SectionData data = 10;
    EXPECT_EQ(data.getStr(), "10");
    data = true;
    EXPECT_EQ(data.getStr(), "true");
    data = "test";
    EXPECT_EQ(data.getStr(), "\"test\"");
    data = std::string("test str");
    EXPECT_EQ(data.getStr(), "\"test str\"");
    data = 10.0f;
    EXPECT_EQ(data.getStr(), std::to_string(10.0f));
    data = 10.0;
    EXPECT_EQ(data.getStr(), std::to_string(10.0));
    // Test std::vector
    data = std::vector<int>{0, 1, 2};
    EXPECT_EQ(data.getStr(), "{0, 1, 2}");
    data = std::vector<std::string>{"one", "two", "three"};
    EXPECT_EQ(data.getStr(), "{\"one\", \"two\", \"three\"}");
    data = std::vector<const char*>{"one", "two", "three"};
    EXPECT_EQ(data.getStr(), "{\"one\", \"two\", \"three\"}");
    // Test std::initializer_list
    data = {0, 1, 2};
    EXPECT_EQ(data.getStr(), "{0, 1, 2}");
    data = {"one", "two", "three"};
    EXPECT_EQ(data.getStr(), "{\"one\", \"two\", \"three\"}");
    data = {true, false};
    EXPECT_EQ(data.getStr(), "{true, false}");
    data = vec2(1.0f, 2.0f);
    EXPECT_EQ(data.getStr(), "vector2(" + std::to_string(1.0f) + ", " + std::to_string(2.0f) + ")");
    data = vec3(1.0f, 2.0f, 3.0f);
    EXPECT_EQ(data.getStr(), "vector3(" + std::to_string(1.0f) + ", " + std::to_string(2.0f) + ", " + std::to_string(3.0f) + ")");
    data = vec4(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_EQ(data.getStr(),
              "vector4(" + std::to_string(1.0f) + ", " + std::to_string(2.0f) + ", " + std::to_string(3.0f) + ", " + std::to_string(4.0f) + ")");
    data = quat(0.7071f, 0.7071f, 0.0f, 0.0f); // TODO quat constructor should be quat(i, j, k, r)
    EXPECT_EQ(data.getStr(), "quaternion(" + std::to_string(0.7071f) + ", " + std::to_string(0.0f) + ", " + std::to_string(0.0f) + ", " +
                                 std::to_string(0.7071f) + ")");
}

TEST(File_Section, SectionData_StringToValue) {}

TEST(File_Section, Section_ToString) {}

TEST(File_Section, Section_ToValue) {}

} // namespace
