#include "Scale.h"
#include "Parser.h"

PRINTABLE_CLASS_NAME(Scale)

Scale::Scale(Vec3f* const components) :
	Transformation(Transformation::SCALE, components)
{
	this->updateMatrix();
}

Scale::Scale(const float x, const float y, const float z) :
	Transformation(Transformation::SCALE, x, y, z)
{
	this->updateMatrix();
}

void Scale::setIdentifier(const int sequenceNumber){
	this->identifier = "scale" + std::to_string(sequenceNumber);
}

void Scale::updateMatrix(){
	this->matrix.scale(&this->components);
}

Scale::~Scale(){}
