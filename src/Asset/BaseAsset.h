#pragma once

#include <string>

namespace gbe {
	namespace asset {
		class BaseAsset {
		private:
			std::string path;
			bool loaded;
			bool destroy_queued;
		public:
			bool Get_destroy_queued();
		};
	}
}