#include "Mesh.h"

gbe::asset::Mesh::Mesh(std::string path) : BaseAsset(path) {

}

unsigned int gbe::asset::Mesh::Get_meshid() {
	return this->load_data.gl_id;
}