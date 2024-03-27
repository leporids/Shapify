#include "InterpolatedFrameFunction.h"

InterpolatedFrameFunction::InterpolatedFrameFunction(const int type, float* const pointer, const float start, const float end) :
	FrameFunction(type, pointer, start),
	end(end),
	interpolate((type == FrameFunction::SET_TO) ? (&InterpolatedFrameFunction::setTo) : (&InterpolatedFrameFunction::linearTo))
{}

void InterpolatedFrameFunction::tick(const float percentage) const{
	(this->*interpolate)(percentage);
}

void InterpolatedFrameFunction::setTo(const float percentage) const{}

void InterpolatedFrameFunction::linearTo(const float percentage) const{
	*this->pointer = this->value + percentage * (this->end - this->value);
}

InterpolatedFrameFunction::~InterpolatedFrameFunction(){}
