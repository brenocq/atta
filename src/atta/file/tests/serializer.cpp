//--------------------------------------------------
// Atta File Module Tests
// serializer.cpp
// Date: 2022-06-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/section.h>
#include <atta/file/serializer/serializer.h>
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
    EXPECT_EQ(data.getStr(), "vec2(" + std::to_string(1.0f) + ", " + std::to_string(2.0f) + ")");
    data = vec3(1.0f, 2.0f, 3.0f);
    EXPECT_EQ(data.getStr(), "vec3(" + std::to_string(1.0f) + ", " + std::to_string(2.0f) + ", " + std::to_string(3.0f) + ")");
    data = vec4(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_EQ(data.getStr(),
              "vec4(" + std::to_string(1.0f) + ", " + std::to_string(2.0f) + ", " + std::to_string(3.0f) + ", " + std::to_string(4.0f) + ")");
    data = quat(0.7071f, 0.7071f, 0.0f, 0.0f); // TODO quat constructor should be quat(i, j, k, r)
    EXPECT_EQ(data.getStr(),
              "quat(" + std::to_string(0.7071f) + ", " + std::to_string(0.0f) + ", " + std::to_string(0.0f) + ", " + std::to_string(0.7071f) + ")");
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

TEST(File_Serializer, Section_InsertFromString) {
    Section section("test_section");

    // Valid input strings
    section.insertFromString("key1 = \"someString\"");
    section.insertFromString("key2 = 10");
    section.insertFromString("key3 = true");

    // Verify the values by casting to their expected types
    EXPECT_TRUE(section.contains("key1"));
    EXPECT_EQ(std::string(section["key1"]), "someString");

    EXPECT_TRUE(section.contains("key2"));
    EXPECT_EQ(int(section["key2"]), 10);

    EXPECT_TRUE(section.contains("key3"));
    EXPECT_EQ(bool(section["key3"]), true);

    // Invalid input strings
    section.insertFromString("key4 =");
    EXPECT_FALSE(section.contains("key4")); // No value after '='

    section.insertFromString("= valueWithoutKey");
    EXPECT_FALSE(section.contains("")); // No key before '='

    section.insertFromString("key5 valueWithoutEquals");
    EXPECT_FALSE(section.contains("key5")); // Missing '=' delimiter

    // Strings with extra spaces and tabs
    section.insertFromString("   key6    =    \"trimmedString\"   ");
    EXPECT_TRUE(section.contains("key6"));
    EXPECT_EQ(std::string(section["key6"]), "trimmedString");

    section.insertFromString("\t\tkey7\t=\t42\t");
    EXPECT_TRUE(section.contains("key7"));
    EXPECT_EQ(int(section["key7"]), 42);

    // Strings with newlines or empty input
    section.insertFromString("key8 = valueWithNewline\n"); // Invalid string input
    EXPECT_TRUE(section.contains("key8"));
    EXPECT_EQ(std::string(section["key8"]), "");

    section.insertFromString("");
    EXPECT_FALSE(section.contains("")); // Empty string, nothing to add

    section.insertFromString("   ");
    EXPECT_FALSE(section.contains("   ")); // Only whitespace, nothing to add

    // Test for overwriting existing keys
    section.insertFromString("key1 = \"newValue\"");
    EXPECT_EQ(std::string(section["key1"]), "newValue"); // Overwrite existing value

    // Complex types
    section.insertFromString("key9.vec2 = vec2(1.0, 2.0)");
    EXPECT_TRUE(section.contains("key9.vec2"));
    EXPECT_EQ(vec2(section["key9.vec2"]), vec2(1.0f, 2.0f));

    section.insertFromString("key10.vec3 = vec3(1.0, 2.0, 3.0)");
    EXPECT_TRUE(section.contains("key10.vec3"));
    EXPECT_EQ(vec3(section["key10.vec3"]), vec3(1.0f, 2.0f, 3.0f));

    section.insertFromString("key11.vec4 = vec4(1.0, 2.0, 3.0, 4.0)");
    EXPECT_TRUE(section.contains("key11.vec4"));
    EXPECT_EQ(vec4(section["key11.vec4"]), vec4(1.0f, 2.0f, 3.0f, 4.0f));

    section.insertFromString("key12.quat = quat(0.7071, 0.0, 0.0, 0.7071)");
    EXPECT_TRUE(section.contains("key12.quat"));
    EXPECT_EQ(quat(section["key12.quat"]), quat(0.7071f, 0.7071f, 0.0f, 0.0f));

    // Vector of elements
    section.insertFromString("key13.vectorInt = {1, 2, 3}");
    EXPECT_TRUE(section.contains("key13.vectorInt"));
    EXPECT_THAT(std::vector<int>(section["key13.vectorInt"]), ElementsAre(1, 2, 3));

    section.insertFromString("key14.vectorVec2 = {vec2(1.0, 2.0), vec2(3.0, 4.0)}");
    EXPECT_TRUE(section.contains("key14.vectorVec2"));
    EXPECT_THAT(std::vector<vec2>(section["key14.vectorVec2"]), ElementsAre(vec2(1.0f, 2.0f), vec2(3.0f, 4.0f)));
}

TEST(File_Serializer, Serializer_Serialize) {
    Serializer serializer;
    Section s0("section0");
    s0["key1"] = 10;
    s0["key2"] = true;
    s0["key3"] = "test";
    serializer.addSection(s0);

    Section s1("section1");
    s1["key1.vec2"] = vec2(1.0f, 2.0f);
    s1["key2.vec3"] = vec3(1.0f, 2.0f, 3.0f);
    s1["key3.vec4"] = vec4(1.0f, 2.0f, 3.0f, 4.0f);
    s1["key4.quat"] = quat(0.7071f, 0.7071f, 0.0f, 0.0f);
    serializer.addSection(s1);

    Section s2("section2");
    s2["key5.vectorInt"] = std::vector<int>{0, 1, 2};
    s2["key6.vectorStr"] = std::vector<std::string>{"one", "two", "three"};
    s2["key7.vectorVec2"] = std::vector<vec2>{vec2(0.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 0.0f)};
    s2["key8.vectorVectorVec2"] =
        std::vector<std::vector<vec2>>{std::vector<vec2>{vec2(0.0f, 0.0f), vec2(0.0f, 1.0f)}, std::vector<vec2>{vec2(1.0f, 0.0f), vec2(1.0f, 0.0f)}};
    serializer.addSection(s2);

    // Construct the expected serialized output
    std::string expected = "[section0]\n"
                           "key1 = 10\n"
                           "key2 = true\n"
                           "key3 = \"test\"\n"
                           "\n"
                           "[section1]\n"
                           "key1.vec2 = vec2(1.000000, 2.000000)\n"
                           "key2.vec3 = vec3(1.000000, 2.000000, 3.000000)\n"
                           "key3.vec4 = vec4(1.000000, 2.000000, 3.000000, 4.000000)\n"
                           "key4.quat = quat(0.707100, 0.000000, 0.000000, 0.707100)\n"
                           "\n"
                           "[section2]\n"
                           "key5.vectorInt = {0, 1, 2}\n"
                           "key6.vectorStr = {\"one\", \"two\", \"three\"}\n"
                           "key7.vectorVec2 = {vec2(0.000000, 0.000000), vec2(0.000000, 1.000000), vec2(1.000000, 0.000000)}\n"
                           "key8.vectorVectorVec2 = {{vec2(0.000000, 0.000000), vec2(0.000000, 1.000000)}, {vec2(1.000000, 0.000000), vec2(1.000000, "
                           "0.000000)}}\n"
                           "\n";

    // Compare the serialized output with the expected string
    EXPECT_EQ(serializer.toString(), expected);
}

TEST(File_Serializer, Serializer_Deserialize) {
    Serializer serializer;

    // Input string with valid sections, extra spaces, and invalid lines
    std::string input = R"(
        [section1]
        key1 = 10
        key2 = true

        key3 = "stringValue"
        key4 = invalidString
        = 10

        [section2]
        key1.vec2 = vec2(1.0, 2.0)
        key2.vec3 =  vec3(1.0 , 2.0 , 3.0)
        key3.vec4 = vec4(1.0, 2.0, 3.0, 4.0)
        invalid_line_without_equals
        another_invalid_line

        [section3]
        key1.vectorInt = { 1, 2, 3}
        key2.vectorStr = {"one", "two", "three"}
        key3.quat = quat(0.7071, 0.0, 0.0, 0.7071)
    )";

    serializer.fromString(input);

    const auto& sections = serializer.getSections();
    ASSERT_EQ(sections.size(), 3);

    // Validate section1
    const auto& section1 = sections[0];
    EXPECT_EQ(section1.getName(), "section1");
    EXPECT_EQ(section1.size(), 4);
    EXPECT_EQ(int(section1["key1"]), 10);
    EXPECT_EQ(bool(section1["key2"]), true);
    EXPECT_EQ(std::string(section1["key3"]), "stringValue");
    EXPECT_TRUE(section1.contains("key4"));
    EXPECT_EQ(std::string(section1["key4"]), "");

    // Validate section2
    const auto& section2 = sections[1];
    EXPECT_EQ(section2.getName(), "section2");
    EXPECT_EQ(section2.size(), 3);
    EXPECT_EQ(vec2(section2["key1.vec2"]), vec2(1.0f, 2.0f));
    EXPECT_EQ(vec3(section2["key2.vec3"]), vec3(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(vec4(section2["key3.vec4"]), vec4(1.0f, 2.0f, 3.0f, 4.0f));

    // Validate section3
    const auto& section3 = sections[2];
    EXPECT_EQ(section3.getName(), "section3");
    EXPECT_EQ(section3.size(), 3);
    EXPECT_THAT(std::vector<int>(section3["key1.vectorInt"]), ElementsAre(1, 2, 3));
    EXPECT_THAT(std::vector<std::string>(section3["key2.vectorStr"]), ElementsAre("one", "two", "three"));
    EXPECT_EQ(quat(section3["key3.quat"]), quat(0.7071f, 0.7071f, 0.0f, 0.0f));
}

} // namespace
