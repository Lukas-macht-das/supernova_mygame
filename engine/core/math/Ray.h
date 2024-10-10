#ifndef ray_h
#define ray_h

#include "math/Vector3.h"
#include "math/Plane.h"
#include "math/AABB.h"
#include "object/physics/Body2D.h"
#include "object/physics/Body3D.h"

namespace Supernova {

    enum class RayFilter{
        BODY_2D,
        BODY_3D
    };

    struct RayReturn{
        bool hit;
        float distance;
        Vector3 point;
        Vector3 normal;
        Entity body;
        size_t shapeIndex;

        operator bool() const {
            return hit;
        }
    };

    class Ray{

    private:

        Vector3 origin;
        Vector3 direction; // direction and length of the ray (anything beyond this length will not be a hit)

    public:

        static const RayReturn NO_HIT;

        Ray();
        Ray(const Ray &ray);
        Ray(Vector3 origin, Vector3 direction);

        Ray &operator=(const Ray &);
        Vector3 operator*(float t);

        void setOrigin(Vector3 point);
        Vector3 getOrigin() const;

        void setDirection(Vector3 direction);
        Vector3 getDirection() const;

        Vector3 getPoint(float distance);

        RayReturn intersects(Plane plane);
        RayReturn intersects(AABB box);
        RayReturn intersects(Body2D body);
        RayReturn intersects(Body2D body, size_t shape);
        RayReturn intersects(Body3D body);
        RayReturn intersects(Body3D body, size_t shape);
        RayReturn intersects(Scene* scene, RayFilter raytest);
        RayReturn intersects(Scene* scene, RayFilter raytest, bool onlyStatic);
        RayReturn intersects(Scene* scene, RayFilter raytest, uint16_t categoryBits, uint16_t maskBits);
        RayReturn intersects(Scene* scene, RayFilter raytest, bool onlyStatic, uint16_t categoryBits, uint16_t maskBits);
        RayReturn intersects(Scene* scene, uint8_t broadPhaseLayer3D); // only 3D bodies
        RayReturn intersects(Scene* scene, uint8_t broadPhaseLayer3D, uint16_t categoryBits, uint16_t maskBits); // only 3D bodies
    };
    
}


#endif /* ray_h */
