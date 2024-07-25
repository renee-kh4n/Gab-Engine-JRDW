#include "LineHandler.h"

gde::LineHandler::LineHandler()
{
}

void gde::LineHandler::DrawAll()
{
	/*
	for (auto liner : this->object_list)
	{
		auto p1 = liner->GetPos(0);
		auto p2 = liner->GetPos(1);
		glm::vec4 d1 = this->renderpipeline->GetProjMat() * glm::vec4(p1.x, p1.y, p1.z, 1.0f);
		glm::vec4 d2 = this->renderpipeline->GetProjMat() * glm::vec4(p2.x, p2.y, p2.z, 1.0f);

		glBegin(GL_LINES);
		glVertex3f(d1.x, d1.y, d1.z);
		glVertex3f(d2.x, d2.y, d2.z);
		glEnd();
	}
	*/
}
