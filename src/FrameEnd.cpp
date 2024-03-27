#include "FrameEnd.h"

FrameEnd::FrameEnd(float* const pointer, const float value) :
	pointer(pointer),
	value(value)
{}

void FrameEnd::finishFrame() const{
	*this->pointer = this->value;
}

FrameEnd::~FrameEnd(){}
