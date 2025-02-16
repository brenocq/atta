//--------------------------------------------------
// Atta Utils Tests
// stringUtils.cpp
// Date: 2025-02-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/stringUtils.h>
#include <gtest/gtest.h>

using namespace atta;

namespace {

TEST(Utils_StringUtils, SplitWords) {
    std::vector<std::string> expected;

    expected = {"apple", "pie"};
    EXPECT_EQ(splitWords("apple_pie"), expected);

    expected = {"File", "Name"};
    EXPECT_EQ(splitWords("File_Name"), expected);

    expected = {"Open", "GL", "Renderer"};
    EXPECT_EQ(splitWords("OpenGLRenderer"), expected);

    expected = {"user", "Input"};
    EXPECT_EQ(splitWords("userInput"), expected);

    expected = {"Window", "Size"};
    EXPECT_EQ(splitWords("Window Size"), expected);

    expected = {"High", "Resolution", "Texture"};
    EXPECT_EQ(splitWords("High Resolution Texture"), expected);

    expected = {"vector", "XYZ"};
    EXPECT_EQ(splitWords("vectorXYZ"), expected);

    expected = {"HTTP", "Request"};
    EXPECT_EQ(splitWords("HTTPRequest"), expected);
}

TEST(Utils_StringUtils, CamelCase) {
    EXPECT_EQ(toCamelCase("apple_pie"), "applePie");
    EXPECT_EQ(toCamelCase("File_Name"), "fileName");
    EXPECT_EQ(toCamelCase("OpenGLRenderer"), "openGLRenderer");
    EXPECT_EQ(toCamelCase("userInput"), "userInput");
    EXPECT_EQ(toCamelCase("Window Size"), "windowSize");
    EXPECT_EQ(toCamelCase("High Resolution Texture"), "highResolutionTexture");
    EXPECT_EQ(toCamelCase("vectorXYZ"), "vectorXYZ");
    EXPECT_EQ(toCamelCase("HTTPRequest"), "httpRequest");
}

TEST(Utils_StringUtils, PascalCase) {
    EXPECT_EQ(toPascalCase("apple_pie"), "ApplePie");
    EXPECT_EQ(toPascalCase("File_Name"), "FileName");
    EXPECT_EQ(toPascalCase("OpenGLRenderer"), "OpenGLRenderer");
    EXPECT_EQ(toPascalCase("userInput"), "UserInput");
    EXPECT_EQ(toPascalCase("Window Size"), "WindowSize");
    EXPECT_EQ(toPascalCase("High Resolution Texture"), "HighResolutionTexture");
    EXPECT_EQ(toPascalCase("vectorXYZ"), "VectorXYZ");
    EXPECT_EQ(toPascalCase("HTTPRequest"), "HTTPRequest");
}

TEST(Utils_StringUtils, SnakeCase) {
    EXPECT_EQ(toSnakeCase("applePie"), "apple_pie");
    EXPECT_EQ(toSnakeCase("FileName"), "file_name");
    EXPECT_EQ(toSnakeCase("OpenGLRenderer"), "open_gl_renderer");
    EXPECT_EQ(toSnakeCase("userInput"), "user_input");
    EXPECT_EQ(toSnakeCase("Window Size"), "window_size");
    EXPECT_EQ(toSnakeCase("High Resolution Texture"), "high_resolution_texture");
    EXPECT_EQ(toSnakeCase("vectorXYZ"), "vector_xyz");
    EXPECT_EQ(toSnakeCase("HTTPRequest"), "http_request");
}

TEST(Utils_StringUtils, TitleCase) {
    EXPECT_EQ(toTitleCase("apple_pie"), "Apple Pie");
    EXPECT_EQ(toTitleCase("File_Name"), "File Name");
    EXPECT_EQ(toTitleCase("OpenGLRenderer"), "Open GL Renderer");
    EXPECT_EQ(toTitleCase("userInput"), "User Input");
    EXPECT_EQ(toTitleCase("Window Size"), "Window Size");
    EXPECT_EQ(toTitleCase("High Resolution Texture"), "High Resolution Texture");
    EXPECT_EQ(toTitleCase("vectorXYZ"), "Vector XYZ");
    EXPECT_EQ(toTitleCase("HTTPRequest"), "HTTP Request");
}

} // namespace
