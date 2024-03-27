#include "Translate.h"
#include "Parser.h"

PRINTABLE_CLASS_NAME(Translate)

Translate::Translate(Vec3f* const components) :
	Transformation(Transformation::TRANSLATE, components)
{
	this->updateMatrix();
}

Translate::Translate(const float x, const float y, const float z) :
	Transformation(Transformation::TRANSLATE, x, y, z)
{
	this->updateMatrix();
}

void Translate::setIdentifier(const int sequenceNumber){
	this->identifier = "translate" + std::to_string(sequenceNumber);
}

void Translate::updateMatrix(){
	this->matrix.translate(&this->components);
}

Translate::~Translate(){}
