//
// (c) 2024 Eduardo Doria.
//

#ifndef FOG_COMPONENT_H
#define FOG_COMPONENT_H

#include "util/Color.h"

namespace Supernova{

    enum class FogType{
        LINEAR,
		EXPONENTIAL,
		EXPONENTIALSQUARED
    };

    struct SUPERNOVA_API FogComponent{
        FogType type = FogType::LINEAR;
		Vector3 color = Color::sRGBToLinear(Vector3(0.8f, 0.8f, 0.8f)); // linear color
		float density = 0.01f; // for exponential fog
		float linearStart = 10;
		float linearEnd = 60;
    };
    
}

#endif //FOG_COMPONENT_H