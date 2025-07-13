#include "AnimoBuilder.h"

gbe::ext::AnimoBuilder::AnimoBuilder::AnimoBuilder()
{
}

gbe::ext::AnimoBuilder::AnimoBuilder::~AnimoBuilder()
{
}

gbe::ext::AnimoBuilder::GenerationResult gbe::ext::AnimoBuilder::AnimoBuilder::Generate(GenerationParams params)
{
	GenerationResult res{};

	auto add = [&](std::string type, Vector3 position, Vector3 scale) {
		res.meshes.push_back({
			.type = type,
			.position = position,
			.scale = scale,
			.rotation = Quaternion::Euler(Vector3(0,0,0))
			});
		};

	//PCG FUNCTION

	//STEP 1: CALCULATE VECTORS
	Vector3 delta = params.to - params.from;
	Vector3 stepdir = delta.Normalize();
	Vector3 forward = stepdir.Cross(params.up).Normalize();
	float abdistance = delta.Magnitude();
	float halfheight = params.height * 0.5f;


		//STEP 2.2.2: BASE SEGMENT PLACEMENT
		int x = 0; //placeholder
		Vector3 basepos = stepdir * (x + (params.pillarInterval * 0.5f));
		basepos -= params.up * (params.base_height * 0.5f);
		Vector3 basescale = Vector3(params.pillarInterval * 0.5f, (params.base_height * 0.5f), params.base_width * 0.5f);

		add("base", params.from + basepos, basescale);

	
	return res;
}
