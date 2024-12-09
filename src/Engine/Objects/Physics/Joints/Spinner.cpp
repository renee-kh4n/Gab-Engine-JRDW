#include "Spinner.h"

void gbe::Spinner::InvokeUpdate(float deltatime)
{
	this->body.Set_angularVelocity(this->rotateSpeed);
}
