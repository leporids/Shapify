#include "PlanePoints.h"
#include "Parser.h"

PRINTABLE_CLASS_NAME(PlanePoints)

PlanePoints::PlanePoints() :
	PlanePoints(PlanePoints::XY, 0.0f, 0.0f, 0.0f)
{}

PlanePoints::PlanePoints(const int type, const float x, const float y, const float z) :
	Point(x, y, z),
	type(type),
	children()
{}

int PlanePoints::getType() const{
	return this->type;
}

void PlanePoints::setType(const int type){
	if(type < PlanePoints::MINIMUM_TYPE){
		this->type = PlanePoints::MINIMUM_TYPE;
	}else if(type > PlanePoints::MAXIMUM_TYPE){
		this->type = PlanePoints::MAXIMUM_TYPE;
	}else{
		this->type = type;
	}
}

bool PlanePoints::addChild(Point* const point){
	if((point) && (point->workspaceIndex < this->workspaceIndex)){
		++point->referenceCount;
		this->children.push_back(point);
		return true;
	}
	return false;
}

bool PlanePoints::addChildFromWorkspace(const std::string& identifier){
	Point* point;
	this->workspace->findPoint(identifier, &point);
	return this->addChild(point);
}

void PlanePoints::setIdentifier(const int sequenceNumber){
	this->identifier = "planePoints" + std::to_string(sequenceNumber);
}

size_t PlanePoints::size() const{
	return this->children.size();
}

bool PlanePoints::apply(const int function, const float argument){
	switch(function){
	case Parser::FUNCTION_NAME_TYPE:
		this->setType((int)argument);
		return true;
	default:
		return Point::apply(function, argument);
	}
}

bool PlanePoints::apply(const int function, const std::string& listElement){
	switch(function){
	case Parser::FUNCTION_NAME_POINTS:
		return this->addChildFromWorkspace(listElement);
	default:
		return false;
	}
}

PlanePoints::~PlanePoints(){}
