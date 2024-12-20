#pragma once

#include <functional>

namespace gbe {
	namespace asset {

		template<class AssetT, class AssetDataT>
		class AssetLoader {
		protected:
			static std::function<bool(AssetT*, AssetDataT*)> load_func;
			virtual bool LoadAsset_(AssetT* asset, AssetDataT* data) = 0;
		public:
			virtual void AssignSelfAsLoader() = 0;
			static bool LoadAsset(AssetT* asset, AssetDataT* data) {
				if (!load_func)
					throw "asset loader for this particular type is not assigned!";

				return load_func(asset, data);
			}
		};

		template<class AssetT, class AssetDataT>
		std::function<bool(AssetT*, AssetDataT*)> AssetLoader<AssetT, AssetDataT>::load_func;
	}
}