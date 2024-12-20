#pragma once

#include "Asset/BaseAsset.h"

namespace gbe {
	namespace asset {
		class Shader : public BaseAsset {
		private:
			struct ShaderData {
				unsigned int gl_id = 0;
			};
		private:
			ShaderData data;
		public:
			Shader(std::string id, const char* vert, const char* frag);

			unsigned int Get_gl_id();
		};
	}
}