//--------------------------------------------------
// Atta File Module Tests
// serializer.cpp
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

TEST(File_Serializer, SectionData_ValueToString) {
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
    // Test custom atta types
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

TEST(File_Serializer, SectionData_StringToValue) {
    SectionData data = 10;
    EXPECT_EQ(int(data), 10);
    data = 10.0f;
    EXPECT_EQ(float(data), 10.0f);
    data = 10.0;
    EXPECT_EQ(double(data), 10.0);
    data = true;
    EXPECT_EQ(bool(data), true);
    data = false;
    EXPECT_EQ(bool(data), false);
    data = "test";
    EXPECT_EQ(std::string(data), "test");
    data = std::string("test");
    EXPECT_EQ(std::string(data), "test");
    // Test custom atta types
    data = vec2(1.0f, 2.0f);
    EXPECT_EQ(vec2(data), vec2(1.0f, 2.0f));
    data = vec3(1.0f, 2.0f, 3.0f);
    EXPECT_EQ(vec3(data), vec3(1.0f, 2.0f, 3.0f));
    data = vec4(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_EQ(vec4(data), vec4(1.0f, 2.0f, 3.0f, 4.0f));
    data = quat(0.7071f, 0.7071f, 0.0f, 0.0f);
    EXPECT_EQ(quat(data), quat(0.7071f, 0.7071f, 0.0f, 0.0f));
    // Test std::vector
    data = std::vector<int>{0, 1, 2};
    EXPECT_THAT(std::vector<int>(data), ElementsAre(0, 1, 2));
    data = std::vector<bool>{true, false, true};
    EXPECT_THAT(std::vector<bool>(data), ElementsAre(true, false, true));
    data = std::vector<std::string>{"one", "two", "three"};
    EXPECT_THAT(std::vector<std::string>(data), ElementsAre("one", "two", "three"));
    data = std::vector<vec2>{vec2(0.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 0.0f)};
    EXPECT_THAT(std::vector<vec2>(data), ElementsAre(vec2(0.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 0.0f)));

    // Test vector of vectors
    std::vector<std::vector<vec2>> expected = {std::vector<vec2>{vec2(0.0f, 0.0f), vec2(0.0f, 1.0f)},
                                               std::vector<vec2>{vec2(1.0f, 0.0f), vec2(1.0f, 0.0f)}};
    data = expected;
    std::vector<std::vector<vec2>> v = std::vector<std::vector<vec2>>(data);
    for (size_t i = 0; i < v.size(); i++)
        EXPECT_THAT(v[i], ElementsAreArray(expected[i]));
}

TEST(File_Serializer, Section_ToString) {
    Section section("section");
    section["key1"] = 10;
    section["key2"] = true;
    section["key3"] = "test";
    EXPECT_EQ(section.toString(), "[section]\nkey1 = 10\nkey2 = true\nkey3 = \"test\"\n");
}

TEST(File_Serializer, Section_ToValue) {}

TEST(File_Serializer, Serializer_Serialize) {}

TEST(File_Serializer, Serializer_Deserialize) {}

} // namespace
