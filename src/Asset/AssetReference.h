#pragma once

namespace gbe {
	namespace asset {
		template<class AssetT>
		class AssetReference {
		private:
			AssetT* asset_ptr;
		public:
			void ValidateAsset() {
				if (asset_ptr == nullptr)
					return;

				if (asset_ptr->Get_destroy_queued()) {
					asset_ptr = nullptr;
				}
			}

			AssetT* Get_asset() {
				return this->asset_ptr;
			}

			void Assign(AssetT* valueptr) {
				this->asset_ptr = valueptr;
			}
		};
	}
}