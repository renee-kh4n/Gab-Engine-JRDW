#pragma once

#include <functional>

namespace gbe {
	namespace asset {

		template<class TAsset, class TAssetImportData, class TAssetLoadData>
		class AssetLoader {
		protected:
			static AssetLoader* active_instance;
			virtual bool LoadAsset_(TAsset* asset, const TAssetImportData& import_data, TAssetLoadData* load_data) = 0;
			virtual void UnLoadAsset_(TAsset* asset, const TAssetImportData& import_data, TAssetLoadData* load_data) = 0;
		public:
			virtual void AssignSelfAsLoader() {
				active_instance = this;
			}
			static bool LoadAsset(TAsset* asset, const TAssetImportData& import_data, TAssetLoadData* load_data) {
				if (active_instance == nullptr)
					throw "asset loader for this particular type is not assigned!";

				return active_instance->LoadAsset_(asset, import_data, load_data);
			}
			static void UnLoadAsset(TAsset* asset, const TAssetImportData& import_data, TAssetLoadData* load_data) {
				if (active_instance == nullptr)
					throw "asset loader for this particular type is not assigned!";

				active_instance->UnLoadAsset_(asset, import_data, load_data);
			}
		};

		template<class TAsset, class TAssetImportData, class TAssetLoadData>
		AssetLoader<TAsset, TAssetImportData, TAssetLoadData>* AssetLoader<TAsset, TAssetImportData, TAssetLoadData>::active_instance = nullptr;
	}
}