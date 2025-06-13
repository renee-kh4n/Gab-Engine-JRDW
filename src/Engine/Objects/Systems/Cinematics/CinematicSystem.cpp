#include "CinematicSystem.h"

#include "Editor/gbe_editor.h"

gbe::CinematicSystem::CinematicSystem()
{
	//DRAWCALL CACHING
	auto gizmoshader = new asset::Shader("DefaultAssets/Shaders/gizmo.shader.gbe");
	auto gizmomesh = new asset::Mesh("DefaultAssets/3D/arrow.obj.gbe");

	auto gizmomat = new asset::Material("DefaultAssets/Materials/gizmo.mat.gbe");
	gizmomat->setOverride("color", Vector4(1, 0, 0, 1.0f));
	this->node_dracall = RenderPipeline::Get_Instance()->RegisterDrawCall(gizmomesh, gizmomat);

	//INSPECTOR BUTTON
	auto spawn_button = new editor::InspectorButton();
	spawn_button->name = "Spawn Node";
	spawn_button->onpress = [this]() {
		this->SpawnNode();
		};

	this->inspectorData->fields.push_back(spawn_button);

	auto play_button = new editor::InspectorButton();
	play_button->name = "Play";
	play_button->onpress = [this]() {
		this->StartCameraMovement();
		};

	this->inspectorData->fields.push_back(play_button);
}

void gbe::CinematicSystem::StartCameraMovement()
{
	this->cur_cam = this->root->GetHandler<Camera>()->object_list.front();

	this->is_playing = true;
	this->sequence_t = 0;
}

void gbe::CinematicSystem::SpawnNode()
{
	auto current_camera = this->root->GetHandler<Camera>()->object_list.front();
	
	//NEW OBJECT
	auto newGizmo = new RenderObject(this->node_dracall);
	newGizmo->SetParent(this->root);

	//POSITIONS
	newGizmo->Local().scale.Set(Vector3(0.1f, 0.1f, 1.0f));
	newGizmo->SetWorldPosition(current_camera->World().position.Get());
	newGizmo->Local().rotation.Set(current_camera->World().rotation.Get());

	//NEW NODE
	this->path.push_back({
			.position = newGizmo->World().position.Get(),
			.rotation = newGizmo->World().rotation.Get(),
			.duration = 1.0f
		});
}

void gbe::CinematicSystem::InvokeUpdate(float deltatime)
{

	if (this->is_playing == false)
		return;

	if (this->path.size() == 0) {
		throw new std::runtime_error("Empty path!");
	}

	this->sequence_t += deltatime;

	CameraSequenceNode* fromnode = &this->path[0];
	CameraSequenceNode* tonode = nullptr;
	float agregated_dur = 0;

	for (size_t i = 1; i < this->path.size(); i++)
	{
		agregated_dur += this->path[i].duration;

		if (agregated_dur > this->sequence_t)
		{
			tonode = &this->path[i];
			fromnode = &this->path[i - 1];
			break;
		}
	}

	if (tonode == nullptr) {
		is_playing = false;
		return;
	}

	const auto norm_t = (this->sequence_t - (agregated_dur - tonode->duration)) / tonode->duration;

	const auto curpos = Vector3::Lerp(fromnode->position, tonode->position, norm_t);
	const auto currot = Quaternion::Lerp(fromnode->rotation, tonode->rotation, norm_t);

	this->cur_cam->SetWorldPosition(curpos);
	this->cur_cam->Local().rotation.Set(currot);
}
