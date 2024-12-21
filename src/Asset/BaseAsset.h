#pragma once

#include <string>
#include <vector>
#include "AssetLoader.h"

namespace gbe {
	namespace asset {
		template<class TFinal, class TData>
		class BaseAsset {
		private:

			std::vector<std::string> paths;
			std::string asset_identifier;
			bool destroy_queued;
		protected:
			TData data;
		public:
			template<typename... Args>
			BaseAsset(std::string id, Args... args) {
				this->asset_identifier = id;
				for (const auto arg : {args...})
				{
					paths.push_back(arg);
				}

				AssetLoader<TFinal, TData>::LoadAsset(static_cast<TFinal*>(this), &this->data);
			}

			std::string GetPath(int index = 0) {
				return this->paths[index];
			}
			std::string Get_assetId() {
				return this->asset_identifier;
			}
			bool Get_destroy_queued() {
				return this->destroy_queued;
			}
		};
	}
}