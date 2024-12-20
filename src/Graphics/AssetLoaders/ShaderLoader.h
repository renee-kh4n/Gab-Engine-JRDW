#pragma once

#include "Asset/gbe_asset.h"
#include "Math/gbe_math.h"
#include <glad/glad.h>

namespace gbe {
	namespace gfx {
		class ShaderLoader : public asset::AssetLoader<asset::Shader, asset::Shader::ShaderData> {
		private:
			unsigned int TryCompileShader(std::string path, int compile_type);
		protected:
			bool LoadAsset_(asset::Shader* asset, asset::Shader::ShaderData* data) override;
		public:
			void AssignSelfAsLoader() override;
		};
	}
}