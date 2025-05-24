#pragma once

#include <string>
#include <vector>
#include "Asset/AssetLoading/AssetLoader.h"
#include "Asset/Parsing/gbeParser.h"

namespace gbe {
	namespace asset {
		namespace internal {
			class BaseAsset_base {

			};
		}

		template<class TFinal, class TImportData, class TLoadData>
		class BaseAsset : internal::BaseAsset_base {
		private:
			std::string asset_directory;
			bool destroy_queued;
		protected:
			struct BaseImportData {
				std::string asset_type;
				std::string asset_id;
			}base_import_data;

			TImportData import_data;

			TLoadData load_data;
		public:
			BaseAsset(std::string asset_path) {
				gbe::asset::serialization::gbeParser::PopulateClass(this->base_import_data, asset_path);
				gbe::asset::serialization::gbeParser::PopulateClass(this->import_data, asset_path);
				
				this->asset_directory = asset_path;

				while (this->asset_directory.back() != '/')
				{
					this->asset_directory.pop_back();
				}
				
				AssetLoader_base<TFinal, TImportData, TLoadData>::LoadAsset(static_cast<TFinal*>(this), this->import_data, &this->load_data);
			}
			std::string Get_asset_directory() {
				return this->asset_directory;
			}
			std::string Get_assetId() {
				return this->base_import_data.asset_id;
			}
			bool Get_destroy_queued() {
				return this->destroy_queued;
			}
		};
	}
}