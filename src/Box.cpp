#include "Box.h"

PRINTABLE_CLASS_NAME(Box)

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
			const float color = 0.7f * ((float)(i + 1) / (float)Box::NUMBER_OF_TRIANGLES);
			PUSH_BACK_4F(this->colors, color, color, color, 1.0f)
			PUSH_BACK_4F(this->colors, 0.1f, color, 0.1f, 1.0f)
		}
		glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->colors.size(), (void*)this->colors.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		this->needsColorBuffer = false;
	}
	if(this->needsElementBuffer){
		glGenBuffers(1, &this->elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Box::indices), (void*)Box::indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		this->needsElementBuffer = false;
	}
}

void Box::render(const Mat4f* const parentTransformations, const bool isSelected) const{
	Mat4f model;
	this->transformToWorld(&this->position, &model);
	Mat4f allTransformations(parentTransformations, &model);
	glUseProgram(this->shader);
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	GLuint location = glGetAttribLocation(this->shader, "position");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
	location = glGetAttribLocation(this->shader, "color");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(isSelected ? (4 * sizeof(float)) : 0));	
	location = glGetUniformLocation(this->shader, "transformations");
	glUniformMatrix4fv(location, 1, GL_TRUE, allTransformations.getEntries());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
	glDrawElements(GL_TRIANGLES, Box::NUMBER_OF_INDICES, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
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
