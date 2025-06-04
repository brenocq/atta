//--------------------------------------------------
// Atta UI Module
// component.cpp
// Date: 2025-02-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/base.h>
#include <atta/component/componentRegistry.h>
#include <atta/component/interface.h>
#include <atta/ui/interface.h>
#include <atta/ui/widgets/component.h>

#include <imgui.h>

namespace atta::ui {

template <cmp::AttributeType attributeType>
void renderAttribute(cmp::AttributeDescription aDesc, void* d, unsigned size) {
    LOG_WARN("ui::componentWidget", "Rendering for attribute of type $0 was not defined", attributeType);
}

template <typename T>
void renderSliders(cmp::AttributeDescription aDesc, void* d, unsigned size) {
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
            ImGui::DragScalar("", dataType, data, step, &min, &max, format.c_str());
        else
            ImGui::SliderScalar("", dataType, data, &min, &max, format.c_str(), sliderFlags);
    } else if (qty <= 4) {
        const std::array<const char*, 4> labels = {"X", "Y", "Z", "W"};

        if (aDesc.step > 0.0f || undefinedMinMax)
            for (unsigned i = 0; i < qty; i++)
                ImGui::DragScalar(labels[i], dataType, data + i, step, &min, &max, format.c_str());
        else
            for (unsigned i = 0; i < qty; i++)
                ImGui::SliderScalar(labels[i], dataType, data + i, &min, &max, format.c_str(), sliderFlags);
    } else {
        // Format for arbitrarily long vector
        if (aDesc.step > 0.0f || undefinedMinMax)
            for (unsigned i = 0; i < qty; i++)
                ImGui::DragScalar(std::to_string(i).c_str(), dataType, data + i, step, &min, &max, format.c_str());
        else
            for (unsigned i = 0; i < qty; i++)
                ImGui::SliderScalar(std::to_string(i).c_str(), dataType, data + i, &min, &max, format.c_str(), sliderFlags);
    }
}

template <typename T>
void renderCombo(cmp::AttributeDescription aDesc, void* d, unsigned size) {
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

    if (ImGui::BeginCombo(aDesc.name.c_str(), valuesPreview[comboValue].c_str())) {
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

template <>
void renderAttribute<cmp::AttributeType::QUAT>(cmp::AttributeDescription aDesc, void* d, unsigned size) {
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
            oriChanged |= ImGui::DragFloat("Roll", &euler.x, step, min, max, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
            oriChanged |= ImGui::DragFloat("Pitch", &euler.y, step, min, max, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
            oriChanged |= ImGui::DragFloat("Yaw", &euler.z, step, min, max, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
        } else {
            oriChanged |= ImGui::SliderScalar("Roll", ImGuiDataType_Float, &euler.x, &min, &max, "%.2f");
            oriChanged |= ImGui::SliderScalar("Pitch", ImGuiDataType_Float, &euler.y, &min, &max, "%.2f");
            oriChanged |= ImGui::SliderScalar("Yaw", ImGuiDataType_Float, &euler.z, &min, &max, "%.2f");
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
        ImGui::DragFloat("r", &q->r, 0.01f, min, max, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::DragFloat("i", &q->i, 0.01f, min, max, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::DragFloat("j", &q->j, 0.01f, min, max, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::DragFloat("k", &q->k, 0.01f, min, max, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
    }
}

template <>
void renderAttribute<cmp::AttributeType::STRINGID>(cmp::AttributeDescription aDesc, void* d, unsigned size) {
    StringHash* data = (StringHash*)d;

    if (aDesc.options.size() > 0) {
        std::vector<StringId> sids;
        for (std::string value : aDesc.options)
            sids.emplace_back(StringId(value));

        int comboValue = -1;
        for (size_t i = 0; i < sids.size(); i++)
            if (sids[i] == *data)
                comboValue = i;
        if (comboValue == -1 && sids.size()) {
            comboValue = 0;
            *data = sids[0].getId();
        }

        const char* comboPreviewValue = sids[comboValue].getString().c_str();
        if (ImGui::BeginCombo(aDesc.name.c_str(), comboPreviewValue)) {
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
void renderAttribute<cmp::AttributeType::BOOL>(cmp::AttributeDescription aDesc, void* d, unsigned size) {
    bool* data = (bool*)d;
    ImGui::Checkbox(aDesc.name.c_str(), data);
}

template <>
void renderAttribute<cmp::AttributeType::CHAR>(cmp::AttributeDescription aDesc, void* d, unsigned size) {
    char* data = (char*)d;
    ImGui::InputText(aDesc.name.c_str(), data, size);
}

#define ATTA_UI_RENDER_ATTRIBUTE_COMBO_SLIDER(AttaType, CppType)                                                                                     \
    template <>                                                                                                                                      \
    void renderAttribute<cmp::AttributeType::AttaType>(cmp::AttributeDescription aDesc, void* d, unsigned size) {                                    \
        if (aDesc.options.empty())                                                                                                                   \
            renderSliders<CppType>(aDesc, d, size);                                                                                                  \
        else                                                                                                                                         \
            renderCombo<CppType>(aDesc, d, size);                                                                                                    \
    }

#define ATTA_UI_RENDER_ATTRIBUTE_SLIDER(AttaType, CppType)                                                                                           \
    template <>                                                                                                                                      \
    void renderAttribute<cmp::AttributeType::AttaType>(cmp::AttributeDescription aDesc, void* d, unsigned size) {                                    \
        renderSliders<CppType>(aDesc, d, size);                                                                                                      \
    }

// These types may be rendered as a slider, or combo box (in case of enum)
ATTA_UI_RENDER_ATTRIBUTE_COMBO_SLIDER(INT8, int8_t)
ATTA_UI_RENDER_ATTRIBUTE_COMBO_SLIDER(INT16, int16_t)
ATTA_UI_RENDER_ATTRIBUTE_COMBO_SLIDER(INT32, int32_t)
ATTA_UI_RENDER_ATTRIBUTE_COMBO_SLIDER(INT64, int64_t)
ATTA_UI_RENDER_ATTRIBUTE_COMBO_SLIDER(UINT8, uint8_t)
ATTA_UI_RENDER_ATTRIBUTE_COMBO_SLIDER(UINT16, uint16_t)
ATTA_UI_RENDER_ATTRIBUTE_COMBO_SLIDER(UINT32, uint32_t)
ATTA_UI_RENDER_ATTRIBUTE_COMBO_SLIDER(UINT64, uint64_t)
// Floating point types are always rendered as sliders
ATTA_UI_RENDER_ATTRIBUTE_SLIDER(FLOAT32, float)
ATTA_UI_RENDER_ATTRIBUTE_SLIDER(FLOAT64, double)

#define ATTA_UI_ATTRIBUTE_CASE(ComponentType)                                                                                                        \
    case cmp::AttributeType::ComponentType:                                                                                                          \
    case cmp::AttributeType::VECTOR_##ComponentType:                                                                                                 \
        renderAttribute<cmp::AttributeType::ComponentType>(aDesc, d, size);                                                                          \
        break;

void renderAttribute(cmp::AttributeDescription aDesc, void* d, unsigned size) {
    switch (aDesc.type) {
        case cmp::AttributeType::BOOL:
            renderAttribute<cmp::AttributeType::BOOL>(aDesc, d, size);
            break;
            ATTA_UI_ATTRIBUTE_CASE(CHAR)
            ATTA_UI_ATTRIBUTE_CASE(INT8)
            ATTA_UI_ATTRIBUTE_CASE(INT16)
            ATTA_UI_ATTRIBUTE_CASE(INT32)
            ATTA_UI_ATTRIBUTE_CASE(INT64)
            ATTA_UI_ATTRIBUTE_CASE(UINT8)
            ATTA_UI_ATTRIBUTE_CASE(UINT16)
            ATTA_UI_ATTRIBUTE_CASE(UINT32)
            ATTA_UI_ATTRIBUTE_CASE(UINT64)
            ATTA_UI_ATTRIBUTE_CASE(FLOAT32)
            ATTA_UI_ATTRIBUTE_CASE(FLOAT64)
        case cmp::AttributeType::QUAT:
            renderAttribute<cmp::AttributeType::QUAT>(aDesc, d, size);
            break;
        case cmp::AttributeType::STRINGID:
            renderAttribute<cmp::AttributeType::STRINGID>(aDesc, d, size);
            break;
        default:
            break;
    }
}

void renderAttributes(const std::vector<cmp::AttributeDescription>& aDescs, cmp::Component* comp, unsigned compSizeOf) {
    // Render UI for each attribute
    for (unsigned i = 0; i < aDescs.size(); i++) {
        cmp::AttributeDescription aDesc = aDescs[i];

        // Calculate data and size
        void* data = (void*)((uint8_t*)comp + aDesc.offset);
        unsigned size = (i == aDescs.size() - 1) ? compSizeOf - aDesc.offset : aDescs[i + 1].offset - aDesc.offset;

        // Render attribute
        ImGui::PushID(aDesc.name.c_str());
        renderAttribute(aDesc, data, size);
        ImGui::PopID();
    }
}

void componentDefaultRender(cmp::Entity entity, cmp::ComponentId cid, cmp::Component* comp) {
    // Get component description
    std::vector<cmp::ComponentRegistry*> cmpRegs = cmp::getComponentRegistries();
    cmp::ComponentRegistry* cmpReg = nullptr;
    for (auto cmpReg_ : cmpRegs) {
        if (cmpReg_->getId() == cid) {
            cmpReg = cmpReg_;
            break;
        }
    }
    if (cmpReg == nullptr) {
        LOG_ERROR("ui::componentWidget", "ComponentId [w]$0[] is invalid, there are only [w]$1`[] components registered", cid, cmpRegs.size());
        return;
    }

    // Render component attributes
    const cmp::ComponentDescription& description = cmpReg->getDescription();
    const std::vector<cmp::AttributeDescription> attributeDescriptions = description.attributeDescriptions;
    renderAttributes(attributeDescriptions, comp, cmpReg->getSizeof());
}

void componentWidget(cmp::Entity entity, cmp::ComponentId cid, cmp::Component* comp) {
    // Check if custom rendering was defined for this component
    std::optional<ComponentUIFunc> uiFunc = ui::getComponentUI(cid);
    if (uiFunc.has_value()) {
        // Custom component UI (custom UI specified defined for this component)
        uiFunc.value()(entity, comp);
        return;
    }
    // Default component UI (render attributes)
    componentDefaultRender(entity, cid, comp);
}

} // namespace atta::ui
