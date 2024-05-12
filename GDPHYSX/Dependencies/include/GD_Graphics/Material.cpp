#include "Material.h"

namespace gde {
	Material::Material(Shader* m_shader)
	{
		this->m_shader = m_shader;
	}
}