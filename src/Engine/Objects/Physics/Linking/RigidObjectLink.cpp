#include "RigidObjectLink.h"

float gbe::RigidObjectLink::CurrentLength()
{
    Vector3 ret = objects[0]->World()->position - objects[1]->World()->position;
    return ret.Magnitude();
}
