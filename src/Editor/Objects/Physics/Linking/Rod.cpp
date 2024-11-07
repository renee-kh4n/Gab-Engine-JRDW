#include "Rod.h"

gde::CollisionContact* gde::Rod::GetContact()
{
    auto p0 = this->objects[0]->World()->position;
    auto p1 = this->objects[1]->World()->position;
    Vector3 delta_p = p1 - p0;
    auto delta_mag = delta_p.Magnitude();

    if (delta_mag == length)
        return nullptr;

    CollisionContact* ret = new CollisionContact();
    ret->objects[0] = this->objects[0];
    ret->objects[1] = this->objects[1];

    Vector3 dir = delta_p * (1.0f / delta_mag);

    if (delta_mag > length) {
        ret->contactNormal = dir;
        ret->depth = delta_mag - length;
    }
    else {
        ret->contactNormal = -dir;
        ret->depth = length - delta_mag;
    }

    ret->restitution = this->restitution;
    return ret;
}
