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
    this->cascade_splits = { 0.07, 0.3f, 0.5f };
    int map_count = this->cascade_splits.size() + 1;
    this->shadowmaps = std::vector<Framebuffer*>(map_count);
    this->cascade_projections = std::vector<Matrix4>(map_count);

    for (size_t i = 0; i < map_count; i++)
    {
        this->shadowmaps[i] = new Framebuffer(Vector2Int(512, 512));
    }
}

void gbe::rendering::DirLight::SetShadowmapResolution(int res) {
    for (size_t i = 0; i < this->cascade_splits.size() + 1; i++)
    {
        this->shadowmaps[i] = new Framebuffer(Vector2Int(res, res));
    }
}

Light::Type gbe::rendering::ConeLight::GetType()
{
    return Light::CONE;
}
