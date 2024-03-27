#include "Loft.h"
#include "units.h"

PRINTABLE_CLASS_NAME(Loft)

Loft::Loft() :
	Shape(),
	children()
{}

Loft::Loft(const Vec3f* const position) :
	Shape(position),
	children()
{}

Loft::Loft(const float x, const float y, const float z) :
	Shape(x, y, z),
	children()
{}

void Loft::setIdentifier(const int sequenceNumber){
	this->identifier = "loft" + std::to_string(sequenceNumber);
}

void Loft::addSize(const Vec3f* const other){}

bool Loft::addChild(PlanePoints* const planePoints){
	if(planePoints){
		++planePoints->referenceCount;
		this->children.push_back(planePoints);
		return true;
	}
	return false;
}

bool Loft::addChildFromWorkspace(const std::string& identifier){
	Point* planePoints;
	this->workspace->findPoint(identifier, &planePoints);
	if(planePoints->size() > 0){
		return this->addChild((PlanePoints*)planePoints);
	}
	return false;
}

size_t Loft::size() const{
	return this->children.size();
}

void Loft::render(const Mat4f* const parentTransformations, const bool isSelected) const{
	Mat4f model;
	this->transformToWorld(&this->position, &model);
	Mat4f allTransformations(parentTransformations, &model);
}

bool Loft::apply(const int function, const std::string& listElement){
	switch(function){
	case Parser::FUNCTION_NAME_POINTS:
		return this->addChildFromWorkspace(listElement);
	case Parser::FUNCTION_NAME_TRANSFORMATIONS:
		return this->addTransformationFromWorkspace(listElement);
	default:
		return false;
	}
}

Loft::~Loft(){}
