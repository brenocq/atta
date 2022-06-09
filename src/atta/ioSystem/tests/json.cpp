//--------------------------------------------------
// Atta File System Tests
// section.cpp
// Date: 2022-06-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <atta/ioSystem/http/json.h>

using namespace atta;
using namespace testing;

namespace
{
    TEST(IO_Json, Parse_Null)
    {
        io::Json json;
        std::string j = R"(
            null
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::NONE);

        // Broken
        j = R"(
            nu
        )";
        EXPECT_EQ(json.parse(j), false);

        // Broken
        j = R"(
            nulll
        )";
        EXPECT_EQ(json.parse(j), false);
    }

    TEST(IO_Json, Parse_Bool)
    {
        io::Json json;
        std::string j = R"(
            false
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::BOOL);
        EXPECT_EQ(bool(json), false);

        j = R"(
            true
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::BOOL);
        EXPECT_EQ(bool(json), true);

        j = R"(
            truee
        )";
        EXPECT_EQ(json.parse(j), false);

        j = R"(
            False
        )";
        EXPECT_EQ(json.parse(j), false);
    }

    TEST(IO_Json, Parse_Int)
    {
        io::Json json;
        std::string j = R"(
            10
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::INT);
        EXPECT_EQ(int(json), 10);

        j = R"(
            -2
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::INT);
        EXPECT_EQ(int(json), -2);
    }

    TEST(IO_Json, Parse_Float)
    {
        io::Json json;
        std::string j = R"(
            10.0
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::FLOAT);
        EXPECT_EQ(float(json), 10.0f);

        j = R"(
            -2.5
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::FLOAT);
        EXPECT_EQ(float(json), -2.5f);
    }
    
    TEST(IO_Json, Parse_String)
    {
        io::Json json;
        std::string j = R"(
            "testing string"
        )";
        json.parse(j);
        EXPECT_EQ(json.getType(), io::Json::STRING);
        EXPECT_EQ(std::string(json), "testing string");

        j = R"(
            "skip \" value"
        )";
        json.parse(j);
        EXPECT_EQ(json.getType(), io::Json::STRING);
        EXPECT_EQ(std::string(json), "skip \" value");
    }

    TEST(IO_Json, Parse_Map)
    {
        io::Json json;
        std::string j = R"(
            {
                "test": "easy",
                "bool": true,
                "int": 1,
                "float": 2.0
            }
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::MAP);
        EXPECT_EQ(json["test"].getType(), io::Json::STRING);
        EXPECT_EQ(json["bool"].getType(), io::Json::BOOL);
        EXPECT_EQ(json["int"].getType(), io::Json::INT);
        EXPECT_EQ(json["float"].getType(), io::Json::FLOAT);
        EXPECT_EQ(std::string(json["test"]), "easy");
        EXPECT_EQ(bool(json["bool"]), true);
        EXPECT_EQ(int(json["int"]), 1);
        EXPECT_EQ(float(json["float"]), 2.0f);
    }

    TEST(IO_Json, Parse_Vector)
    {
        io::Json json;

        // Vector of ints
        std::string j = R"(
            [1, 2, 3]
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::VECTOR);
        EXPECT_EQ(json.vector().size(), 3);
        EXPECT_EQ(int(json.vector()[0]), 1);
        EXPECT_EQ(int(json.vector()[1]), 2);
        EXPECT_EQ(int(json.vector()[2]), 3);

        // Vector of floats
        j = R"(
            [1.0, 2.0, 3.0]
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::VECTOR);
        EXPECT_EQ(json.vector().size(), 3);
        EXPECT_EQ(float(json.vector()[0]), 1.0f);
        EXPECT_EQ(float(json.vector()[1]), 2.0f);
        EXPECT_EQ(float(json.vector()[2]), 3.0f);

        // Vector of strings
        j = R"(
            ["myString", "with spaces", "with special \"char\""]
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::VECTOR);
        EXPECT_EQ(json.vector().size(), 3);
        EXPECT_EQ(std::string(json.vector()[0]), "myString");
        EXPECT_EQ(std::string(json.vector()[1]), "with spaces");
        EXPECT_EQ(std::string(json.vector()[2]), "with special \"char\"");

        // Vector of maps
        j = R"(
            [{"num0": 1.0, "str0": "ok"}, {"num1": 3, "str1": "hey" }]
        )";
        EXPECT_EQ(json.parse(j), true);
        EXPECT_EQ(json.getType(), io::Json::VECTOR);
        EXPECT_EQ(json.vector().size(), 2);
        EXPECT_EQ(float(json.vector()[0].map()["num0"]), 1.0f);
        EXPECT_EQ(std::string(json.vector()[0].map()["str0"]), "ok");
        EXPECT_EQ(int(json.vector()[1].map()["num1"]), 3);
        EXPECT_EQ(std::string(json.vector()[1].map()["str1"]), "hey");
    }
}
