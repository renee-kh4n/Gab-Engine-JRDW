#pragma once

#include <functional>

namespace gbe {
	namespace asset {

		template<class AssetT, class AssetDataT>
		class AssetLoaderGeneric {
		private:
			static std::function<bool(AssetT*, AssetDataT&)> load_func;
		public:
			static void Set_load_func(std::function<bool(AssetT*, AssetDataT&)> newfunc) {
				load_func = newfunc;
			}

			static bool LoadAsset(AssetT* asset, AssetDataT& data) {
				if (!load_func)
					throw "asset loader for this particular type is not assigned!";

				return load_func(asset, data);
			}
		};

		template<class AssetT, class AssetDataT>
		std::function<bool(AssetT*, AssetDataT&)> AssetLoaderGeneric<AssetT, AssetDataT>::load_func;
	}
}