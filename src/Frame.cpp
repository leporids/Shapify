#include "Frame.h"
#include "Parser.h"

Frame::Frame(const float time) :
	time(time),
	functions(),
	interpolationFunctions(),
	interpolatedShapes(),
	interpolatedTransformations(),
	finalFunctions(),
	finalShapes(),
	finalTransformations()
{}

float Frame::getTime() const{
	return this->time;
}

void Frame::setTime(const float time){
	this->time = time;
}

void Frame::addFrameFunction(FrameFunction* const frameFunction){
	this->functions.push_back(frameFunction);
}

template <typename T>
void Frame::addIfAbsentIdentical(std::vector<T*>& collection, T* const item){
	for(size_t i = 0; i < collection.size(); ++i){
		if(collection[i] == item){
			return;
		}
	}
	collection.push_back(item);
}

#define DELETE_ALL(X) for(i = 0; i < X.size(); ++i){delete X[i];}

void Frame::buildInterpolationFunctions(const Frame* const previous){
	const std::vector<FrameFunction*>& previousFunctions = previous->functions;
	size_t i;
	DELETE_ALL(this->interpolationFunctions)
	this->interpolationFunctions.clear();
	this->interpolatedShapes.clear();
	this->interpolatedTransformations.clear();
	for(i = 0; i < this->functions.size(); ++i){
		if(this->functions[i]->type != FrameFunction::SET_TO){
			float* const pointer = this->functions[i]->pointer;
			for(size_t j = 0; j < previousFunctions.size(); ++j){
				if(previousFunctions[j]->pointer == pointer){
					this->newInterpolationFunction(this->functions[i]->type, this->functions[i]->receiver, this->functions[i]->receiverType, pointer, previousFunctions[j]->value, this->functions[i]->value);
					break;
				}
			}
		}
	}
}

void Frame::newInterpolationFunction(const int type, void* const receiver, const int receiverType, float* const pointer, const float start, const float end){
	this->interpolationFunctions.push_back(new InterpolatedFrameFunction(type, pointer, start, end));
	if(receiverType == FrameFunction::RECEIVER_SHAPE_SIZE){
		this->addIfAbsentIdentical(this->interpolatedShapes, (Shape*)receiver);
	}else if(receiverType == FrameFunction::RECEIVER_TRANSFORMATION_VALUE){
		this->addIfAbsentIdentical(this->interpolatedTransformations, (Transformation*)receiver);
	}
}

void Frame::buildFinalFunctions(const Frame* const next){
	const std::vector<FrameFunction*>& nextFunctions = next->functions;
	size_t i;
	DELETE_ALL(this->finalFunctions)
	this->finalFunctions.clear();
	this->finalShapes.clear();
	this->finalTransformations.clear();
	for(i = 0; i < this->functions.size(); ++i){
		float* const pointer = this->functions[i]->pointer;
		bool notInNext = true;
		if(this->functions[i]->type == FrameFunction::SET_TO){
			this->newFinalFunction(this->functions[i]->receiver, this->functions[i]->receiverType, pointer, this->functions[i]->value);
			continue;
		}
		for(size_t j = 0; j < nextFunctions.size(); ++j){
			if(nextFunctions[j]->pointer == pointer){
				notInNext = false;
				if(nextFunctions[j]->type == FrameFunction::SET_TO){
					this->newFinalFunction(this->functions[i]->receiver, this->functions[i]->receiverType, pointer, this->functions[i]->value);
				}
				break;
			}
		}
		if(notInNext){
			this->newFinalFunction(this->functions[i]->receiver, this->functions[i]->receiverType, pointer, this->functions[i]->value);
		}
	}
}

void Frame::newFinalFunction(void* const receiver, const int receiverType, float* const pointer, const float value){
	this->finalFunctions.push_back(new FrameEnd(pointer, value));
	if(receiverType == FrameFunction::RECEIVER_SHAPE_SIZE){
		this->addIfAbsentIdentical(this->finalShapes, (Shape*)receiver);
	}else if(receiverType == FrameFunction::RECEIVER_TRANSFORMATION_VALUE){
		this->addIfAbsentIdentical(this->finalTransformations, (Transformation*)receiver);
	}
}

float Frame::percentageFrom(const float time, const Frame* const previous) const{
	return (time - previous->time) / (this->time - previous->time);
}

#define FRAME_CALL_ALL(collection, name) \
	for(size_t i = 0; i < collection.size(); ++i){ \
		collection[i]->name(); \
	}

#define FRAME_CALL_ALL_WITH_ARGUMENT(collection, name, argument) \
	for(size_t i = 0; i < collection.size(); ++i){ \
		collection[i]->name(argument); \
	}

bool Frame::tick(const float time, const float percentage) const{
	if(time < this->time){
		FRAME_CALL_ALL_WITH_ARGUMENT(this->interpolationFunctions, tick, percentage)
		FRAME_CALL_ALL(this->interpolatedShapes, initializeVertexBuffers)
		FRAME_CALL_ALL(this->interpolatedTransformations, updateMatrix)
		return true;
	}
	return false;
}

void Frame::finalize() const{
	FRAME_CALL_ALL(this->finalFunctions, finishFrame)
	FRAME_CALL_ALL(this->finalShapes, initializeVertexBuffers)
	FRAME_CALL_ALL(this->finalTransformations, updateMatrix)
}

bool Frame::apply(const int function, const float argument){
	switch(function){
	case Parser::FUNCTION_NAME_TIME:
		if(argument >= 0){
			this->setTime(argument);
			return true;
		}else{
			return false;
		}
	default:
		return false;
	}
}

Frame::~Frame(){
	size_t i;
	DELETE_ALL(this->functions)
	DELETE_ALL(this->interpolationFunctions)
	DELETE_ALL(this->finalFunctions)
}
