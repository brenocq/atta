//--------------------------------------------------
// Atta File System
// section.inl
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
    template <typename T>
    Section::Section(T value)
    {
        _data = std::any(value);
    }

    template <typename T>
    void Section::operator=(T value)
    {
        _data = std::any(value);
    }

    template <typename T>
    void Section::operator=(std::initializer_list<T> list)
    {
        _data = std::vector<T>(list);
    }

    template <typename T>
    Section::operator T()
    {
        if(isValue())
        {
            try
            {
                return std::any_cast<T>(_data);
            }
            catch(const std::bad_any_cast& e)
            {
                LOG_WARN("Filesystem::Section", "Wrong section casting to [w]$0[], value is of type [w]$1[]", typeid(T).name(), _data.type().name());
            }
        }
        else
            LOG_WARN("Filesystem::Section", "Trying to cast section as [w]$0[], but this section is not a value", typeid(T).name());

        return T{};
    }
}
