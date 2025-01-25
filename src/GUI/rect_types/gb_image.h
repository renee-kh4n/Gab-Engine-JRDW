#pragma once

#include "../gb_rect.h"
#include "Asset/AssetTypes/Texture.h"

namespace gbe {
	namespace gui {
		class gb_image : public gb_rect {
		private:
			asset::Texture* sprite;
			glm::vec4 current_color;
		public:
			void Set_Image(asset::Texture* tex);
			void Render() override;
			void Set_tint(glm::vec4 color);
		};
	}
}