#include "ModelExport.h"

gbe::editor::ModelExport::ModelExport(std::vector<gbe::Object*> selected)
{
	this->selected = selected;
}

void gbe::editor::ModelExport::Export(std::string path)
{
	std::vector<asset::data::Vertex> export_vertices;
	std::vector<unsigned int> export_indices;

	std::vector<RenderObject*> renderers;


	for (const auto& root_object : this->selected)
	{
		root_object->CallRecursively([&](Object* obj) {
			RenderObject* target = dynamic_cast<RenderObject*>(obj);

			if (target != nullptr) {
				renderers.push_back(target);
			}
			});
	}

	for (const auto& renderer : renderers)
	{
		asset::Mesh* mesh = renderer->Get_DrawCall()->get_mesh();

		for (const auto& vertex: mesh->Get_load_data().vertices)
		{
			asset::data::Vertex newvert = asset::data::Vertex(vertex);
			auto newpos = renderer->GetWorldMatrix() * Vector4(vertex.pos, 1.0);

			newvert.pos = Vector3(newpos);

			export_vertices.push_back(newvert);
		}

		for (const auto& index: mesh->Get_load_data().indices)
		{
			export_indices.push_back(index);
		}
	}
}
