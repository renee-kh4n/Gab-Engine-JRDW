#pragma once

#include <string>
#include <vector>
#include <functional>

namespace gbe {
	namespace editor {
		struct InspectorField {
			enum FieldType {
				STRING,
				INT,
				FLOAT,
				VECTOR3,
				QUATERNION,
				COLOR,
				BOOLEAN,
				FUNCTION
			};
			
			std::string name;
			FieldType fieldtype;
		};

		struct InspectorVec3 : public InspectorField {
			float* x = nullptr;
			float* y = nullptr;
			float* z = nullptr;

			InspectorVec3() {
				this->fieldtype = FieldType::VECTOR3;
			}
		};

		struct InspectorButton :public InspectorField {
			std::function<void()> onpress;

			InspectorButton() {
				this->fieldtype = FieldType::FUNCTION;
			}
		};

		struct InspectorData {
			std::vector<InspectorField*> fields;
		};
	}
}