//--------------------------------------------------
// Atta Graphics Module
// drawer.inl
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::graphics {

// Draw 3d objects
template <typename T>
void Drawer::add(T obj, StringId group) {
    getInstance().addImpl<T>(obj, group);
}
template <typename T>
void Drawer::clear(StringId group) {
    getInstance().clearImpl<T>(group);
}

// Get data
template <typename T>
std::map<StringHash, std::vector<T>>& Drawer::getGroups() {
    return getInstance().getGroups<T>();
}
template <typename T>
unsigned Drawer::getMaxNumber() {
    return getInstance().getMaxNumberImpl<T>();
}
template <typename T>
unsigned Drawer::getCurrNumber() {
    return getInstance().getCurrNumberImpl<T>();
}

// Draw 3d objects implementation
template <typename T>
void Drawer::addImpl(T obj, StringId group) {
    if constexpr (std::is_same<T, Drawer::Line>::value || std::is_same<T, Drawer::Point>::value) {
        getGroupsImpl<T>()[group.getId()].push_back(obj);
        setCurrNumber<T>(getCurrNumber<T>() + 1);
        setChanged<T>(true);
    } else
        ASSERT(false, "Drawer add() to unknown type $0", typeid(T).name());
}

template <typename T>
void Drawer::clearImpl(StringId group) {
    if constexpr (std::is_same<T, Drawer::Line>::value || std::is_same<T, Drawer::Point>::value) {
        setCurrNumber<T>(getCurrNumber<T>() - getGroupsImpl<T>()[group.getId()].size());
        getGroupsImpl<T>()[group.getId()].clear();
        setChanged<T>(true);
    } else
        ASSERT(false, "Drawer clear() to unknown type $0. Should clear only lines or points.", typeid(T).name());
}

// Get data implementation
template <typename T>
std::map<StringHash, std::vector<T>>& Drawer::getGroupsImpl() {
    if constexpr (std::is_same<T, Drawer::Line>::value)
        return _lineGroups;
    else if constexpr (std::is_same<T, Drawer::Point>::value)
        return _pointGroups;
    else
        ASSERT(false, "Drawer getGroups() to unknown type $0", typeid(T).name());
}

template <typename T>
unsigned Drawer::getMaxNumberImpl() {
    if constexpr (std::is_same<T, Drawer::Line>::value)
        return _maxNumberOfLines;
    else if constexpr (std::is_same<T, Drawer::Point>::value)
        return _maxNumberOfPoints;
    else
        ASSERT(false, "Drawer getMaxNumber() to unknown type $0", typeid(T).name());
}

template <typename T>
unsigned Drawer::getCurrNumberImpl() {
    if constexpr (std::is_same<T, Drawer::Line>::value)
        return _currNumberOfLines;
    else if constexpr (std::is_same<T, Drawer::Point>::value)
        return _currNumberOfPoints;
    else
        ASSERT(false, "Drawer getCurrNumber() to unknown type $0", typeid(T).name());
}

template <typename T>
unsigned Drawer::getChanged() {
    if constexpr (std::is_same<T, Drawer::Line>::value)
        return _linesChanged;
    else if constexpr (std::is_same<T, Drawer::Point>::value)
        return _pointsChanged;
    else
        ASSERT(false, "Drawer getChanged() to unknown type $0", typeid(T).name());
}

template <typename T>
void Drawer::setCurrNumber(unsigned number) {
    if constexpr (std::is_same<T, Drawer::Line>::value)
        _currNumberOfLines = number;
    else if constexpr (std::is_same<T, Drawer::Point>::value)
        _currNumberOfPoints = number;
    else
        ASSERT(false, "Drawer setCurrNumber() to unknown type $0", typeid(T).name());
}

template <typename T>
void Drawer::setChanged(bool changed) {
    if constexpr (std::is_same<T, Drawer::Line>::value)
        _linesChanged = changed;
    else if constexpr (std::is_same<T, Drawer::Point>::value)
        _pointsChanged = changed;
    else
        ASSERT(false, "Drawer setChanged() to unknown type $0", typeid(T).name());
}

} // namespace atta::graphics
