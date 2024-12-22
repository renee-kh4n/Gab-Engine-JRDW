#pragma once

#include <functional>

namespace gbe {
	namespace asset {

		template<class AssetT, class AssetDataT>
		class AssetLoader {
		protected:
			static AssetLoader* active_instance;
			virtual bool LoadAsset_(AssetT* asset, AssetDataT* data) = 0;
			virtual void UnLoadAsset_(AssetT* asset, AssetDataT* data) = 0;
		public:
			virtual void AssignSelfAsLoader() {
				active_instance = this;
			}
			static bool LoadAsset(AssetT* asset, AssetDataT* data) {
				if (active_instance == nullptr)
					throw "asset loader for this particular type is not assigned!";

				return active_instance->LoadAsset_(asset, data);
			}
			static void UnLoadAsset(AssetT* asset, AssetDataT* data) {
				if (active_instance == nullptr)
					throw "asset loader for this particular type is not assigned!";

				active_instance->UnLoadAsset_(asset, data);
			}
		};

		template<class AssetT, class AssetDataT>
		AssetLoader<AssetT, AssetDataT>* AssetLoader<AssetT, AssetDataT>::active_instance = nullptr;
	}
}