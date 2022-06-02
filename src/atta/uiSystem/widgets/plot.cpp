//--------------------------------------------------
// Atta UI System
// plot.cpp
// Date: 2022-05-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/widgets/plot.h>

namespace atta::ui
{
    void plot(InfoOneLine info)
    {
        InfoMultiLine infom;
        if(info.x.size())
        {
            infom.x.resize(1);
            infom.x[0] = info.x;
        }
        infom.y.resize(1);
        infom.y[0] = info.y;
        infom.color.resize(1);
        infom.color[0] = info.color;
        infom.bgColor = info.bgColor;
        infom.height = info.height;
        infom.width = info.width;
        plot(infom);
    }

    void plot(InfoMultiLine info)
    {

    }
}
