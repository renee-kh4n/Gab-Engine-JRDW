#include "ModelExport.h"

#include <sstream>
#include <ostream>
#include <filesystem>

gbe::editor::ModelExport::ModelExport(std::vector<gbe::Object*> selected)
{
	this->selected = selected;
}

void gbe::editor::ModelExport::Export(std::string path)
{
	std::vector<export_subobject> export_subobjects;
	std::vector<RenderObject*> renderers;


	for (const auto& root_object : this->selected)
	{
		root_object->CallRecursively([&](Object* obj) {
			RenderObject* target = dynamic_cast<RenderObject*>(obj);

			if (target != nullptr && !target->is_editor()) {
				auto it = std::find(renderers.begin(), renderers.end(), target);

				// Check if the value was found
				if (it == renderers.end()) {
					renderers.push_back(target);
				}
			}
			});
	}

	size_t index_offset = 1;

	for (const auto& renderer : renderers)
	{
		size_t verts_here = 0;
		export_subobject new_subobject;
		asset::Mesh* mesh = renderer->Get_DrawCall()->get_mesh();

		for (const auto& vertex : mesh->Get_load_data().vertices)
		{
			asset::data::Vertex newvert = asset::data::Vertex(vertex);
			auto newpos = renderer->GetWorldMatrix() * Vector4(vertex.pos, 1.0);

			newvert.pos = Vector3(newpos);

			new_subobject.export_vertices.push_back(newvert);

			verts_here++;
		}

		for (const auto& face : mesh->Get_load_data().faces)
		{
			auto newface = face;

			for (auto& face_v : newface)
			{
				face_v += index_offset;
			}

			new_subobject.export_faces.push_back(newface);
		}

		export_subobjects.push_back(new_subobject);

		index_offset += verts_here;
	}

	std::stringstream out_string;
	out_string << "# GabEngine 0.01" << std::endl;
	out_string << "# www.gabwithtie.com" << std::endl;
	for (const auto& subobject : export_subobjects)
	{
		out_string << "o mesh" << std::endl;

		for (const auto& vert : subobject.export_vertices)
		{
			out_string << "v " << vert.pos.x << " " << vert.pos.y << " " << vert.pos.z << std::endl;
		}
		for (const auto& vert : subobject.export_vertices)
		{
			out_string << "vn " << vert.normal.x << " " << vert.normal.y << " " << vert.normal.z << std::endl;
		}
		for (const auto& vert : subobject.export_vertices)
		{
			out_string << "vt " << vert.texCoord.x << " " << vert.texCoord.y << std::endl;
		}

		for (const auto& face : subobject.export_faces)
		{
			out_string << "f ";

			for (const auto& index : face)
			{
				out_string << index << "/" << index << "/" << index << " ";
			}

			out_string << std::endl;
		}
	}

	// Attempt to create the directory
	if (std::filesystem::create_directory("out/")) {
		std::cout << "Mesh output directory created successfully." << std::endl;
	}
	else {
		std::cout << "Mesh output directory creation failed or already exists." << std::endl;
	}

	auto file = std::ofstream("out/" + path, std::ios::out | std::ios::binary);
	file << out_string.str();
	file.close();

	auto meta_file = std::ofstream("out/" + path + ".gbe", std::ios::out | std::ios::binary);
	meta_file << "{\"path\":" << "\"" << path << "\"" << "}";
	meta_file.close();
}
