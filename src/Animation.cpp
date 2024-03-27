#include "Animation.h"
#include "Point.h"
#include "PlanePoints.h"
#include "Shape.h"
#include "Box.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Compound.h"
#include "Loft.h"
#include "Transformation.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"

Animation::Animation() :
	frames(),
	frameIndex(0),
	current(0),
	next(0)
{}

void Animation::addFrame(const float time){
	this->frames.push_back(new Frame(time));
}

void Animation::addFrame(Frame* const frame){
	if(frame){
		this->frames.push_back(frame);
	}
}

bool Animation::applyToLastFrame(const int function, const float argument){
	if(this->frames.size() > 0){
		return this->frames.back()->apply(function, argument);
	}else{
		return false;
	}
}

bool Animation::addFrameFunction(const int type, float* const pointer, const float value, void* const receiver, const int receiverType, const int selector){
	if((this->frames.size() == 0) || (type < FrameFunction::MINIMUM_INTERPOLATION_TYPE) || (type > FrameFunction::MAXIMUM_INTERPOLATION_TYPE)){
		return false;
	}
	this->frames.back()->addFrameFunction(new FrameFunction(type, pointer, value, receiver, receiverType, selector));
	return true;
}

#define ANIMATION_FRAME_FUNCTION(t, name, receiverType, pointer) case Animation::t: return this->addFrameFunction(type, &name->pointer, value, name, receiverType, Animation::t);
#define ANIMATION_POINT_FRAME_FUNCTION(t, name, pointer) ANIMATION_FRAME_FUNCTION(t, name, FrameFunction::RECEIVER_POINT_VALUE, pointer)
#define ANIMATION_SHAPE_POSITION_FRAME_FUNCTION(t, name, pointer) ANIMATION_FRAME_FUNCTION(t, name, FrameFunction::RECEIVER_SHAPE_POSITION, pointer)
#define ANIMATION_SHAPE_SIZE_FRAME_FUNCTION(t, name, pointer) ANIMATION_FRAME_FUNCTION(t, name, FrameFunction::RECEIVER_SHAPE_SIZE, pointer)
#define ANIMATION_TRANSFORMATION_FRAME_FUNCTION(t, name, pointer) ANIMATION_FRAME_FUNCTION(t, name, FrameFunction::RECEIVER_TRANSFORMATION_VALUE, pointer)

bool Animation::addFrameFunction(const int type, Point* const point, const int selector, const float value){
	switch(selector){
	ANIMATION_POINT_FRAME_FUNCTION(X, point, position.components[0])
	ANIMATION_POINT_FRAME_FUNCTION(Y, point, position.components[1])
	ANIMATION_POINT_FRAME_FUNCTION(Z, point, position.components[2])
	default:
		return false;
	}
}

bool Animation::addFrameFunction(const int type, PlanePoints* const planePoints, const int selector, const float value){
	return this->addFrameFunction(type, (Point*)planePoints, selector, value);
}

bool Animation::addFrameFunction(const int type, Shape* const shape, const int selector, const float value){
	switch(selector){
	ANIMATION_SHAPE_POSITION_FRAME_FUNCTION(X, shape, position.components[0])
	ANIMATION_SHAPE_POSITION_FRAME_FUNCTION(Y, shape, position.components[1])
	ANIMATION_SHAPE_POSITION_FRAME_FUNCTION(Z, shape, position.components[2])
	default:
		return false;
	}
}

bool Animation::addFrameFunction(const int type, Box* const box, const int selector, const float value){
	switch(selector){
	ANIMATION_SHAPE_SIZE_FRAME_FUNCTION(LENGTH, box, size.components[0])
	ANIMATION_SHAPE_SIZE_FRAME_FUNCTION(WIDTH, box, size.components[1])
	ANIMATION_SHAPE_SIZE_FRAME_FUNCTION(HEIGHT, box, size.components[2])
	default:
		return this->addFrameFunction(type, (Shape*)box, selector, value);
	}
}

bool Animation::addFrameFunction(const int type, Cone* const cone, const int selector, const float value){
	switch(selector){
	ANIMATION_SHAPE_SIZE_FRAME_FUNCTION(HEIGHT, cone, height)
	ANIMATION_SHAPE_SIZE_FRAME_FUNCTION(RADIUS, cone, radius)
	default:
		return this->addFrameFunction(type, (Shape*)cone, selector, value);
	}
}

bool Animation::addFrameFunction(const int type, Cylinder* const cylinder, const int selector, const float value){
	switch(selector){
	ANIMATION_SHAPE_SIZE_FRAME_FUNCTION(HEIGHT, cylinder, height)
	ANIMATION_SHAPE_SIZE_FRAME_FUNCTION(RADIUS, cylinder, radius)
	default:
		return this->addFrameFunction(type, (Shape*)cylinder, selector, value);
	}
}

bool Animation::addFrameFunction(const int type, Sphere* const sphere, const int selector, const float value){
	switch(selector){
	ANIMATION_SHAPE_SIZE_FRAME_FUNCTION(RADIUS, sphere, radius)
	default:
		return this->addFrameFunction(type, (Shape*)sphere, selector, value);
	}
}

bool Animation::addFrameFunction(const int type, Compound* const compound, const int selector, const float value){
	return this->addFrameFunction(type, (Shape*)compound, selector, value);
}

bool Animation::addFrameFunction(const int type, Loft* const loft, const int selector, const float value){
	return this->addFrameFunction(type, (Shape*)loft, selector, value);
}

bool Animation::addFrameFunction(const int type, Transformation* const transformation, const int selector, const float value){
	switch(selector){
	ANIMATION_TRANSFORMATION_FRAME_FUNCTION(X, transformation, components.components[0])
	ANIMATION_TRANSFORMATION_FRAME_FUNCTION(Y, transformation, components.components[1])
	ANIMATION_TRANSFORMATION_FRAME_FUNCTION(Z, transformation, components.components[2])
	default:
		return false;
	}
}

bool Animation::addFrameFunction(const int type, Translate* const translate, const int selector, const float value){
	return this->addFrameFunction(type, (Transformation*)translate, selector, value);
}

bool Animation::addFrameFunction(const int type, Rotate* const rotate, const int selector, const float value){
	switch(selector){
	ANIMATION_TRANSFORMATION_FRAME_FUNCTION(ANGLE, rotate, angle)
	default:
		return this->addFrameFunction(type, (Transformation*)rotate, selector, value);
	}
}

bool Animation::addFrameFunction(const int type, Scale* const scale, const int selector, const float value){
	return this->addFrameFunction(type, (Transformation*)scale, selector, value);
}

void Animation::reset(){
	if(this->frames.size() > 1){
		this->current = this->frames[0];
		this->next = this->frames[1];
		this->frameIndex = 2 % this->frames.size();
		this->current->finalize();
	}
}

bool Animation::build(){
	if(this->frames.size() > 1){
		for(size_t i = 0; i < this->frames.size(); ++i){
			int previousIndex = i - 1;
			int nextIndex = i + 1;
			if(previousIndex < 0){
				previousIndex = this->frames.size() - 1;
			}
			if((size_t)nextIndex >= this->frames.size()){
				nextIndex = 0;
			}
			if((i > 0) && (this->frames[(size_t)previousIndex]->getTime() >= this->frames[i]->getTime())){
				return false;
			}
			this->frames[i]->buildInterpolationFunctions(this->frames[(size_t)previousIndex]);
			this->frames[i]->buildFinalFunctions(this->frames[(size_t)nextIndex]);
		}
	}
	this->reset();
	return true;
}

bool Animation::tick(const float time){
	if(!this->next->tick(time, this->next->percentageFrom(time, this->current))){
		this->next->finalize();
		this->current = this->next;
		this->next = this->frames[this->frameIndex];
		this->frameIndex = (this->frameIndex + 1) % this->frames.size();
		if(this->frameIndex == 1){
			this->reset();
			return false;
		}
	}
	return true;
}

#define ANIMATION_WORKSPACE_SELECTOR_STRING(t, s) \
	case Animation::t: \
		output = #s; \
		break;

void Animation::getWorkspaceSelectorString(const int selector, std::string& output){
	switch(selector){
	ANIMATION_WORKSPACE_SELECTOR_STRING(NONE, none)
	ANIMATION_WORKSPACE_SELECTOR_STRING(X, x)
	ANIMATION_WORKSPACE_SELECTOR_STRING(Y, y)
	ANIMATION_WORKSPACE_SELECTOR_STRING(Z, z)
	ANIMATION_WORKSPACE_SELECTOR_STRING(LENGTH, length)
	ANIMATION_WORKSPACE_SELECTOR_STRING(WIDTH, width)
	ANIMATION_WORKSPACE_SELECTOR_STRING(HEIGHT, height)
	ANIMATION_WORKSPACE_SELECTOR_STRING(RADIUS, radius)
	ANIMATION_WORKSPACE_SELECTOR_STRING(ANGLE, angle)
	default:
		break;
	}
}

#define ANIMATION_SELECTOR_STRING(t) \
	case Animation::t: \
		output = "Animation::" #t; \
		break;

void Animation::getSelectorString(const int selector, std::string& output){
	switch(selector){
	ANIMATION_SELECTOR_STRING(NONE)
	ANIMATION_SELECTOR_STRING(X)
	ANIMATION_SELECTOR_STRING(Y)
	ANIMATION_SELECTOR_STRING(Z)
	ANIMATION_SELECTOR_STRING(LENGTH)
	ANIMATION_SELECTOR_STRING(WIDTH)
	ANIMATION_SELECTOR_STRING(HEIGHT)
	ANIMATION_SELECTOR_STRING(RADIUS)
	ANIMATION_SELECTOR_STRING(ANGLE)
	default:
		break;
	}
}

#define ANIMATION_RETURN_SELECTOR(t, s) if(string == #s){return Animation::t;}

int Animation::selectorFromWorkspaceString(const std::string& string){
	ANIMATION_RETURN_SELECTOR(X, x)
	ANIMATION_RETURN_SELECTOR(Y, y)
	ANIMATION_RETURN_SELECTOR(Z, z)
	ANIMATION_RETURN_SELECTOR(LENGTH, length)
	ANIMATION_RETURN_SELECTOR(WIDTH, width)
	ANIMATION_RETURN_SELECTOR(HEIGHT, height)
	ANIMATION_RETURN_SELECTOR(RADIUS, radius)
	ANIMATION_RETURN_SELECTOR(ANGLE, angle)
	return Animation::NONE;
}

Animation::~Animation(){
	for(size_t i = 0; i < this->frames.size(); ++i){
		delete this->frames[i];
	}
}
