#include "Point.h"
#include "Parser.h"

PRINTABLE_CLASS_NAME(Point)

Point::Point() :
	position(Vec3f()),
	workspace(0),
	workspaceIndex(0),
	referenceCount(0),
	identifier("point")
{}

Point::Point(const float x, const float y, const float z) :
	position(x, y, z),
	workspace(0),
	workspaceIndex(0),
	referenceCount(0),
	identifier("point")
{}

void Point::setWorkspace(Workspace* const workspace){
	this->workspace = workspace;
}

const Vec3f* Point::getPosition() const{
	return &this->position;
}

const std::string& Point::getIdentifier() const{
	return this->identifier;
}

void Point::setPosition(const Vec3f* const position){
	this->position.setComponents(position);
}

void Point::setPosition(const float x, const float y, const float z){
	this->position.setComponents(x, y, z);
}

void Point::setIdentifier(const std::string& identifier){
	this->identifier = identifier;
}

void Point::setIdentifier(const int sequenceNumber){
	this->identifier = "point" + std::to_string(sequenceNumber);
}

size_t Point::size() const{
	return 0;
}

void Point::addPosition(const Vec3f* const other){
	this->position.add(other);
}

bool Point::apply(const int function, const float argument){
	switch(function){
	case Parser::FUNCTION_NAME_X:
		this->position.setX(argument);
		return true;
	case Parser::FUNCTION_NAME_Y:
		this->position.setY(argument);
		return true;
	case Parser::FUNCTION_NAME_Z:
		this->position.setZ(argument);
		return true;
	default:
		return false;
	}
}

bool Point::apply(const int function, const std::string& listElement){
	return false;
}

Point::~Point(){}
