#include "DOF.h"

DOF::DOF(float val, float min, float max) {
	this->val = val;
	this->min = min;
	this->max = max;
}
void DOF::setval(float value)
{
	if (value > max)
	{
		val = max;
		return;
	}
	else if (value < min)
	{
		val = min;
		return;
	}
	else
	{
		val = value;
		return;
	}
}