#pragma once

#include <glaze/glaze.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

namespace gbe {
	namespace asset {
		namespace serialization {
			class gbeParser {
			public:
				template<class TImportData>
				static void PopulateClass(TImportData& target, std::string asset_path) {
					std::string buffer;
					TImportData datareceiver;
					auto ec = glz::read_file_json<glz::opts{.error_on_unknown_keys = false}>(datareceiver, asset_path, buffer);
					target = datareceiver;
					std::cout << ec << std::endl;
				}
			};
		}
	}
}