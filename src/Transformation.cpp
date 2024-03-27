#include "Transformation.h"
#include "Parser.h"

Transformation::Transformation(const int type, Vec3f* const components) :
	type(type),
	referenceCount(0),
	components(components),
	matrix(),
	identifier("transformation")
{}

Transformation::Transformation(const int type, const float x, const float y, const float z) :
	type(type),
	referenceCount(0),
	components(Vec3f(x, y, z)),
	matrix(),
	identifier("transformation")
{}

Transformation::Transformation(const Transformation* const other) :
	type(other->type),
	referenceCount(0),
	components(&other->components),
	matrix(&other->matrix)
{}

int Transformation::getType() const{
	return this->type;
}

const Vec3f* Transformation::getComponents() const{
	return &this->components;
}

const Mat4f* Transformation::getMatrix() const{
	return &this->matrix;
}

const std::string& Transformation::getIdentifier() const{
	return this->identifier;
}

void Transformation::setIdentifier(const std::string& identifier){
	this->identifier = identifier;
}

void Transformation::setIdentifier(const int sequenceNumber){
	this->identifier = "transformation" + std::to_string(sequenceNumber);
}

void Transformation::addPosition(const Vec3f* const other){
	this->components.add(other);
	this->updateMatrix();
}

bool Transformation::apply(const int function, const float argument){
	switch(function){
	case Parser::FUNCTION_NAME_X:
		this->components.setX(argument);
		this->updateMatrix();
		return true;
	case Parser::FUNCTION_NAME_Y:
		this->components.setY(argument);
		this->updateMatrix();
		return true;
	case Parser::FUNCTION_NAME_Z:
		this->components.setZ(argument);
		this->updateMatrix();
		return true;
	default:
		return false;
	}
}

Transformation::~Transformation(){}
