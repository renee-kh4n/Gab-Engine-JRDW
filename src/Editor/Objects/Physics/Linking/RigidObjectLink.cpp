#include "RigidObjectLink.h"

float gde::RigidObjectLink::CurrentLength()
{
    Vector3 ret = objects[0]->World()->position - objects[1]->World()->position;
    return ret.Magnitude();
}
