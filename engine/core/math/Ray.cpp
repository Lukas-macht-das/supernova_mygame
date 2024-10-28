//
// (c) 2024 Eduardo Doria.
//

#include "Ray.h"

#include "util/Box2DAux.h"
#include "util/JoltPhysicsAux.h"
#include <stdlib.h>

using namespace Supernova;


const RayReturn Ray::NO_HIT( {false, -1, Vector3::ZERO, Vector3::ZERO, NULL_ENTITY, 0} );

Ray::Ray(){

}

Ray::Ray(const Ray &ray){
    (*this) = ray;
}

Ray::Ray(Vector3 origin, Vector3 direction){
    this->origin = origin;
    this->direction = direction;
}

Ray& Ray::operator=(const Ray &r){

    this->origin = r.origin;
    this->direction = r.direction;

    return *this;
}

Vector3 Ray::operator*(float t){
    return getPoint(t);
}

void Ray::setOrigin(Vector3 origin){
    this->origin = origin;
}

Vector3 Ray::getOrigin() const{
    return origin;
}

void Ray::setDirection(Vector3 direction){
    this->direction = direction;
}

Vector3 Ray::getDirection() const{
    return direction;
}

Vector3 Ray::getPoint(float distance){
    return Vector3(origin + (direction * distance));
}

RayReturn Ray::intersects(Plane plane) {
    float denom = plane.normal.dotProduct(getDirection());

    if (abs(denom) < std::numeric_limits<float>::epsilon()) {
        // when plane is parallel
        return NO_HIT;
    }else{
        float nom = plane.normal.dotProduct(getOrigin()) + plane.d;
        float dist = -(nom/denom);
        if ((dist >= 0) && (dist <= 1)) // anything beyond this length will not be a hit
            return {true, dist, getPoint(dist), plane.normal, NULL_ENTITY, 0};
    }

    return NO_HIT;
}

RayReturn Ray::intersects(AABB box){

    if (box.isNull()) return NO_HIT;
    if (box.isInfinite()) return NO_HIT;

    float lowt = 0.0f;
    float t;
    bool hit = false;
    Vector3 hitpoint;
    const Vector3& min = box.getMinimum();
    const Vector3& max = box.getMaximum();
    const Vector3& rayorig = getOrigin();
    const Vector3& raydir = getDirection();

    // inside box
    if ( rayorig > min && rayorig < max ) {
        return {true, 0, getPoint(0), Vector3::ZERO, NULL_ENTITY, 0};
    }

    // Min x
    if (rayorig.x <= min.x && raydir.x > 0) {
        t = (min.x - rayorig.x) / raydir.x;

        hitpoint = rayorig + raydir * t;
        if (hitpoint.y >= min.y && hitpoint.y <= max.y && hitpoint.z >= min.z && hitpoint.z <= max.z && (!hit || t < lowt)) {
            hit = true;
            lowt = t;
        }
    }
    // Max x
    if (rayorig.x >= max.x && raydir.x < 0) {
        t = (max.x - rayorig.x) / raydir.x;

        hitpoint = rayorig + raydir * t;
        if (hitpoint.y >= min.y && hitpoint.y <= max.y && hitpoint.z >= min.z && hitpoint.z <= max.z && (!hit || t < lowt)) {
            hit = true;
            lowt = t;
        }
    }
    // Min y
    if (rayorig.y <= min.y && raydir.y > 0) {
        t = (min.y - rayorig.y) / raydir.y;

        hitpoint = rayorig + raydir * t;
        if (hitpoint.x >= min.x && hitpoint.x <= max.x && hitpoint.z >= min.z && hitpoint.z <= max.z && (!hit || t < lowt)) {
            hit = true;
            lowt = t;
        }
    }
    // Max y
    if (rayorig.y >= max.y && raydir.y < 0) {
        t = (max.y - rayorig.y) / raydir.y;

        hitpoint = rayorig + raydir * t;
        if (hitpoint.x >= min.x && hitpoint.x <= max.x && hitpoint.z >= min.z && hitpoint.z <= max.z && (!hit || t < lowt)) {
            hit = true;
            lowt = t;
        }
    }
    // Min z
    if (rayorig.z <= min.z && raydir.z > 0) {
        t = (min.z - rayorig.z) / raydir.z;

        hitpoint = rayorig + raydir * t;
        if (hitpoint.x >= min.x && hitpoint.x <= max.x && hitpoint.y >= min.y && hitpoint.y <= max.y && (!hit || t < lowt)) {
            hit = true;
            lowt = t;
        }
    }
    // Max z
    if (rayorig.z >= max.z && raydir.z < 0) {
        t = (max.z - rayorig.z) / raydir.z;

        hitpoint = rayorig + raydir * t;
        if (hitpoint.x >= min.x && hitpoint.x <= max.x && hitpoint.y >= min.y && hitpoint.y <= max.y && (!hit || t < lowt)) {
            hit = true;
            lowt = t;
        }
    }

    if (hit)
        if ((lowt >= 0) && (lowt <= 1)){ // anything beyond this length will not be a hit

            Vector3 point = getPoint(lowt);
            Vector3 normal = Vector3::ZERO;

            if (point.x == min.x)
                normal = Vector3(-1, 0, 0); // Left face
            if (point.x == max.x)
                normal = Vector3(1, 0, 0); // Right face
            if (point.y == min.y)
                normal = Vector3(0, -1, 0); // Bottom face
            if (point.y == max.y)
                normal = Vector3(0, 1, 0); // Top face
            if (point.z == min.z)
                normal = Vector3(0, 0, -1); // Front face
            if (point.z == max.z)
                normal = Vector3(0, 0, 1); // Back face

            return {true, lowt, point, normal, NULL_ENTITY, 0};
        }

    return NO_HIT;
}

RayReturn Ray::intersects(Sphere sphere) {
    Vector3 rayorig = origin - sphere.center;
    float radius = sphere.radius;

    // discard inside sphere
    //if (rayorig.squaredLength() <= radius*radius && discardInside){
    //    return {true, 0, Vector3::ZERO, Vector3::ZERO, NULL_ENTITY, 0};
    //}

    float a = direction.dotProduct(direction);
    float b = 2 * rayorig.dotProduct(direction);
    float c = rayorig.dotProduct(rayorig) - radius * radius;

    // determinant
    float d = (b * b) - (4 * a * c);
    if (d >= 0){
        float t = ( -b - sqrt(d) ) / (2 * a);
        if (t < 0){
            t = ( -b + sqrt(d) ) / (2 * a);
        }

        Vector3 point = getPoint(t);
        Vector3 normal = (point - sphere.center).normalize();

        return {true, t, point, normal, NULL_ENTITY, 0};
    }

    return NO_HIT;
}

RayReturn Ray::intersects(Body2D body){
    Body2DComponent& bodycomp = body.getComponent<Body2DComponent>();

    float ptmScale = body.getPointsToMeterScale();

    Vector3 end = origin + direction;
    b2Vec2 bOrigin = {origin.x / ptmScale, origin.y / ptmScale};
    b2Vec2 bEnd = {end.x / ptmScale, end.y / ptmScale};

    b2Vec2 translation = b2Sub(bEnd, bOrigin);

    const int shapesCount = b2Body_GetShapeCount(bodycomp.body);
    std::vector<b2ShapeId> shapeIds(shapesCount);
    int returnCount = b2Body_GetShapes(bodycomp.body, &shapeIds.front(), shapesCount);

    float closestFraction = FLT_MAX;
    b2Vec2 intersectionPoint = {0, 0};
    b2Vec2 intersectionNormal = {0, 0};
    size_t shapeIndex = 0;
    for (int i = 0; i < returnCount; ++i){
        b2ShapeId shapeid = shapeIds[i];
        const b2RayCastInput rayCastInput = {bOrigin, translation, 1.0};
        b2CastOutput castOutput = b2Shape_RayCast(shapeid, &rayCastInput);

        if (castOutput.hit){
            closestFraction = castOutput.fraction;
            intersectionPoint = castOutput.point;
            intersectionNormal = castOutput.normal;
            shapeIndex = reinterpret_cast<size_t>(b2Shape_GetUserData(shapeid));
        }
    }

    if (closestFraction >= 0 && closestFraction <= 1){
        Vector3 point = Vector3(intersectionPoint.x * ptmScale, intersectionPoint.y * ptmScale, 0);
        Vector3 normal = Vector3(intersectionNormal.x, intersectionNormal.y, 0);

        return {true, closestFraction, point, normal, body.getEntity(), shapeIndex};
    }

    return NO_HIT;
}

RayReturn Ray::intersects(Body2D body, size_t shape){
    Body2DComponent& bodycomp = body.getComponent<Body2DComponent>();

    float ptmScale = body.getPointsToMeterScale();

    Vector3 end = origin + direction;
    b2Vec2 bOrigin = {origin.x / ptmScale, origin.y / ptmScale};
    b2Vec2 bEnd = {end.x / ptmScale, end.y / ptmScale};

    b2Vec2 translation = b2Sub(bEnd, bOrigin);

    b2ShapeId shapeid = bodycomp.shapes[shape].shape;
    const b2RayCastInput rayCastInput = {bOrigin, translation, 1.0};
    b2CastOutput castOutput = b2Shape_RayCast(shapeid, &rayCastInput);

    if (castOutput.hit){
        Vector3 point = Vector3(castOutput.point.x * ptmScale, castOutput.point.y * ptmScale, 0);
        Vector3 normal = Vector3(castOutput.normal.x, castOutput.normal.y, 0);

        return {true, castOutput.fraction, point, normal, body.getEntity(), shape};
    }

    return NO_HIT;
}

RayReturn Ray::intersects(Body3D body){
    Body3DComponent& bodycomp = body.getComponent<Body3DComponent>();
    std::shared_ptr<PhysicsSystem> physicsSystem = body.getScene()->getSystem<PhysicsSystem>();
    JPH::PhysicsSystem* world = physicsSystem->getWorld3D();

    if (!bodycomp.body.IsInvalid()){
        JPH::RayCast ray(JPH::Vec3(origin.x, origin.y, origin.z), JPH::Vec3(direction.x, direction.y, direction.z));
        JPH::SubShapeIDCreator id_creator;
        JPH::RayCastResult hit;

        JPH::ShapeRefC shapeRef = world->GetBodyInterface().GetShape(bodycomp.body);

        JPH::Vec3 normal = shapeRef->GetSurfaceNormal(hit.mSubShapeID2, ray.GetPointOnRay(hit.mFraction));

        size_t shapeIndex = shapeRef->GetSubShapeUserData(hit.mSubShapeID2);

        if (shapeRef->CastRay(ray, id_creator, hit)){
            return {true, hit.mFraction, getPoint(hit.mFraction), Vector3(normal.GetX(), normal.GetY(), normal.GetZ()), body.getEntity(), shapeIndex};
        }
    }

    return NO_HIT;
}

RayReturn Ray::intersects(Body3D body, size_t shape){
    Body3DComponent& bodycomp = body.getComponent<Body3DComponent>();

    if (!bodycomp.body.IsInvalid()){
        JPH::RayCast ray(JPH::Vec3(origin.x, origin.y, origin.z), JPH::Vec3(direction.x, direction.y, direction.z));
        JPH::SubShapeIDCreator id_creator;
        JPH::RayCastResult hit;

        if (shape < bodycomp.numShapes){
            if (bodycomp.shapes[shape].shape->CastRay(ray, id_creator, hit)){
                JPH::Vec3 normal = bodycomp.shapes[shape].shape->GetSurfaceNormal(hit.mSubShapeID2, ray.GetPointOnRay(hit.mFraction));

                return {true, hit.mFraction, getPoint(hit.mFraction), Vector3(normal.GetX(), normal.GetY(), normal.GetZ()), body.getEntity(), shape};
            }
        }
    }

    return NO_HIT;
}

RayReturn Ray::intersects(Scene* scene, RayFilter raytest){
    return intersects(scene, raytest, false);
}

RayReturn Ray::intersects(Scene* scene, RayFilter raytest, bool onlyStatic){
    return intersects(scene, raytest, onlyStatic, (uint16_t)~0u, (uint16_t)~0u);
}

RayReturn Ray::intersects(Scene* scene, RayFilter raytest, uint16_t categoryBits, uint16_t maskBits){
    return intersects(scene, raytest, false, categoryBits, maskBits);
}

RayReturn Ray::intersects(Scene* scene, RayFilter raytest, bool onlyStatic, uint16_t categoryBits, uint16_t maskBits){
    if (raytest == RayFilter::BODY_2D){

        b2WorldId world = scene->getSystem<PhysicsSystem>()->getWorld2D();

        float ptmScale = scene->getSystem<PhysicsSystem>()->getPointsToMeterScale2D();

        Vector3 end = origin + direction;
        b2Vec2 bOrigin = {origin.x / ptmScale, origin.y / ptmScale};
        b2Vec2 bEnd = {end.x / ptmScale, end.y / ptmScale};

        b2Vec2 translation = b2Sub(bEnd, bOrigin);

        b2QueryFilter filter;
        filter.categoryBits = categoryBits;
        filter.maskBits = maskBits;

        std::vector<Box2DWorldRayCastOutput> outputs;

        Box2DWorldRayCastContext context = {&outputs, onlyStatic};

        b2World_CastRay(world, bOrigin, translation, filter, Box2DAux::CastCallback, &context);

        float closestFraction = FLT_MAX;
        b2Vec2 intersectionPoint = {0, 0};
        b2Vec2 intersectionNormal = {0, 0};
        Entity entity = NULL_ENTITY;
        size_t shapeIndex = 0;
        for (size_t i = 0; i < outputs.size(); i++){
                if ( outputs[i].fraction < closestFraction ) {
                    closestFraction = outputs[i].fraction;
                    intersectionPoint = outputs[i].point;
                    intersectionNormal = outputs[i].normal;
                    entity = reinterpret_cast<uintptr_t>(b2Body_GetUserData(outputs[i].bodyId));
                    shapeIndex = reinterpret_cast<size_t>(b2Shape_GetUserData(outputs[i].shapeId));
                }
        }

        if (closestFraction >= 0 && closestFraction <= 1){
            Vector3 point = Vector3(intersectionPoint.x * ptmScale, intersectionPoint.y * ptmScale, 0);
            Vector3 normal = Vector3(intersectionNormal.x, intersectionNormal.y, 0);

            return {true, closestFraction, point, normal, entity, shapeIndex};
        }

    }else if (raytest == RayFilter::BODY_3D){

        JPH::PhysicsSystem* world = scene->getSystem<PhysicsSystem>()->getWorld3D();

        if (world){
            JPH::RayCast ray(JPH::Vec3(origin.x, origin.y, origin.z), JPH::Vec3(direction.x, direction.y, direction.z));
            JPH::RayCastResult hit;

            JPH::ObjectLayer objectLayer = JPH::ObjectLayerPairFilterMask::sGetObjectLayer(categoryBits, maskBits);

            if (world->GetNarrowPhaseQuery().CastRay(JPH::RRayCast(ray), hit, { }, JPH::DefaultObjectLayerFilter(JPH::ObjectLayerPairFilterMask(), objectLayer), OnlyStaticBodyFilter(onlyStatic))){
                JPH::Vec3 normal;
                Entity entity = NULL_ENTITY;
                size_t shapeIndex = 0;
                JPH::BodyLockRead lock(world->GetBodyLockInterface(), hit.mBodyID);
                if (lock.Succeeded()){
                    const JPH::Body &hit_body = lock.GetBody();
                    normal = hit_body.GetWorldSpaceSurfaceNormal(hit.mSubShapeID2, ray.GetPointOnRay(hit.mFraction));
                    entity = hit_body.GetUserData();
                    shapeIndex = hit_body.GetShape()->GetSubShapeUserData(hit.mSubShapeID2);
                }

                return {true, hit.mFraction, getPoint(hit.mFraction), Vector3(normal.GetX(), normal.GetY(), normal.GetZ()), entity, shapeIndex};
            }
        }

    }

    return NO_HIT;
}

RayReturn Ray::intersects(Scene* scene, uint8_t broadPhaseLayer3D){
    return intersects(scene, broadPhaseLayer3D, (uint16_t)~0u, (uint16_t)~0u);
}

RayReturn Ray::intersects(Scene* scene, uint8_t broadPhaseLayer3D, uint16_t categoryBits, uint16_t maskBits){
    std::shared_ptr<PhysicsSystem> physicsSystem = scene->getSystem<PhysicsSystem>();
    JPH::PhysicsSystem* world = physicsSystem->getWorld3D();

    if (world && broadPhaseLayer3D < MAX_BROADPHASELAYER_3D){
        JPH::RayCast ray(JPH::Vec3(origin.x, origin.y, origin.z), JPH::Vec3(direction.x, direction.y, direction.z));
        JPH::RayCastResult hit;

        JPH::ObjectLayer objectLayer = JPH::ObjectLayerPairFilterMask::sGetObjectLayer(categoryBits, maskBits);

        if (world->GetNarrowPhaseQuery().CastRay(JPH::RRayCast(ray), hit, JPH::SpecifiedBroadPhaseLayerFilter(JPH::BroadPhaseLayer(broadPhaseLayer3D)), JPH::DefaultObjectLayerFilter(JPH::ObjectLayerPairFilterMask(), objectLayer))){
            JPH::Vec3 normal;
            Entity entity = NULL_ENTITY;
            size_t shapeIndex = 0;

            const JPH::BodyLockInterface& bodyLockInterface = physicsSystem->isLock3DBodies()? static_cast<const JPH::BodyLockInterface &>(world->GetBodyLockInterface()) : static_cast<const JPH::BodyLockInterface &>(world->GetBodyLockInterfaceNoLock());

            JPH::BodyLockRead lock(bodyLockInterface, hit.mBodyID);
            if (lock.Succeeded()){
                const JPH::Body &hit_body = lock.GetBody();
                normal = hit_body.GetWorldSpaceSurfaceNormal(hit.mSubShapeID2, ray.GetPointOnRay(hit.mFraction));
                entity = hit_body.GetUserData();
                shapeIndex = hit_body.GetShape()->GetSubShapeUserData(hit.mSubShapeID2);
            }

            return {true, hit.mFraction, getPoint(hit.mFraction), Vector3(normal.GetX(), normal.GetY(), normal.GetZ()), entity, shapeIndex};
        }
    }

    return NO_HIT;
}