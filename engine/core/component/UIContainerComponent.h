//
// (c) 2024 Eduardo Doria.
//

#ifndef UI_CONTAINER_COMPONENT_H
#define UI_CONTAINER_COMPONENT_H

#define MAX_CONTAINER_BOXES 30

#include "math/Rect.h"

namespace Supernova{

    enum class ContainerType{
        VERTICAL,
        HORIZONTAL,
        VERTICAL_WRAP,
        HORIZONTAL_WRAP
    };

    struct ContainerBox{
        Entity layout = NULL_ENTITY;
        Rect rect = Rect(0,0,0,0);
        bool expand = true;
    };

    struct SUPERNOVA_API UIContainerComponent{
        ContainerType type = ContainerType::VERTICAL;

        ContainerBox boxes[MAX_CONTAINER_BOXES];

        int numBoxes = 0;
        int fixedWidth = 0;
        int fixedHeight = 0;
        int maxWidth = 0;
        int maxHeight = 0;
        int numBoxExpand = 0;
    };
    
}

#endif //UI_CONTAINER_COMPONENT_H
