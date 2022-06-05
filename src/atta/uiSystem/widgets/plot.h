//--------------------------------------------------
// Atta UI System
// plot.h
// Date: 2022-05-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_WIDGETS_PLOT_H
#define ATTA_UI_SYSTEM_WIDGETS_PLOT_H

namespace atta::ui
{
    struct InfoOneLine 
    {
        std::vector<float> x;
        std::vector<float> y;
        vec4 color;
        vec4 bgColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
        float width = -1;
        float height = -1;
    };

    struct InfoMultiLine 
    {
        std::vector<std::vector<float>> x;
        std::vector<std::vector<float>> y;
        std::vector<vec4> color;
        vec4 bgColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
        float width = -1;
        float height = -1;
    };

    void plot(InfoOneLine info);
    void plot(InfoMultiLine info);
}

#endif// ATTA_UI_SYSTEM_WIDGETS_PLOT_H
