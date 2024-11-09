#include "Light.h"

using namespace gbe::rendering;

Light::Type gbe::rendering::PointLight::GetType()
{
    return Light::POINT;
}

Light::Type gbe::rendering::DirLight::GetType()
{
    return Light::DIRECTION;
}

Light::Type gbe::rendering::ConeLight::GetType()
{
    return Light::CONE;
}
