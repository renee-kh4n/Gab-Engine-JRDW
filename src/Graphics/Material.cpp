#include "Material.h"

namespace gbe {
	using namespace rendering;

	Material::Material(Shader* m_shader)
	{
		this->m_shader = m_shader;
	}

	MaterialOverride::MaterialOverride() {

	}
}