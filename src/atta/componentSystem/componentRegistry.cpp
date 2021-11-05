//--------------------------------------------------
// Atta Component System
// componentRegistry.cpp
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/math/math.h>
#include <imgui.h>

namespace atta
{
    template<typename T>
    void renderSliders(ComponentRegistry::AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)
    {
        T* data = (T*)d;

        ImGuiDataType dataType;
        if constexpr(std::is_same_v<T, uint8_t>)
            dataType = ImGuiDataType_U8;
        else if constexpr(std::is_same_v<T, uint16_t>)
            dataType = ImGuiDataType_U16;
        else if constexpr(std::is_same_v<T, uint32_t>)
            dataType = ImGuiDataType_U32;
        else if constexpr(std::is_same_v<T, uint64_t>)
            dataType = ImGuiDataType_U64;
        else if constexpr(std::is_same_v<T, int8_t>)
            dataType = ImGuiDataType_S8;
        else if constexpr(std::is_same_v<T, int16_t>)
            dataType = ImGuiDataType_S16;
        else if constexpr(std::is_same_v<T, int32_t>)
            dataType = ImGuiDataType_S32;
        else if constexpr(std::is_same_v<T, int64_t>)
            dataType = ImGuiDataType_S64;
        else if constexpr(std::is_same_v<T, float>)
            dataType = ImGuiDataType_Float;
        else if constexpr(std::is_same_v<T, double>)
            dataType = ImGuiDataType_Double;
        else
            return;

        std::string format;
        if constexpr(std::is_same_v<T, uint8_t>)
            format = "%hu";
        else if constexpr(std::is_same_v<T, uint16_t>)
            format = "%hu";
        else if constexpr(std::is_same_v<T, uint32_t>)
            format = "%u";
        else if constexpr(std::is_same_v<T, uint64_t>)
            format = "%lu";
        else if constexpr(std::is_same_v<T, int8_t>)
            format = "%hd";
        else if constexpr(std::is_same_v<T, int16_t>)
            format = "%hd";
        else if constexpr(std::is_same_v<T, int32_t>)
            format = "%d";
        else if constexpr(std::is_same_v<T, int64_t>)
            format = "%ld";
        else if constexpr(std::is_same_v<T, float>)
            format = "%.6f";
        else if constexpr(std::is_same_v<T, double>)
            format = "%.6lf";

        ImGuiSliderFlags sliderFlags = ImGuiSliderFlags_None;

        unsigned qty = size/sizeof(T);
        if(aDesc.min.has_value() && aDesc.max.has_value())
        {
            T min = std::any_cast<T>(aDesc.min);
            T max = std::any_cast<T>(aDesc.max);
            float step = aDesc.step;

            // Logarithmic slider if range is too big
            if(max-min >= T(10000))
                sliderFlags = ImGuiSliderFlags_Logarithmic;

            if(qty == 1)
            {
                // Format for scalar
                ImGui::Text(aDesc.name.c_str());
                if(aDesc.step > 0.0f)
                    ImGui::DragScalar(imguiId.c_str(), dataType, data, step, &min, &max, format.c_str());
                else
                    ImGui::SliderScalar(imguiId.c_str(), dataType, data, &min, &max, format.c_str(), sliderFlags);
            }
            else if(qty <= 4)
            {
                // Format for small vector
                ImGui::Text(aDesc.name.c_str());

                const std::array<const char*, 4> labels = {"X", "Y", "Z", "W"};

                if(aDesc.step > 0.0f)
                {
                    for(unsigned i = 0; i < qty; i++)
                        ImGui::DragScalar((labels[i]+imguiId).c_str(), dataType, data+i, step, &min, &max, format.c_str());
                }
                else
                    for(unsigned i = 0; i < qty; i++)
                        ImGui::SliderScalar((labels[i]+imguiId).c_str(), dataType, data+i, &min, &max, format.c_str(), sliderFlags);
            }
            else
                // Format for arbitrarily long vector
                for(unsigned i = 0; i < qty; i++)
                    ImGui::SliderScalar(("Coordinate "+std::to_string(i)+imguiId).c_str(), dataType, data+i, &min, &max, format.c_str(), sliderFlags);

        }
    }

    template<typename T>
    void renderCombo(ComponentRegistry::AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)
    {
        T* data = (T*)d;

        unsigned qty = size/sizeof(T);
#ifdef ATTA_DEBUG_BUILD
        if(qty != 1)
        {
            LOG_WARN("Component", "The component attribute [w]$0[] cannot be rendered with options because it is composed of multiple [w]$1[] values", aDesc.name, typeid(T).name());
            return;
        }

        // Check if all values inside option have the same type
        for(unsigned i = 0; i < aDesc.options.size()-1; i++)
            if(aDesc.options[i].type() != aDesc.options[i+1].type())
            {
                LOG_WARN("Component", "The component attribute [w]$0[] cannot be rendered because it has entries of different types.", aDesc.name);
                return;
            }

        // Check if they are value entries
        for(unsigned i = 0; i < aDesc.options.size(); i++)
            if(aDesc.options[i].type() != typeid(const char*) && aDesc.options[i].type() != typeid(T))
            {
                LOG_WARN("Component", "The component attribute [w]$0[] cannot be rendered because the values inside its atta::ComponentRegistry::AttributeDescriptoin::options should be all of type [w]const char*[] or [w]$1[]", aDesc.name, typeid(T).name());
                return;
            }
#endif

        enum Result { INDEX_AS_RESULT, VALUE_AS_RESULT } result = INDEX_AS_RESULT;
        std::vector<std::string> valuesPreview;
        if(aDesc.options[0].type() == typeid(const char*))
        {
            for(auto value : aDesc.options)
                valuesPreview.push_back(std::string(std::any_cast<const char*>(value)));
            result = INDEX_AS_RESULT;
        }
        else if(aDesc.options[0].type() == typeid(T))
        {
            for(auto value : aDesc.options)
                valuesPreview.push_back(std::to_string(std::any_cast<T>(value)));
            result = VALUE_AS_RESULT;
        }

        uint32_t comboValue = 0;
        if(result == INDEX_AS_RESULT)
        {
            for(size_t i = 0; i < valuesPreview.size(); i++)
                if(*data == (T)i)
                    comboValue = i;
        }
        else if(result == VALUE_AS_RESULT)
        {
            for(size_t i = 0; i < valuesPreview.size(); i++)
                if(*data == std::any_cast<T>(aDesc.options[i]))
                    comboValue = i;
        }

        if(ImGui::BeginCombo((aDesc.name+"##"+imguiId).c_str(), valuesPreview[comboValue].c_str()))
        {
            for(size_t i = 0; i < valuesPreview.size(); i++)
            {
                if(ImGui::Selectable(valuesPreview[i].c_str(), comboValue == i))
                {
                    if(result == INDEX_AS_RESULT)
                        *data = (T)i;
                    else if(result == VALUE_AS_RESULT)
                        *data = std::any_cast<T>(aDesc.options[i]);
                }
                if(comboValue == i)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

    }

#define ATTA_RENDER_UI_ATTRIBUTE_NUMBER(AttaType, CppType) \
    template<>\
    void ComponentRegistry::renderUIAttribute<ComponentRegistry::AttributeType::AttaType>(ComponentRegistry::AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)\
    {\
        if(aDesc.options.size() == 0)\
            renderSliders<CppType>(aDesc, d, size, imguiId);\
        else\
            renderCombo<CppType>(aDesc, d, size, imguiId);\
    }

    ATTA_RENDER_UI_ATTRIBUTE_NUMBER(INT8, int8_t);
    ATTA_RENDER_UI_ATTRIBUTE_NUMBER(INT16, int16_t);
    ATTA_RENDER_UI_ATTRIBUTE_NUMBER(INT32, int32_t);
    ATTA_RENDER_UI_ATTRIBUTE_NUMBER(INT64, int64_t);
    ATTA_RENDER_UI_ATTRIBUTE_NUMBER(UINT8, uint8_t);
    ATTA_RENDER_UI_ATTRIBUTE_NUMBER(UINT16, uint16_t);
    ATTA_RENDER_UI_ATTRIBUTE_NUMBER(UINT32, uint32_t);
    ATTA_RENDER_UI_ATTRIBUTE_NUMBER(UINT64, uint64_t);
    ATTA_RENDER_UI_ATTRIBUTE_NUMBER(FLOAT32, float);
    ATTA_RENDER_UI_ATTRIBUTE_NUMBER(FLOAT64, double);

    template<>
    void ComponentRegistry::renderUIAttribute<ComponentRegistry::AttributeType::QUAT>(ComponentRegistry::AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)
    {
        ImGui::Text(aDesc.name.c_str());

        quat* q = (quat*)d;
        vec3 ori = degrees(q->toEuler());
        bool oriChanged = false;

        float min = 0.0f;
        float max = 1.0f;
        if(aDesc.min.has_value())
            min = std::any_cast<float>(aDesc.min);
        if(aDesc.max.has_value())
            max = std::any_cast<float>(aDesc.max);

        if(aDesc.step > 0.0f)
        {
            float step = aDesc.step;
            oriChanged |= ImGui::DragFloat(("Roll"+imguiId).c_str(), &ori.x, step, min, max, "%.2f", ImGuiSliderFlags_None);
            oriChanged |= ImGui::DragFloat(("Pitch"+imguiId).c_str(), &ori.y, step, min, max, "%.2f", ImGuiSliderFlags_None);
            oriChanged |= ImGui::DragFloat(("Yaw"+imguiId).c_str(), &ori.z, step, min, max, "%.2f", ImGuiSliderFlags_None);
        }
        else
        {
            oriChanged |= ImGui::SliderScalar(("Roll"+imguiId).c_str(), ImGuiDataType_Float, &ori.x, &min, &max, "%.2f");
            oriChanged |= ImGui::SliderScalar(("Pitch"+imguiId).c_str(), ImGuiDataType_Float, &ori.y, &min, &max, "%.2f");
            oriChanged |= ImGui::SliderScalar(("Yaw"+imguiId).c_str(), ImGuiDataType_Float, &ori.z, &min, &max, "%.2f");
        }

        if(oriChanged)
            *q = quat(radians(ori));
    }

    template<>
    void ComponentRegistry::renderUIAttribute<ComponentRegistry::AttributeType::STRINGID>(ComponentRegistry::AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)
    {
        StringHash* data = (StringHash*)d;

        if(aDesc.options.size() > 0)
        {
            std::vector<StringId> sids;
            for(auto value : aDesc.options)
                sids.emplace_back(std::string(std::any_cast<const char*>(value)));

            uint32_t comboValue = 0;
            for(size_t i = 0; i<sids.size(); i++)
                if(sids[i] == *data)
                    comboValue = i;

            const char* comboPreviewValue = sids[comboValue].getString().c_str();
            if(ImGui::BeginCombo((aDesc.name+"##"+imguiId).c_str(), comboPreviewValue ))
            {
                for(size_t i = 0; i < sids.size(); i++)
                {
                    if(ImGui::Selectable(sids[i].getString().c_str(), comboValue == i))
                        *data = sids[i].getId();
                    if(comboValue == i)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }
        else
        {
            // TODO text field (do not save StringId!!!)
        }
    }

    template<>
    void ComponentRegistry::renderUIAttribute<ComponentRegistry::AttributeType::CHAR>(ComponentRegistry::AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)
    {
        char* data = (char*)d;
        ImGui::InputText((aDesc.name+"##"+imguiId).c_str(), data, size);
    }
}
