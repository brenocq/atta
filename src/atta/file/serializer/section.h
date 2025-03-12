//--------------------------------------------------
// Atta File Module
// section.h
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SERIALIZER_SECTION_H
#define ATTA_FILE_SERIALIZER_SECTION_H

namespace atta::file {

/** Section Data
 *
 * Handles conversion between string and data types
 **/
class SectionData {
  public:
    // Default constructor
    SectionData() = default;

    // Constructor to handle initialization
    template <typename T>
    SectionData(T&& value) {
        *this = std::forward<T>(value); // Use the assignment operator
    }

    void setStr(const std::string& str) { _str = str; }
    const std::string& getStr() const { return _str; }

    // Assign operators
    template <typename T>
    void operator=(T&& value);
    template <typename T>
    void operator=(std::initializer_list<T> value);

    // Explicit cast operator
    template <typename T>
    explicit operator T() const;

  private:
    std::string _str;
};

/** Each serialized file is composed of multiple sections
 *
 *  Each section looks like this when serialized:
 *  ```
 *  [sectionName]
 *  key1 = "someString"
 *  key2 = 10
 *  key3 = true
 *  ```
 **/
class Section {
  public:
    Section(const std::string& name) : _name(name) {}

    /// Getters
    const std::string& getName() const;
    bool empty() const;
    size_t size() const;
    bool contains(std::string key) const;

    /// Insert/read
    SectionData& operator[](std::string key);
    const SectionData& operator[](std::string key) const;

    /// Serialize/deserialize
    void insertFromString(const std::string& string);
    std::string toString() const;

  private:
    std::string _name;
    std::vector<std::string> _keys; // Keys are stored in a vector to keep the order
    std::map<std::string, SectionData> _map;
};

} // namespace atta::file

#include <atta/file/serializer/section.inl>

#endif // ATTA_FILE_SERIALIZER_SECTION_H
