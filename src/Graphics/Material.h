#pragma once

#include <unordered_map>
#include <string>
#include "Shader.h"
#include "Texture.h"
#include "../Datatypes/Matrix4.h"
#include "../Datatypes/Vector2.h"
#include "../Datatypes/Vector3.h"

namespace gbe {
	namespace rendering {
		enum MaterialOverrideType {
			BOOL,
			FLOAT,
			VEC2,
			VEC3,
			MAT4
		};

		struct MaterialOverride {
			MaterialOverrideType type;

			MaterialOverride();

			union {
				bool value_bool;
				float value_float;
				Vector2 value_vec2;
				Vector3 value_vec3;
				Matrix4 value_mat4;
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
			void setOverride<Vector2>(const char* id, Vector2 value) {
				auto materialOverride = MaterialOverride();
				materialOverride.type = MaterialOverrideType::VEC2;
				materialOverride.value_vec2 = value;

				overrides.insert_or_assign(id, materialOverride);
			}
			template<>
			void setOverride<Vector3>(const char* id, Vector3 value) {
				auto materialOverride = MaterialOverride();
				materialOverride.type = MaterialOverrideType::VEC3;
				materialOverride.value_vec3 = value;

				overrides.insert_or_assign(id, materialOverride);
			}
			template<>
			void setOverride<Matrix4>(const char* id, Matrix4 value) {
				auto materialOverride = MaterialOverride();
				materialOverride.type = MaterialOverrideType::MAT4;
				materialOverride.value_mat4 = value;

				overrides.insert_or_assign(id, materialOverride);
			}
		};
	}
}