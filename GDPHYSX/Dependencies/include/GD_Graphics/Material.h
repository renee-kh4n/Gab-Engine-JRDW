#pragma once

#include <unordered_map>
#include <string>
#include "Shader.h"
#include "Texture.h"

namespace gde {
	enum MaterialOverrideType {
		BOOL,
		FLOAT,
		VEC2,
		VEC3,
		MAT4
	};

	struct MaterialOverride {
		MaterialOverrideType type;

		union {
			bool value_bool;
			float value_float;
			glm::vec2 value_vec2;
			glm::vec3 value_vec3;
			glm::mat4 value_mat4;
		};
	};

	struct MaterialTexture {
		Texture* texture;
		const char* parameterName;
	};

	struct Material {
		std::unordered_map<const char*, MaterialOverride> overrides;
		std::vector<MaterialTexture> textureOverrides;
		Shader* m_shader;

		Material(Shader*);
			
		template <typename TValue>
		void setOverride(const char* id, TValue value) {}

		template<>
		void setOverride<bool>(const char* id, bool value) {
			auto materialOverride = MaterialOverride();
			materialOverride.type = MaterialOverrideType::BOOL;
			materialOverride.value_bool = value;

			overrides.insert_or_assign(id, materialOverride);
		}
		template<>
		void setOverride<float>(const char* id, float value) {
			auto materialOverride = MaterialOverride();
			materialOverride.type = MaterialOverrideType::FLOAT;
			materialOverride.value_float = value;

			overrides.insert_or_assign(id, materialOverride);
		}
		template<>
		void setOverride<glm::vec2>(const char* id, glm::vec2 value) {
			auto materialOverride = MaterialOverride();
			materialOverride.type = MaterialOverrideType::VEC2;
			materialOverride.value_vec2 = value;

			overrides.insert_or_assign(id, materialOverride);
		}
		template<>
		void setOverride<glm::vec3>(const char* id, glm::vec3 value) {
			auto materialOverride = MaterialOverride();
			materialOverride.type = MaterialOverrideType::VEC3;
			materialOverride.value_vec3 = value;

			overrides.insert_or_assign(id, materialOverride);
		}
		template<>
		void setOverride<glm::mat4>(const char* id, glm::mat4 value) {
			auto materialOverride = MaterialOverride();
			materialOverride.type = MaterialOverrideType::MAT4;
			materialOverride.value_mat4 = value;

			overrides.insert_or_assign(id, materialOverride);
		}
	};
}