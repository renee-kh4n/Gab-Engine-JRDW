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

	auto add_cube = [&](Vector3 position, Vector3 scale) {
		res.meshes.push_back({
			.type = "cube",
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

	//STEP 2: EXECUTE MAIN WALL SEGMENT LOOP
	for (float x = 0; x < abdistance; x += params.pillarInterval)
	{
		//STEP 2.1: PILLAR PLACEMENT
		Vector3 pillarpos = stepdir * x;
		pillarpos += params.up * halfheight;
		Vector3 pillarscale = Vector3(0.5f) * params.pillarThickness;
		pillarscale.y = halfheight;

		add_cube(params.from + pillarpos, pillarscale);

		//STEP 2.2: WALL SEGMENT PLACEMENT
		Vector3 wallpos = stepdir * (x + (params.pillarInterval * 0.5f));
		wallpos += params.up * halfheight;
		Vector3 wallscale = Vector3(params.pillarInterval * 0.5f, halfheight, params.wallThickness * 0.5f);

		add_cube(params.from + wallpos, wallscale);

		//STEP 2.3 BEAM PLACEMENT
		for (float y = params.beamInterval; y < params.height; y += params.beamInterval)
		{
			Vector3 beampos = stepdir * (x + (params.pillarInterval * 0.5f));
			beampos += params.up * y;
			Vector3 beamscale = Vector3(params.pillarInterval * 0.5f, params.beamThickness * 0.5f, params.beamThickness * 0.5f);

			add_cube(params.from + beampos, beamscale);
		}

		//STEP 2.4 WINDOW PLACEMENT
		for (float y = 0; y + params.beamInterval < params.height; y += params.beamInterval)
		{
			Vector3 windowpos = stepdir * (x + (params.pillarInterval * 0.5f));
			windowpos += params.up * (y + params.windowHeight + (params.windowSize.y * 0.5f));
			windowpos += -forward * ((params.wallThickness * 0.5f) + (params.windowSize.z * 0.5f));
			Vector3 windowscale = params.windowSize * 0.5f;

			add_cube(params.from + windowpos, windowscale);
		}

		//STEP 2.5 ROOF PLACEMENT
		Vector3 roofpos = stepdir * (x + (params.pillarInterval * 0.5f));
		roofpos += params.up * (params.height + (params.roofHeight * 0.5f));
		Vector3 roofscale = Vector3(params.pillarInterval * 0.5f, params.roofHeight * 0.5f, params.roofThickness * 0.5f);

		add_cube(params.from + roofpos, roofscale);
	}

	return res;
}
