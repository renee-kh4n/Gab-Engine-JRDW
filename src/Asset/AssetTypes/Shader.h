#pragma once

#include "Asset/BaseAsset.h"
#include <functional>
#include "Asset/AssetTypes/Texture.h"
#include "Math/gbe_math.h"

namespace gbe {
	namespace asset {

		namespace data {
			struct ShaderImportData {
				std::string name;
				std::string vert;
				std::string frag;
			};
			struct ShaderLoadData {
				
				struct ShaderOverrideFunctions {
					std::function<void(const char*, bool)> SetOverride_bool;
					std::function<void(const char*, int)> SetOverride_int;
					std::function<void(const char*, float)> SetOverride_float;
					std::function<void(const char*, Vector2)> SetOverride_Vector2;
					std::function<void(const char*, Vector3)> SetOverride_Vector3;
					std::function<void(const char*, Vector4)> SetOverride_Vector4;
					std::function<void(const char*, Matrix4)> SetOverride_Matrix4;
					std::function<void(const char*, asset::Texture*)> SetOverride_Texture;
					std::function<void(const char*, unsigned int)> SetOverride_TextureId;
				}overridefunctions;
			};
		}

		class Shader : public BaseAsset<Shader, data::ShaderImportData, data::ShaderLoadData> {
		public:
			Shader(std::string path);

			std::string Get_name();

			//OVERRIDE FUNCTIONS
			template <typename TValue>
			void SetOverride(const char* id, TValue value) {
				throw "Creating new version of restricted template!";
			}
			template<>
			void SetOverride<bool>(const char* id, bool value) {
				load_data.overridefunctions.SetOverride_bool(id, value);
			}
			template<>
			void SetOverride<int>(const char* id, int value) {
				load_data.overridefunctions.SetOverride_int(id, value);
			}
			template<>
			void SetOverride<float>(const char* id, float value) {
				load_data.overridefunctions.SetOverride_float(id, value);
			}
			template<>
			void SetOverride<Vector2>(const char* id, Vector2 value) {
				load_data.overridefunctions.SetOverride_Vector2(id, value);
			}
			template<>
			void SetOverride<Vector3>(const char* id, Vector3 value) {
				load_data.overridefunctions.SetOverride_Vector3(id, value);
			}
			template<>
			void SetOverride<Vector4>(const char* id, Vector4 value) {
				load_data.overridefunctions.SetOverride_Vector4(id, value);
			}
			template<>
			void SetOverride<Matrix4>(const char* id, Matrix4 value) {
				load_data.overridefunctions.SetOverride_Matrix4(id, value);
			}
			void SetTextureOverride(const char* id, asset::Texture* value);
			void SetTextureIdOverride(const char* id, unsigned int value);
		};
	}
}