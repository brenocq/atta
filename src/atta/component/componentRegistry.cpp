//--------------------------------------------------
// Atta Component Module
// componentRegistry.cpp
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/componentRegistry.h>
#include <atta/component/interface.h>
#include <imgui.h>
#include <type_traits>

namespace atta::component {
void ComponentRegistry::registerToManager() { component::registerComponent(this); }

unsigned ComponentRegistry::getSerializedSize(Component* component) {
    std::ostringstream of;
    std::basic_ostream<char>::pos_type posBefore = of.tellp();
    serialize(of, component);
    ASSERT(of.tellp() != -1, "Failed to calculate componente serialized size")
    unsigned size = (int)of.tellp() - posBefore;
    return size;
}

template <typename T>
void renderSliders(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {
    T* data = (T*)d;

    ImGuiDataType dataType;
    if constexpr (std::is_same_v<T, uint8_t>)
        dataType = ImGuiDataType_U8;
    else if constexpr (std::is_same_v<T, uint16_t>)
        dataType = ImGuiDataType_U16;
    else if constexpr (std::is_same_v<T, uint32_t>)
        dataType = ImGuiDataType_U32;
    else if constexpr (std::is_same_v<T, uint64_t>)
        dataType = ImGuiDataType_U64;
    else if constexpr (std::is_same_v<T, int8_t>)
        dataType = ImGuiDataType_S8;
    else if constexpr (std::is_same_v<T, int16_t>)
        dataType = ImGuiDataType_S16;
    else if constexpr (std::is_same_v<T, int32_t>)
        dataType = ImGuiDataType_S32;
    else if constexpr (std::is_same_v<T, int64_t>)
        dataType = ImGuiDataType_S64;
    else if constexpr (std::is_same_v<T, float>)
        dataType = ImGuiDataType_Float;
    else if constexpr (std::is_same_v<T, double>)
        dataType = ImGuiDataType_Double;
    else
        return;

    std::string format;
    if constexpr (std::is_same_v<T, uint8_t>)
        format = "%hu";
    else if constexpr (std::is_same_v<T, uint16_t>)
        format = "%hu";
    else if constexpr (std::is_same_v<T, uint32_t>)
        format = "%u";
    else if constexpr (std::is_same_v<T, uint64_t>)
        format = "%lu";
    else if constexpr (std::is_same_v<T, int8_t>)
        format = "%hd";
    else if constexpr (std::is_same_v<T, int16_t>)
        format = "%hd";
    else if constexpr (std::is_same_v<T, int32_t>)
        format = "%d";
    else if constexpr (std::is_same_v<T, int64_t>)
        format = "%ld";
    else if constexpr (std::is_same_v<T, float>)
        format = "%.6f";
    else if constexpr (std::is_same_v<T, double>)
        format = "%.6lf";

    ImGuiSliderFlags sliderFlags = ImGuiSliderFlags_None;

    unsigned qty = size / sizeof(T);
    bool undefinedMinMax = false;
    T min, max;

    if (aDesc.min.has_value())
        min = std::any_cast<T>(aDesc.min);
    else {
        undefinedMinMax = true;
        min = std::numeric_limits<T>::lowest();
        if constexpr (std::is_same_v<T, float>)
            min /= 2.0f;
    }

    if (aDesc.max.has_value())
        max = std::any_cast<T>(aDesc.max);
    else {
        undefinedMinMax = true;
        max = std::numeric_limits<T>::max();
        if constexpr (std::is_same_v<T, float>)
            max /= 2.0f;
    }

    float step = aDesc.step > 0 ? aDesc.step : std::numeric_limits<T>::epsilon();

    // Logarithmic slider if range is too big
    if (max - min >= T(10000) && !undefinedMinMax)
        sliderFlags = ImGuiSliderFlags_Logarithmic;

    // Format for scalar
    ImGui::Text(aDesc.name.c_str());

    if (qty == 1) {
        if (aDesc.step > 0.0f || undefinedMinMax)
            ImGui::DragScalar(imguiId.c_str(), dataType, data, step, &min, &max, format.c_str());
        else
            ImGui::SliderScalar(imguiId.c_str(), dataType, data, &min, &max, format.c_str(), sliderFlags);
    } else if (qty <= 4) {
        const std::array<const char*, 4> labels = {"X", "Y", "Z", "W"};

        if (aDesc.step > 0.0f || undefinedMinMax)
            for (unsigned i = 0; i < qty; i++)
                ImGui::DragScalar((labels[i] + imguiId).c_str(), dataType, data + i, step, &min, &max, format.c_str());
        else
            for (unsigned i = 0; i < qty; i++)
                ImGui::SliderScalar((labels[i] + imguiId).c_str(), dataType, data + i, &min, &max, format.c_str(), sliderFlags);
    } else {
        // Format for arbitrarily long vector
        if (aDesc.step > 0.0f || undefinedMinMax)
            for (unsigned i = 0; i < qty; i++)
                ImGui::DragScalar((std::to_string(i) + imguiId).c_str(), dataType, data + i, step, &min, &max, format.c_str());
        else
            for (unsigned i = 0; i < qty; i++)
                ImGui::SliderScalar((std::to_string(i) + imguiId).c_str(), dataType, data + i, &min, &max, format.c_str(), sliderFlags);
    }
}

template <typename T>
void renderCombo(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {
    T* data = (T*)d;

    unsigned qty = size / sizeof(T);
#ifdef ATTA_DEBUG_BUILD
    if (qty != 1) {
        LOG_WARN("component::ComponentRegistry",
                 "The component attribute [w]$0[] cannot be rendered with options because it is composed of multiple [w]$1[] values", aDesc.name,
                 typeid(T).name());
        return;
    }
#endif

    // Create preview values
    std::vector<std::string> valuesPreview;
    for (std::string value : aDesc.options)
        valuesPreview.push_back(value);

    // Find selected value
    int comboValue = -1;
    for (size_t i = 0; i < valuesPreview.size(); i++)
        if ((T)i == *data)
            comboValue = i;
    if (comboValue == -1) {
        comboValue = 0;
        *data = (T)0;
    }

    if (ImGui::BeginCombo((aDesc.name + "##" + imguiId).c_str(), valuesPreview[comboValue].c_str())) {
        auto it = aDesc.options.begin();
        for (size_t i = 0; i < valuesPreview.size(); i++, it++) {
            if (ImGui::Selectable(valuesPreview[i].c_str(), (size_t)comboValue == i))
                *data = (T)i;
            if ((size_t)comboValue == i)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

#define ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(ComponentType)                                                                                        \
    case AttributeType::ComponentType:                                                                                                               \
    case AttributeType::VECTOR_##ComponentType:                                                                                                      \
        ComponentRegistry::renderUIAttribute<AttributeType::ComponentType>(aDesc, d, size, imguiId + aDesc.name);                                    \
        break;

void ComponentRegistry::renderUIAttribute(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {
    // XXX Remember to register each attribute with ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE or will get MSVC errors
    switch (aDesc.type) {
        case AttributeType::BOOL:
            ComponentRegistry::renderUIAttribute<AttributeType::BOOL>(aDesc, d, size, imguiId + aDesc.name);
            break;
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(CHAR)
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(INT8)
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(INT16)
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(INT32)
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(INT64)
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(UINT8)
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(UINT16)
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(UINT32)
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(UINT64)
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(FLOAT32)
            ATTA_COMPONENT_REGISTER_RENDER_UI_CASE(FLOAT64)
        case AttributeType::QUAT:
            ComponentRegistry::renderUIAttribute<AttributeType::QUAT>(aDesc, d, size, imguiId + aDesc.name);
            break;
        case AttributeType::STRINGID:
            ComponentRegistry::renderUIAttribute<AttributeType::STRINGID>(aDesc, d, size, imguiId + aDesc.name);
            break;
        default:
            break;
    }
}

#define ATTA_RENDER_UI_ATTRIBUTE_COMBO_SLIDER(AttaType, CppType)                                                                                     \
    template <>                                                                                                                                      \
    void ComponentRegistry::renderUIAttribute<AttributeType::AttaType>(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {    \
        if (aDesc.options.empty())                                                                                                                   \
            renderSliders<CppType>(aDesc, d, size, imguiId);                                                                                         \
        else                                                                                                                                         \
            renderCombo<CppType>(aDesc, d, size, imguiId);                                                                                           \
    }

#define ATTA_RENDER_UI_ATTRIBUTE_SLIDER(AttaType, CppType)                                                                                           \
    template <>                                                                                                                                      \
    void ComponentRegistry::renderUIAttribute<AttributeType::AttaType>(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {    \
        renderSliders<CppType>(aDesc, d, size, imguiId);                                                                                             \
    }

ATTA_RENDER_UI_ATTRIBUTE_COMBO_SLIDER(INT8, int8_t);
ATTA_RENDER_UI_ATTRIBUTE_COMBO_SLIDER(INT16, int16_t);
ATTA_RENDER_UI_ATTRIBUTE_COMBO_SLIDER(INT32, int32_t);
ATTA_RENDER_UI_ATTRIBUTE_COMBO_SLIDER(INT64, int64_t);
ATTA_RENDER_UI_ATTRIBUTE_COMBO_SLIDER(UINT8, uint8_t);
ATTA_RENDER_UI_ATTRIBUTE_COMBO_SLIDER(UINT16, uint16_t);
ATTA_RENDER_UI_ATTRIBUTE_COMBO_SLIDER(UINT32, uint32_t);
ATTA_RENDER_UI_ATTRIBUTE_COMBO_SLIDER(UINT64, uint64_t);
ATTA_RENDER_UI_ATTRIBUTE_SLIDER(FLOAT32, float);
ATTA_RENDER_UI_ATTRIBUTE_SLIDER(FLOAT64, double);

template <>
void ComponentRegistry::renderUIAttribute<AttributeType::QUAT>(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {
    ImGui::Text(aDesc.name.c_str());
    quat* q = (quat*)d;

    // Trying Euler
    if (false) {
        vec3 eulerBef = degrees(q->getEuler());
        vec3 euler = eulerBef;
        bool oriChanged = false;

        float min = 0.0f;
        float max = 1.0f;
        if (aDesc.min.has_value())
            min = std::any_cast<float>(aDesc.min);
        if (aDesc.max.has_value())
            max = std::any_cast<float>(aDesc.max);

        // Get rotation around axis
        if (aDesc.step > 0.0f) {
            float step = aDesc.step;
            oriChanged |= ImGui::DragFloat(("Roll" + imguiId).c_str(), &euler.x, step, min, max, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
            oriChanged |= ImGui::DragFloat(("Pitch" + imguiId).c_str(), &euler.y, step, min, max, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
            oriChanged |= ImGui::DragFloat(("Yaw" + imguiId).c_str(), &euler.z, step, min, max, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
        } else {
            oriChanged |= ImGui::SliderScalar(("Roll" + imguiId).c_str(), ImGuiDataType_Float, &euler.x, &min, &max, "%.2f");
            oriChanged |= ImGui::SliderScalar(("Pitch" + imguiId).c_str(), ImGuiDataType_Float, &euler.y, &min, &max, "%.2f");
            oriChanged |= ImGui::SliderScalar(("Yaw" + imguiId).c_str(), ImGuiDataType_Float, &euler.z, &min, &max, "%.2f");
        }

        // Rotate quaternian around axis
        if (oriChanged) {
            vec3 axis = vec3(1, 0, 0);
            float angle = 0.0f;
            if (euler.x != eulerBef.x) {
                axis = vec3(1, 0, 0);
                angle = radians(euler.x - eulerBef.x);
            } else if (euler.y != eulerBef.y) {
                axis = vec3(0, 1, 0);
                angle = radians(euler.y - eulerBef.y);
            } else if (euler.z != eulerBef.z) {
                axis = vec3(0, 0, 1);
                angle = radians(euler.z - eulerBef.z);
            }
            q->rotateVector(axis); // Rotate axis to local coordinates
            q->rotateAroundAxis(axis, angle);
        }
    }
    // Trying raw
    else {
        bool oriChanged = false;
        float min = -1.0f;
        float max = 1.0f;
        ImGui::DragFloat(("r" + imguiId).c_str(), &q->r, 0.01f, min, max, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::DragFloat(("i" + imguiId).c_str(), &q->i, 0.01f, min, max, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::DragFloat(("j" + imguiId).c_str(), &q->j, 0.01f, min, max, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::DragFloat(("k" + imguiId).c_str(), &q->k, 0.01f, min, max, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
    }
}

template <>
void ComponentRegistry::renderUIAttribute<AttributeType::STRINGID>(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {
    StringHash* data = (StringHash*)d;

    if (aDesc.options.size() > 0) {
        std::vector<StringId> sids;
        for (auto value : aDesc.options)
            sids.emplace_back(std::any_cast<StringId>(value).getString());

        int comboValue = -1;
        for (size_t i = 0; i < sids.size(); i++)
            if (sids[i] == *data)
                comboValue = i;
        if (comboValue == -1 && sids.size()) {
            comboValue = 0;
            *data = sids[0].getId();
        }

        const char* comboPreviewValue = sids[comboValue].getString().c_str();
        if (ImGui::BeginCombo((aDesc.name + "##" + imguiId).c_str(), comboPreviewValue)) {
            for (size_t i = 0; i < sids.size(); i++) {
                if (ImGui::Selectable(sids[i].getString().c_str(), comboValue == (int)i))
                    *data = sids[i].getId();
                if (comboValue == (int)i)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    } else {
        // TODO text field (do not save StringId!)
    }
}

template <>
void ComponentRegistry::renderUIAttribute<AttributeType::BOOL>(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {
    bool* data = (bool*)d;
    ImGui::Checkbox((aDesc.name + "##" + imguiId).c_str(), data);
}

template <>
void ComponentRegistry::renderUIAttribute<AttributeType::CHAR>(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {
    char* data = (char*)d;
    ImGui::InputText((aDesc.name + "##" + imguiId).c_str(), data, size);
}

} // namespace atta::component
