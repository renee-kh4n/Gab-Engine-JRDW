#pragma once

#include <unordered_map>
#include <string>
#include "../AssetInjection/AssetReference.h"
#include "Texture.h"
#include "Shader.h"
#include "Math/gbe_math.h"

namespace gbe {
	namespace asset {
		namespace data {
			struct MaterialImportData {
				std::string shader;
			};
			struct MaterialLoadData {
				unsigned int gl_id;

			};
		}

		class MaterialOverride {
		public:
			asset::Shader::UniformFieldType type;

			MaterialOverride();
			~MaterialOverride();

			bool value_bool;
			float value_float;
			Vector2 value_vec2;
			Vector3 value_vec3;
			Vector4 value_vec4;
			Matrix4 value_mat4;
			asset::AssetReference<asset::Texture> value_tex;

			bool handled_change = false;
		};

		class Material : public BaseAsset<asset::Material, data::MaterialImportData, data::MaterialLoadData> {
			std::unordered_map<std::string, MaterialOverride> overrides;
			asset::AssetReference<asset::Shader> m_shader;
		public:
			Material(std::string path);

			size_t getOverrideCount() const {
				return overrides.size();
			}

			MaterialOverride& getOverride(size_t index, std::string& id) {
				auto it = overrides.begin();
				std::advance(it, index);

				id = it->first;
				return it->second;
			}

			void setShader(asset::Shader* shader);
			asset::Shader* getShader();

			template <typename TValue>
			void setOverride(std::string id, TValue value) {}

			template<>
			void setOverride<bool>(std::string id, bool value) {
				auto materialOverride = MaterialOverride();
				materialOverride.type = Shader::UniformFieldType::BOOL;
				materialOverride.value_bool = value;

				overrides.insert_or_assign(id, materialOverride);
			}
			template<>
			void setOverride<float>(std::string id, float value) {
				auto materialOverride = MaterialOverride();
				materialOverride.type = Shader::UniformFieldType::FLOAT;
				materialOverride.value_float = value;

				overrides.insert_or_assign(id, materialOverride);
			}
			template<>
			void setOverride<Vector2>(std::string id, Vector2 value) {
				auto materialOverride = MaterialOverride();
				materialOverride.type = Shader::UniformFieldType::VEC2;
				materialOverride.value_vec2 = value;

				overrides.insert_or_assign(id, materialOverride);
			}
			template<>
			void setOverride<Vector3>(std::string id, Vector3 value) {
				auto materialOverride = MaterialOverride();
				materialOverride.type = Shader::UniformFieldType::VEC3;
				materialOverride.value_vec3 = value;

				overrides.insert_or_assign(id, materialOverride);
			}
			template<>
			void setOverride<Vector4>(std::string id, Vector4 value) {
				auto materialOverride = MaterialOverride();
				materialOverride.type = Shader::UniformFieldType::VEC4;
				materialOverride.value_vec4 = value;

				overrides.insert_or_assign(id, materialOverride);
			}
			template<>
			void setOverride<Matrix4>(std::string id, Matrix4 value) {
				auto materialOverride = MaterialOverride();
				materialOverride.type = Shader::UniformFieldType::MAT4;
				materialOverride.value_mat4 = value;

				overrides.insert_or_assign(id, materialOverride);
			}
			template<>
			void setOverride<asset::Texture*>(std::string id, asset::Texture* value) {
				auto materialOverride = MaterialOverride();
				materialOverride.type = Shader::UniformFieldType::TEXTURE;
				materialOverride.value_tex.Assign(value);

				overrides.insert_or_assign(id, materialOverride);
			}
		};
	}
}