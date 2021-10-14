//--------------------------------------------------
// Atta File System
// projectSerializer.inl
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    //--------------------------//
    //--------- WRITE ----------//
    //--------------------------//
    template <typename T>
    inline void ProjectSerializer::write(std::ofstream& os, T x)
    {
        os.write(reinterpret_cast<const char*>(&x), sizeof(T));
    }

    template <>
    inline void ProjectSerializer::write<std::string>(std::ofstream& os, std::string x)
    {
        os.write(x.c_str(), x.size());
        os.put('\0');
    }

    template<typename T, size_t N>
    inline void ProjectSerializer::write(std::ofstream& os, T(&x)[N])
    {
        os.write(reinterpret_cast<const char*>(&x), sizeof(T)*N);
    }

    template <typename It>
    inline void ProjectSerializer::write(std::ofstream& os, It begin, It end)
    {
        using Type = std::remove_pointer_t<std::remove_reference_t<It>>;
        os.write(reinterpret_cast<const char*>(begin), sizeof(Type)*(end-begin));
    }

    //--------------------------//
    //---------- READ ----------//
    //--------------------------//
    template <typename T>
    inline void ProjectSerializer::read(std::ifstream& is, T& x)
    {
        //using Type = std::remove_pointer_t<std::remove_reference_t<T>>;
        is.read(reinterpret_cast<char*>(&x), sizeof(T));
    }

    template <>
    inline void ProjectSerializer::read<std::string>(std::ifstream& is, std::string& x)
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

    template<typename T, size_t N>
    inline void ProjectSerializer::read(std::ifstream& is, T(&x)[N])
    {
        is.read(reinterpret_cast<char*>(&x), sizeof(T)*N);
    }
}
