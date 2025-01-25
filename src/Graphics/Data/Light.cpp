#include "Light.h"

using namespace gbe::gfx;

Light::Type gbe::gfx::PointLight::GetType()
{
    return Light::POINT;
}

Light::Type gbe::gfx::DirLight::GetType()
{
    return Light::DIRECTION;
}

gbe::gfx::DirLight::DirLight()
{
    this->cascade_splits = { 0.15, 0.4f };
    int map_count = this->cascade_splits.size() + 1;
    this->shadowmaps = std::vector<Framebuffer*>(map_count);
    this->cascade_projections = std::vector<Matrix4>(map_count);

    for (size_t i = 0; i < map_count; i++)
    {
        this->shadowmaps[i] = new Framebuffer(Vector2Int(512, 512));
    }
}

void gbe::gfx::DirLight::SetShadowmapResolution(int res) {
    for (size_t i = 0; i < this->cascade_splits.size() + 1; i++)
    {
        this->shadowmaps[i] = new Framebuffer(Vector2Int(res, res));
    }
}

Light::Type gbe::gfx::ConeLight::GetType()
{
    return Light::CONE;
}
