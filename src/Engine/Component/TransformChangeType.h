#pragma once

namespace gbe {
	enum TransformChangeType {
		NONE = 0,
		TRANSLATION = 1 << 0,
		ROTATION = 1 << 1,
		SCALE = 1 << 2,
		ALL = TRANSLATION | ROTATION | SCALE,
	};
}