#include "Light.h"

using namespace gde::rendering;

Light::Type gde::rendering::PointLight::GetType()
{
    return Light::POINT;
}

Light::Type gde::rendering::DirLight::GetType()
{
    return Light::DIRECTION;
}

Light::Type gde::rendering::ConeLight::GetType()
{
    return Light::CONE;
}