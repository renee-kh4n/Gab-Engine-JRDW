#pragma once

#include "../gb_rect.h"
#include "gb_image.h"
#include "../interfaces/gb_onEnter.h"
#include "../interfaces/gb_onExit.h"
#include "../interfaces/gb_onClick.h"
#include "Math/gbe_math.h"
#include <functional>

namespace gbe {
	namespace gui {
		class gb_button : public gb_rect, public gb_onEnter, public gb_onExit, public gb_onClick{
		private:
			gb_image* target_graphic;
			bool hovered;
			Vector4 current_color;
			std::function<void()> onHoverAction;
			std::function<void()> onClickAction;
		public:
			gb_button(gb_image* target_graphic);

			Vector4 normal_color;
			Vector4 hovered_color;
			Vector4 pressed_color;
			float transition_speed = 10;

			void Render() override;
			void Update(float delta) override;

			void onEnter() override;
			void onExit() override;
			void onClick() override;

			void Set_onClickAction(std::function<void()> new_action);
			void Set_onHoverAction(std::function<void()> new_action);
		};
	}
}