//--------------------------------------------------
// Atta File System
// section.h
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SYSTEM_SERIALIZER_SECTION_H
#define ATTA_FILE_SYSTEM_SERIALIZER_SECTION_H
#include <atta/fileSystem/serializer/serializable.h>
#include <atta/fileSystem/serializer/serializer.h>

namespace atta
{
    /// Section Data
    /** Store the binary data for each data and its type so that it is possible to print SectionData and serialize it
     **/
    class SectionData : Serializable
    {
    public:
        using TypeHash = StringId;
        SectionData();

        void clear();

        template<typename T>
        void operator=(T&& value);

        template<typename T>
        T get();
        template<typename T>
        T getConst() const;
        template<typename T>
        T* getPtr();
        template<typename T>
        T* getPtrConst() const;

        TypeHash getTypeHash() const;

        std::string toString() const;
        void serialize(std::ostream& os) override;
        void deserialize(std::istream& is) override;

    private:
        std::vector<uint8_t> _data;
        TypeHash _typeHash;

        using PrintFunction = std::function<std::string(std::vector<uint8_t> data)>;

        template<typename T>
        static void registerType();
        static std::unordered_map<TypeHash, PrintFunction> _typeToString;
    };

    /** There are three main possibilities for a section:
     *  - Map of sections (data as std::map<std::string, Section>)
     *  - Vector of sections (data as std::vector<Section>)
     *  - Data 
     **/
    class Section : Serializable
    {
    public:
        Section();
        template <typename T>
        explicit Section(T value);

        Section(const Section& section);

        // Check type
        bool isUndefined() const;
        bool isMap() const;
        bool isVector() const;
        bool isData() const;

        size_t size() const;
        std::string toString() const;

        //----- Data -----//
        /// Get data
        SectionData& data();
        const SectionData& data() const;

        /// Assign data
        template <typename T>
        void operator=(T value);

        /// Get data (casting)
        template <typename T>
        operator T();

        //----- Map -----//
        /// Get map
        std::map<std::string, Section>& map();
        const std::map<std::string, Section>& map() const;

        /// Map access
        Section& operator[](std::string key);
        Section& operator[](const char* key);

        //----- Vector -----//
        /// Get vector
        std::vector<Section>& vector();
        const std::vector<Section>& vector() const;

        /// Vector access
        Section& operator[](unsigned i);
        
        /// Push to vector
        void push_back(Section section);
        void operator+=(Section section);

        /// Get last section
        Section& back();

        /// Assign vector of values
        template <typename T>
        void operator=(std::initializer_list<T> list);

        //----- Serialization -----//
        void serialize(std::ostream& os) override;
        void deserialize(std::istream& is) override;

    private:
        enum Type
        {
            UNDEFINED = 0,
            MAP,
            VECTOR,
            DATA
        };

        std::map<std::string, Section> _map;
        std::vector<Section> _vector;
        SectionData _data;
        Type _type;
    };

    // <<
    inline std::ostream& operator<<(std::ostream& os, const Section& v)
    {
        return os << v.toString();
    }
}
#include <atta/fileSystem/serializer/section.inl>


#endif// ATTA_FILE_SYSTEM_SERIALIZER_SECTION_H
