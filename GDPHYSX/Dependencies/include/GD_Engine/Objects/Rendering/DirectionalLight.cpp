#include "DirectionalLight.h"

using namespace gde;
using namespace rendering;

rendering::Light* gde::DirectionalLight::GetData()
{
    if (this->mLight == nullptr)
        this->mLight = new DirLight();

    auto dir_light = (DirLight*)this->mLight;

    dir_light->color = this->Color;
    dir_light->intensity = this->intensity;
    dir_light->dir = this->World()->Forward;

    return this->mLight;
}
