//
// (c) 2024 Eduardo Doria.
//

#ifndef SCROLLBAR_COMPONENT_H
#define SCROLLBAR_COMPONENT_H

#include "util/FunctionSubscribe.h"

namespace Supernova{

    enum class ScrollbarType{
        VERTICAL,
        HORIZONTAL
    };

    struct SUPERNOVA_API ScrollbarComponent{
        Entity bar = NULL_ENTITY;
        ScrollbarType type = ScrollbarType::VERTICAL;

        FunctionSubscribe<void(float)> onChange;

        float barSize = 0.5;  // from 0 to 1
        float step = 0; // from 0 to 1

        bool barPointerDown = false;
        float barPointerPos = -1;

        bool needUpdateScrollbar = true;
    };

}

#endif //SCROLLBAR_COMPONENT_H