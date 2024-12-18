#pragma once

#include "../gb_rect.h"

namespace gbe {
	namespace gui {
		class gb_root : public gb_rect {
		public:
			gb_root(gb_canvas* canvas);

			void Render() override;
		};
	}
}