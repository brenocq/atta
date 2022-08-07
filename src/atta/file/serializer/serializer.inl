//--------------------------------------------------
// Atta File Module
// serializer.inl
// Date: 2021-11-12
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
    //--------------------------//
    //--------- WRITE ----------//
    //--------------------------//
    template <typename T>
    inline void write(std::ostream& os, T x)
    {
        os.write(reinterpret_cast<const char*>(&x), sizeof(T));
    }

    template <>
    inline void write<std::string>(std::ostream& os, std::string x)
    {
        os.write(x.c_str(), strlen(x.c_str()));
        os.put('\0');
    }

    template <>
    inline void write<StringId>(std::ostream& os, StringId x)
    {
        std::string str = x.getString();
        write(os, str);
    }

    template<typename T, size_t N>
    inline void write(std::ostream& os, T(&x)[N])
    {
        os.write(reinterpret_cast<const char*>(&x), sizeof(T)*N);
    }

    template <typename T>
    inline void write(std::ostream& os, T* x, size_t size)
    {
        os.write(reinterpret_cast<const char*>(x), size);
    }

    //--------------------------//
    //---------- READ ----------//
    //--------------------------//
    template <typename T>
    inline void read(std::istream& is, T& x)
    {
        //using Type = std::remove_pointer_t<std::remove_reference_t<T>>;
        is.read(reinterpret_cast<char*>(&x), sizeof(T));
    }

    template <>
    inline void read<std::string>(std::istream& is, std::string& x)
    {
        // Calculate string size
        int init = is.tellg();
        while(is.get()!='\0' && !is.eof());
        int size = int(is.tellg())-init-1;

        // Return to string first char
        is.seekg(init);

        // Read string
        x.resize(size);
        is.read(&x[0], size);
        is.ignore();// jump \0
    }

    template <>
    inline void read<StringId>(std::istream& is, StringId& x)
    {
        std::string str;
        read(is, str);
        x = StringId(str);
    }

    template<typename T, size_t N>
    inline void read(std::istream& is, T(&x)[N])
    {
        is.read(reinterpret_cast<char*>(&x), sizeof(T)*N);
    }

    template<typename T>
    inline void read(std::istream& is, T* x, size_t size)
    {
        is.read(reinterpret_cast<char*>(x), size);
    }
}
