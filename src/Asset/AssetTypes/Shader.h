#pragma once

#include "Asset/BaseAsset.h"
#include <functional>;

namespace gbe {
	namespace asset {
		class Shader : public BaseAsset {
		public:
			struct ShaderData {
				unsigned int gl_id = 0;

				struct ShaderOverrideFunctions {
					std::function<void(const char*, bool)> SetOverride_bool;
					std::function<void(const char*, int)> SetOverride_int;
					std::function<void(const char*, float)> SetOverride_float;
					std::function<void(const char*, Vector2)> SetOverride_Vector2;
					std::function<void(const char*, Vector3)> SetOverride_Vector3;
					std::function<void(const char*, Vector4)> SetOverride_Vector4;
					std::function<void(const char*, Matrix4)> SetOverride_Matrix4;
					std::function<void(const char*, asset::Texture*)> SetOverride_Texture;
				}overridefunctions;
			};
		private:
			ShaderData data;
		public:
			Shader(std::string id, const char* vert, const char* frag);

			unsigned int Get_gl_id();
			void SetFunctions(ShaderData::ShaderOverrideFunctions& funcs);

			//OVERRIDE FUNCTIONS
			template <typename TValue>
			void SetOverride(const char* id, TValue value) {
				throw "Creating new version of restricted template!";
			}
			template<>
			void SetOverride<bool>(const char* id, bool value) {
				data.overridefunctions.SetOverride_bool(id, value);
			}
			template<>
			void SetOverride<int>(const char* id, int value) {
				data.overridefunctions.SetOverride_int(id, value);
			}
			template<>
			void SetOverride<float>(const char* id, float value) {
				data.overridefunctions.SetOverride_float(id, value);
			}
			template<>
			void SetOverride<Vector2>(const char* id, Vector2 value) {
				data.overridefunctions.SetOverride_Vector2(id, value);
			}
			template<>
			void SetOverride<Vector3>(const char* id, Vector3 value) {
				data.overridefunctions.SetOverride_Vector3(id, value);
			}
			template<>
			void SetOverride<Vector4>(const char* id, Vector4 value) {
				data.overridefunctions.SetOverride_Vector4(id, value);
			}
			template<>
			void SetOverride<Matrix4>(const char* id, Matrix4 value) {
				data.overridefunctions.SetOverride_Matrix4(id, value);
			}
			void SetTextureOverride(const char* id, asset::Texture* value, int gpu_tex_slot);

		};
	}
}