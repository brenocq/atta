//--------------------------------------------------
// Atta File System
// section.h
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SYSTEM_SERIALIZER_SECTION_H
#define ATTA_FILE_SYSTEM_SERIALIZER_SECTION_H

namespace atta
{
    /** There are three main possibilities for a section:
     *  - Map of sections (data as std::map<std::string, Section>)
     *  - Vector of sections (data as std::vector<Section>)
     *  - Data 
     **/
    class Section
    {
    public:
        Section();
        template <typename T>
        Section(T value);

        // Check type
        bool isUndefined() const;
        bool isMap() const;
        bool isVector() const;
        bool isValue() const;

        size_t size() const;
        std::string toString() const;

        //----- Value -----//
        /// Assign value
        template <typename T>
        void operator=(T value);

        /// Get value (casting)
        template <typename T>
        operator T();

        //----- Map -----//
        /// Get map
        std::map<std::string, Section>& map();
        const std::map<std::string, Section>& map() const;

        /// Map access
        Section& operator[](std::string key);

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

    private:
        std::any _data;
    };

    // <<
    inline std::ostream& operator<<(std::ostream& os, const Section& v)
    {
        return os << v.toString();
    }
}
#include <atta/fileSystem/serializer/section.inl>


#endif// ATTA_FILE_SYSTEM_SERIALIZER_SECTION_H
