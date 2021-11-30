//--------------------------------------------------
// Atta Component System
// componentRegistry.cpp
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/math/math.h>
#include <atta/componentSystem/componentManager.h>
#include <imgui.h>

namespace atta
{
    void ComponentRegistry::registerToComponentManager()
    {
        ComponentManager::registerComponent(this);
    }

    unsigned ComponentRegistry::getSerializedSize(Component* component)
    {
        std::ostringstream of;
        std::basic_ostream<char>::pos_type posBefore = of.tellp();
        serialize(of, component);
        ASSERT(of.tellp() != -1, "Failed to calculate componente serialized size")
        unsigned size = (int)of.tellp() - posBefore;
        return size;
    }

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
		bool undefinedMinMax = false;
		T min, max;

        if(aDesc.min.has_value())
            min = std::any_cast<T>(aDesc.min);
		else
		{
			undefinedMinMax = true;
			min = std::numeric_limits<T>::lowest();
			if constexpr(std::is_same_v<T, float>)
				min /= 2.0f;
		}

		if(aDesc.max.has_value())
            max = std::any_cast<T>(aDesc.max);
		else
		{
			undefinedMinMax = true;
			max = std::numeric_limits<T>::max();
			if constexpr(std::is_same_v<T, float>)
				max /= 2.0f;
		}

		float step = aDesc.step > 0 ? aDesc.step : std::numeric_limits<T>::epsilon();

		// Logarithmic slider if range is too big
		if(max-min >= T(10000) && !undefinedMinMax)
			sliderFlags = ImGuiSliderFlags_Logarithmic;

		// Format for scalar
		ImGui::Text(aDesc.name.c_str());

		if(qty == 1)
		{
			if(aDesc.step > 0.0f || undefinedMinMax )
				ImGui::DragScalar(imguiId.c_str(), dataType, data, step, &min, &max, format.c_str());
			else
				ImGui::SliderScalar(imguiId.c_str(), dataType, data, &min, &max, format.c_str(), sliderFlags);
		}
		else if(qty <= 4)
		{
			const std::array<const char*, 4> labels = {"X", "Y", "Z", "W"};

			if(aDesc.step > 0.0f || undefinedMinMax)
				for(unsigned i = 0; i < qty; i++)
					ImGui::DragScalar((labels[i]+imguiId).c_str(), dataType, data+i, step, &min, &max, format.c_str());
			else
				for(unsigned i = 0; i < qty; i++)
					ImGui::SliderScalar((labels[i]+imguiId).c_str(), dataType, data+i, &min, &max, format.c_str(), sliderFlags);
		}
		else
		{
			// Format for arbitrarily long vector
			if(aDesc.step > 0.0f || undefinedMinMax)
				for(unsigned i = 0; i < qty; i++)
					ImGui::DragScalar((std::to_string(i)+imguiId).c_str(), dataType, data+i, step, &min, &max, format.c_str());
			else
				for(unsigned i = 0; i < qty; i++)
					ImGui::SliderScalar((std::to_string(i)+imguiId).c_str(), dataType, data+i, &min, &max, format.c_str(), sliderFlags);
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
        //for(auto it = aDesc.options.begin(); it != aDesc.options.end(); it++)
        //    if(it->type() != (it+1)->type())
        //    {
        //        LOG_WARN("Component", "The component attribute [w]$0[] cannot be rendered because it has entries of different types.", aDesc.name);
        //        return;
        //    }

        // Check if they are value entries
        for(auto it = aDesc.options.begin(); it != aDesc.options.end(); it++)
            if(it->type() != typeid(const char*) && it->type() != typeid(T))
            {
                LOG_WARN("Component", "The component attribute [w]$0[] cannot be rendered because the values inside its atta::ComponentRegistry::AttributeDescriptoin::options should be all of type [w]const char*[] or [w]$1[]", aDesc.name, typeid(T).name());
                return;
            }
#endif

        enum Result { INDEX_AS_RESULT, VALUE_AS_RESULT } result = INDEX_AS_RESULT;
        std::vector<std::string> valuesPreview;
        if(aDesc.options.begin()->type() == typeid(const char*))
        {
            for(auto value : aDesc.options)
                valuesPreview.push_back(std::string(std::any_cast<const char*>(value)));
            result = INDEX_AS_RESULT;
        }
        else if(aDesc.options.begin()->type() == typeid(T))
        {
            for(auto value : aDesc.options)
                valuesPreview.push_back(std::to_string(std::any_cast<T>(value)));
            result = VALUE_AS_RESULT;
        }

        int comboValue = -1;
        if(result == INDEX_AS_RESULT)
        {
            for(size_t i = 0; i < valuesPreview.size(); i++)
                if(*data == (T)i)
                    comboValue = i;
        }
        else if(result == VALUE_AS_RESULT)
        {
            auto it = aDesc.options.begin();
            for(size_t i = 0; i < valuesPreview.size(); i++, it++)
                if(*data == std::any_cast<T>(*it))
                    comboValue = i;
        }
        if(comboValue == -1)
        {
            comboValue = 0;
            *data = std::any_cast<T>(*aDesc.options.begin());
        }

        if(ImGui::BeginCombo((aDesc.name+"##"+imguiId).c_str(), valuesPreview[comboValue].c_str()))
        {
            auto it = aDesc.options.begin();
            for(size_t i = 0; i < valuesPreview.size(); i++, it++)
            {
                if(ImGui::Selectable(valuesPreview[i].c_str(), (size_t)comboValue == i))
                {
                    if(result == INDEX_AS_RESULT)
                        *data = (T)i;
                    else if(result == VALUE_AS_RESULT)
                        *data = std::any_cast<T>(*it);
                }
                if((size_t)comboValue == i)
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
                sids.emplace_back(std::any_cast<StringId>(value).getString());

            int comboValue = -1;
            for(size_t i = 0; i < sids.size(); i++)
                if(sids[i] == *data)
                    comboValue = i;
            if(comboValue == -1 && sids.size())
            {
                comboValue = 0;
                *data = sids[0].getId();
            }

            const char* comboPreviewValue = sids[comboValue].getString().c_str();
            if(ImGui::BeginCombo((aDesc.name+"##"+imguiId).c_str(), comboPreviewValue ))
            {
                for(size_t i = 0; i < sids.size(); i++)
                {
                    if(ImGui::Selectable(sids[i].getString().c_str(), comboValue == (int)i))
                        *data = sids[i].getId();
                    if(comboValue == (int)i)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }
        else
        {
            // TODO text field (do not save StringId!)
        }
    }

    template<>
    void ComponentRegistry::renderUIAttribute<ComponentRegistry::AttributeType::BOOL>(ComponentRegistry::AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)
    {
        bool* data = (bool*)d;
        ImGui::Checkbox((aDesc.name+"##"+imguiId).c_str(), data);
    }

    template<>
    void ComponentRegistry::renderUIAttribute<ComponentRegistry::AttributeType::CHAR>(ComponentRegistry::AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)
    {
        char* data = (char*)d;
        ImGui::InputText((aDesc.name+"##"+imguiId).c_str(), data, size);
    }
}
