#include "Compound.h"
#include "units.h"

PRINTABLE_CLASS_NAME(Compound)

Compound::Compound() :
	Shape(),
	children()
{}

Compound::Compound(const Vec3f* const position) :
	Shape(position),
	children()
{}

Compound::Compound(const float x, const float y, const float z) :
	Shape(x, y, z),
	children()
{}

void Compound::setIdentifier(const int sequenceNumber){
	this->identifier = "compound" + std::to_string(sequenceNumber);
}

void Compound::addSize(const Vec3f* const other){}

bool Compound::addChild(Shape* const shape){
	if((shape) && (shape->workspaceIndex < this->workspaceIndex)){
		++shape->referenceCount;
		this->children.push_back(shape);
		return true;
	}
	return false;
}

bool Compound::addChildFromWorkspace(const std::string& identifier){
	Shape* shape;
	this->workspace->findShape(identifier, &shape);
	return this->addChild(shape);
}

size_t Compound::size() const{
	return this->children.size();
}

void Compound::render(const Mat4f* const parentTransformations, const bool isSelected) const{
	Mat4f model;
	this->transformToWorld(&this->position, &model);
	Mat4f allTransformations(parentTransformations, &model);
	for(size_t i = 0; i < this->children.size(); ++i){
		this->children[i]->render(&allTransformations, isSelected);
	}
}

bool Compound::apply(const int function, const std::string& listElement){
	switch(function){
	case Parser::FUNCTION_NAME_SHAPES:
		return this->addChildFromWorkspace(listElement);
	case Parser::FUNCTION_NAME_TRANSFORMATIONS:
		return this->addTransformationFromWorkspace(listElement);
	default:
		return false;
	}
}

Compound::~Compound(){}
