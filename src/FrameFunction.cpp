#include "FrameFunction.h"
#include "Animation.h"
#include "Point.h"
#include "Shape.h"

const std::string& FrameFunction::UNPRINTABLE = ".class";

FrameFunction::FrameFunction(const int type, float* const pointer, const float value) :
	type(type),
	pointer(pointer),
	value(value),
	receiver((void*)0),
	receiverType(FrameFunction::RECEIVER_NONE),
	selector(Animation::NONE)
{}

FrameFunction::FrameFunction(const int type, float* const pointer, const float value, void* const receiver, const int receiverType, const int selector) :
	type(type),
	pointer(pointer),
	value(value),
	receiver(receiver),
	receiverType(receiverType),
	selector(selector)
{}

const std::string& FrameFunction::getIdentifier() const{
	switch(receiverType){
	case FrameFunction::RECEIVER_POINT_VALUE:
		return ((Point*)(this->receiver))->getIdentifier();
	case FrameFunction::RECEIVER_SHAPE_POSITION:
	case FrameFunction::RECEIVER_SHAPE_SIZE:
		return ((Shape*)(this->receiver))->getIdentifier();
	case FrameFunction::RECEIVER_TRANSFORMATION_VALUE:
		return ((Transformation*)(this->receiver))->getIdentifier();
	case FrameFunction::RECEIVER_NONE:
	default:
		return FrameFunction::UNPRINTABLE;
	}
}

#define FRAME_FUNCTION_WORKSPACE_TYPE_STRING(t, s) \
	case FrameFunction::t: \
		output = #s; \
		break;

void FrameFunction::getWorkspaceTypeString(const int type, std::string& output){
	switch(type){
	FRAME_FUNCTION_WORKSPACE_TYPE_STRING(SET_TO, setTo)
	FRAME_FUNCTION_WORKSPACE_TYPE_STRING(LINEAR_TO, linearTo)
	default:
		break;
	}
}

#define FRAME_FUNCTION_TYPE_STRING(t) \
	case FrameFunction::t: \
		output = "FrameFunction::" #t; \
		break;

void FrameFunction::getTypeString(const int type, std::string& output){
	switch(type){
	FRAME_FUNCTION_TYPE_STRING(SET_TO)
	FRAME_FUNCTION_TYPE_STRING(LINEAR_TO)
	default:
		break;
	}
}

FrameFunction::~FrameFunction(){}
