#include "Spinner.h"

void gbe::Spinner::InvokeUpdate(float deltatime)
{
	this->Set_angularVelocity(this->rotateSpeed);
}
