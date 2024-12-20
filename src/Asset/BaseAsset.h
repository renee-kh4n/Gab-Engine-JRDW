#pragma once

#include <string>
#include <vector>

namespace gbe {
	namespace asset {
		class BaseAsset {
		private:
			std::vector<std::string> paths;
			std::string asset_identifier;
			bool destroy_queued;
		public:
			template<typename... Args>
			BaseAsset(std::string id, Args... args) {
				this->asset_identifier = id;
				for (const auto arg : {args...})
				{
					paths.push_back(arg);
				}
			}

			std::string GetPath(int index = 0);

			bool Get_destroy_queued();
		};
	}
}