#include "Box.h"

PRINTABLE_CLASS_NAME(Box)

const float Box::normals[Box::NUMBER_OF_NORMAL_COMPONENTS] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f};

const int Box::indices[Box::NUMBER_OF_INDICES] = {6, 2, 0, 6, 4, 0, 6, 7, 5, 6, 4, 5, 7, 3, 1, 7, 5, 1, 2, 3, 1, 2, 0, 1, 6, 7, 3, 6, 2, 3, 4, 5, 1, 4, 0, 1};

Box::Box() :
	Shape(),
	size(Vec3f(1.0f, 1.0f, 1.0f))
{
	this->initializeVertexBuffers();
}

Box::Box(const Vec3f* const position, const Vec3f* const size) :
	Shape(position),
	size(size)
{
	this->initializeVertexBuffers();
}

Box::Box(const float x, const float y, const float z, const float l, const float w, const float h) :
	Shape(x, y, z),
	size(l, w, h)
{
	this->initializeVertexBuffers();
}

const Vec3f* Box::getSize() const{
	return &this->size;
}

void Box::addSize(const Vec3f* const other){
	const float* const size = other->getComponents();
	this->size.add(-size[2], size[0], size[1]);
	this->initializeVertexBuffers();
}

void Box::setIdentifier(const int sequenceNumber){
	this->identifier = "box" + std::to_string(sequenceNumber);
}

void Box::initializeVertexBuffers(){
	const float* const lwh = this->size.getComponents();
	const float l = lwh[0] / 2.0f;
	const float w = lwh[1] / 2.0f;
	const float h = lwh[2] / 2.0f;
	if(this->needsPositionBuffer){
		glGenBuffers(1, &this->positionBuffer);
		this->needsPositionBuffer = false;
	}
	this->vertices.clear();
	for(int i = 0; i < Box::NUMBER_OF_VERTICES; ++i){
		this->vertices.push_back((i & 4) ? (-w) : w);
		this->vertices.push_back((i & 2) ? (-h) : h);
		this->vertices.push_back((i & 1) ? (-l) : l);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), (void*)this->vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if(this->needsColorBuffer){
		glGenBuffers(1, &this->colorBuffer);
		this->colors.clear();
		for(int i = 0; i < Box::NUMBER_OF_TRIANGLES; ++i){
			PUSH_BACK_4F(this->colors, Shape::DEFAULT_COLOR_R, Shape::DEFAULT_COLOR_G, Shape::DEFAULT_COLOR_B, Shape::DEFAULT_COLOR_A)
			PUSH_BACK_4F(this->colors, Shape::DEFAULT_SELECTED_COLOR_R, Shape::DEFAULT_SELECTED_COLOR_G, Shape::DEFAULT_SELECTED_COLOR_B, Shape::DEFAULT_SELECTED_COLOR_A)
		}
		glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->colors.size(), (void*)this->colors.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		this->needsColorBuffer = false;
	}
	if(this->needsNormalBuffer){
		glGenBuffers(1, &this->normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Box::normals), (void*)Box::normals, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		this->needsNormalBuffer = false;
	}
	if(this->needsElementBuffer){
		glGenBuffers(1, &this->elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Box::indices), (void*)Box::indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		this->needsElementBuffer = false;
	}
}

void Box::render(const Mat4f* const projection, const Mat4f* const view, const Mat4f* const parentTransformations, const float* const cameraPositionComponents, const bool isSelected) const{
	Mat4f model;
	this->transformToWorld(&this->position, &model);
	Mat4f allTransformations(parentTransformations, &model);
	glUseProgram(this->shader);
	SHAPE_UNIFORM_MATRICES(projection, view, allTransformations)
	SHAPE_ATTRIB_POINTER(position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0)
	SHAPE_ATTRIB_POINTER(color, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (isSelected ? (4 * sizeof(float)) : 0))
	SHAPE_ATTRIB_POINTER(normal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0)
	SHAPE_UNIFORM_DEFAULT_LIGHTING(cameraPositionComponents)
	SHAPE_DRAW_ELEMENTS(Box::NUMBER_OF_INDICES)
}

bool Box::apply(const int function, const float argument){
	switch(function){
	case Parser::FUNCTION_NAME_LENGTH:
		this->size.setX(argument);
		this->initializeVertexBuffers();
		return true;
	case Parser::FUNCTION_NAME_WIDTH:
		this->size.setY(argument);
		this->initializeVertexBuffers();
		return true;
	case Parser::FUNCTION_NAME_HEIGHT:
		this->size.setZ(argument);
		this->initializeVertexBuffers();
		return true;
	default:
		return Shape::apply(function, argument);
	}
}

bool Box::apply(const int function, const std::string& listElement){
	switch(function){
	case Parser::FUNCTION_NAME_TRANSFORMATIONS:
		return this->addTransformationFromWorkspace(listElement);
	default:
		return false;
	}
}

Box::~Box(){}
