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

gbe::rendering::DirLight::DirLight()
{
    this->shadowmap = new Framebuffer(Vector2(800, 800));
}

Light::Type gbe::rendering::ConeLight::GetType()
{
    return Light::CONE;
}
