//--------------------------------------------------
// Atta File System Tests
// section.cpp
// Date: 2022-06-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <atta/fileSystem/serializer/section.h>

using namespace atta;
using namespace testing;

namespace
{
    TEST(File_Section, Data)
    {
        Section section;
        EXPECT_EQ(section.isUndefined(), true);

        section = int(10);
        EXPECT_EQ(int(section), 10);
        EXPECT_EQ(section.isData(), true);
        EXPECT_EQ(section.isVector(), false);
        EXPECT_EQ(section.isMap(), false);
        EXPECT_EQ(section.isUndefined(), false);

        section = float(20);
        EXPECT_EQ(float(section), 20.0f);

        section = unsigned(30);
        EXPECT_EQ(unsigned(section), 30u);

        // Wrong cast, print error and return 0
        EXPECT_EQ(float(section), 0);

        // Assign vector
        std::vector<int> vec = {0, 1, 2};
        EXPECT_EQ(section.isData(), true);
        section = vec;
        ASSERT_THAT(std::vector<int>(section), ElementsAre(0, 1, 2));

        // Assign initializer list
        section = {1, 2, 3, 4};
        EXPECT_EQ(section.isData(), true);
        ASSERT_THAT(std::vector<int>(section), ElementsAre(1, 2, 3, 4));
    }

    TEST(File_Section, Map)
    {
        Section section;

        // Test section with map of values
        section["myInt"] = 1;
        section["myUnsigned"] = 2u;
        section["myFloat"] = 3.0f;
        EXPECT_EQ(int(section["myInt"]), 1);
        EXPECT_EQ(unsigned(section["myUnsigned"]), 2u);
        EXPECT_EQ(float(section["myFloat"]), 3.0f);
        EXPECT_EQ(section.isMap(), true);

        // Reset section to be a value
        section = 1.0f;
        EXPECT_EQ(float(section), 1.0f);
        EXPECT_EQ(section.isData(), true);

        // Nested section as map of values
        section["child0"]["int"] = 1;
        section["child0"]["float"] = 1.0f;
        section["child1"]["unsigned"] = 1u;
        EXPECT_EQ(section.isMap(), true);

        EXPECT_EQ(int(section["child0"]["int"]), 1);
        EXPECT_EQ(float(section["child0"]["float"]), 1.0f);
        EXPECT_EQ(unsigned(section["child1"]["unsigned"]), 1u);

        // Iterate map 
        std::vector<std::string> keys;
        for(std::pair<std::string, Section> child : section.map())
            keys.push_back(child.first);
        ASSERT_THAT(keys, ElementsAre("child0", "child1"));

        keys.clear();
        for(const std::pair<const std::string, Section>& child : section.map())
            keys.push_back(child.first);
        ASSERT_THAT(keys, ElementsAre("child0", "child1"));

        keys.clear();
        for(auto [key, value] : section.map())
            keys.push_back(key);
        ASSERT_THAT(keys, ElementsAre("child0", "child1"));

        keys.clear();
        for(const auto& [key, value] : section.map())
            keys.push_back(key);
        ASSERT_THAT(keys, ElementsAre("child0", "child1"));
    }

    TEST(File_Section, Vector)
    {
        Section section = std::vector<Section>();
        EXPECT_EQ(section.isVector(), true);
        section = std::vector<Section>();
        EXPECT_EQ(section.isVector(), true);

        // Test section with map of values
        Section child0;
        Section child1;
        child0["test"] = 1;
        child1["test"] = 2;
        section = { child0, child1 };
        EXPECT_EQ(int(section[0]["test"]), 1);
        EXPECT_EQ(int(section[1]["test"]), 2);

        std::vector<int> values;
        for(Section s : section.vector())
            values.push_back(int(s["test"]));
        ASSERT_THAT(values, ElementsAre(1, 2));
    }



    TEST(File_Section, Example)
    {
        Section section;

        // Header section
        section["header"]["version"] = std::string("0.0.0");
        section["header"]["projName"] = std::string("myProject");

        // Component system section
        struct TestComponent
        {
            int test;
        };

        Section testComponent;
        testComponent["entities"] = {1, 2, 3};
        testComponent["components"] = { TestComponent{1}, TestComponent{2}, TestComponent{3} };
        section["componentSystem"]["components"]["testComponent0"] = testComponent;
        section["componentSystem"]["components"]["testComponent1"] = testComponent;

        EXPECT_EQ(section["componentSystem"].size(), 1);
        EXPECT_EQ(section["componentSystem"]["components"].size(), 2);
        EXPECT_EQ(section["componentSystem"]["components"]["testComponent0"].size(), 2);
        ASSERT_THAT(std::vector<int>(section["componentSystem"]["components"]["testComponent0"]["entities"]), ElementsAre(1, 2, 3));
        ASSERT_THAT(std::vector<TestComponent>(section["componentSystem"]["components"]["testComponent1"]["components"])[1].test, 2);

        // Resource system section
        Section material0;
        material0["name"] = std::string("mat0");
        material0["albedo"] = vec3(1.0f, 0.0f, 1.0f);
        material0["roughness"] = 0.5f;
        material0["metallic"] = 0.0f;

        Section material1 = material0;
        material1["name"] = std::string("mat1");
        Section material2 = material0;
        material2["name"] = std::string("mat2");
        Section material3 = material0;
        material3["name"] = std::string("mat3");

        section["resourceSystem"]["materials"] = { material0, material1 };
        section["resourceSystem"]["materials"].push_back(material2);
        section["resourceSystem"]["materials"] += material3;

        EXPECT_EQ(section["resourceSystem"]["materials"].size(), 4);
        EXPECT_EQ(std::string(section["resourceSystem"]["materials"][0]["name"]), "mat0");
        EXPECT_EQ(vec3(section["resourceSystem"]["materials"][0]["albedo"]).x, 1.0f);
        EXPECT_EQ(float(section["resourceSystem"]["materials"][0]["roughness"]), 0.5f);
        EXPECT_EQ(float(section["resourceSystem"]["materials"][0]["metallic"]), 0.0f);
        EXPECT_EQ(std::string(section["resourceSystem"]["materials"][1]["name"]), "mat1");
        EXPECT_EQ(std::string(section["resourceSystem"]["materials"][2]["name"]), "mat2");
        EXPECT_EQ(std::string(section["resourceSystem"]["materials"][3]["name"]), "mat3");

        LOG_DEBUG("FileSystem::tests::section", "Example: [w]$0[]", section);
    }
}
