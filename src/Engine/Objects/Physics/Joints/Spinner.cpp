#include "Spinner.h"

void gbe::Spinner::InvokeUpdate(float deltatime)
{
	this->angularVelocity = this->rotateSpeed;
}
