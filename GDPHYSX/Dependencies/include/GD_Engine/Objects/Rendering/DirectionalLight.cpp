#include "DirectionalLight.h"

using namespace gde;

rendering::Light* gde::DirectionalLight::GetData()
{
    if (this->mLight == nullptr)
        this->mLight = new rendering::DirLight();

    auto dir_light = (rendering::DirLight*)this->mLight;

    dir_light->color = this->Color;
    dir_light->intensity = this->intensity;
    dir_light->dir = this->World()->Forward;

    return this->mLight;
}
