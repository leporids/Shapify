#include "Rotate.h"
#include "Parser.h"

PRINTABLE_CLASS_NAME(Rotate)

Rotate::Rotate(const float angle, Vec3f* const components) :
	Transformation(Transformation::ROTATE, components),
	angle(angle)
{
	this->updateMatrix();
}

Rotate::Rotate(const float angle, const float x, const float y, const float z) :
	Transformation(Transformation::ROTATE, x, y, z),
	angle(angle)
{
	this->updateMatrix();
}

float Rotate::getAngle() const{
	return this->angle;
}

void Rotate::setAngle(const float angle){
	this->angle = angle;
	this->updateMatrix();
}

void Rotate::addAngle(const float angle){
	this->angle += angle;
	this->updateMatrix();
}

void Rotate::setIdentifier(const int sequenceNumber){
	this->identifier = "rotate" + std::to_string(sequenceNumber);
}

bool Rotate::apply(const int function, const float argument){
	switch(function){
	case Parser::FUNCTION_NAME_ANGLE:
		this->angle = argument;
		this->updateMatrix();
		return true;
	default:
		return Transformation::apply(function, argument);
	}
}

void Rotate::updateMatrix(){
	this->matrix.rotate(this->angle, &this->components);
}

Rotate::~Rotate(){}
