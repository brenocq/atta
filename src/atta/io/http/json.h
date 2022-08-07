//--------------------------------------------------
// Atta IO Module
// json.h
// Date: 2022-06-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_HTTP_JSON_H
#define ATTA_IO_HTTP_JSON_H

namespace atta::io
{
    class Json
    {
    public:
        Json(): _type(NONE) {}
        Json(std::string str);

        enum Type
        {
            NONE = 0,
            BOOL,
            INT,
            FLOAT,
            STRING,
            VECTOR,
            MAP
        };

        bool parse(const std::string& str);
        std::string dump();

        // Access
        Json& operator[](std::string key);
        Json& operator[](unsigned i);
        const Json& at(std::string key) const;
        const Json& at(unsigned i) const;

        // Assign
        void operator=(bool b);
        void operator=(int i);
        void operator=(float f);
        void operator=(std::string s);
        void operator=(std::vector<Json> v);
        void operator=(std::map<std::string, Json> m);

        // Get
        Type getType() const;

        std::map<std::string, Json>& map();
        const std::map<std::string, Json>& map() const;
        std::vector<Json>& vector();
        const std::vector<Json>& vector() const;

        explicit operator bool() const;
        explicit operator int() const;
        explicit operator float() const;
        explicit operator std::string() const;

    private:
        void parseAux(const std::string& str, unsigned& pos, bool& res);
        void parseNone(const std::string& str, unsigned& pos, bool& res);
        bool parseBool(const std::string& str, unsigned& pos, bool& res);
        int parseInt(const std::string& str, unsigned& pos, bool& res);
        float parseFloat(const std::string& str, unsigned& pos, bool& res);
        std::string parseString(const std::string& str, unsigned& pos, bool& res);
        std::vector<Json> parseVector(const std::string& str, unsigned& pos, bool& res);
        std::map<std::string, Json> parseMap(const std::string& str, unsigned& pos, bool& res);

        std::map<std::string, Json> _map;
        std::vector<Json> _vector;
        std::string _string;
        union Value
        {
            bool b;
            int i;
            float f;
        } _value;
        Type _type;
    };
}

#endif// ATTA_IO_HTTP_JSON_H
