#include <cmath>
#include "Sphere.h"
#include "units.h"

PRINTABLE_CLASS_NAME(Sphere)

Sphere::Sphere() :
	Shape(),
	radius(1.0f)
{
	this->initializeVertexBuffers();
}

Sphere::Sphere(const Vec3f* const position, const float radius) :
	Shape(position),
	radius(radius)
{
	this->initializeVertexBuffers();
}

Sphere::Sphere(const float x, const float y, const float z, const float radius) :
	Shape(x, y, z),
	radius(radius)
{
	this->initializeVertexBuffers();
}

float Sphere::getRadius() const{
	return this->radius;
}

void Sphere::addSize(const Vec3f* const other){
	const float* const size = other->getComponents();
	this->radius += size[0];
	this->initializeVertexBuffers();
}

void Sphere::setIdentifier(const int sequenceNumber){
	this->identifier = "sphere" + std::to_string(sequenceNumber);
}

void Sphere::initializeVertexBuffers(){
	if(this->needsPositionBuffer){
		glGenBuffers(1, &this->positionBuffer);
		this->needsPositionBuffer = false;
	}
	this->vertices.clear();
	for(int i = 1; i <= Shape::MAX_POINTS_CIRCLE; ++i){
		const float rcos = this->radius * cos((float)i * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
		const float rsin = this->radius * sin((float)i * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
		PUSH_BACK_3F(this->vertices, rcos, 0, rsin)
	}
	const float radiusSquared = this->radius * this->radius;
	for(int i = 1; i <= Shape::MAX_LAYERS_CIRCLE; ++i){
		const float layerHeight = ((float)i * this->radius) / (float)Shape::MAX_LAYERS_CIRCLE;
		const float layerRadius = sqrt(radiusSquared - (layerHeight * layerHeight));
		for(int j = 1; j <= Shape::MAX_POINTS_CIRCLE; ++j){
			const float rcos = layerRadius * cos((float)j * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
			const float rsin = layerRadius * sin((float)j * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
			PUSH_BACK_3F(this->vertices, rcos, layerHeight, rsin)
			PUSH_BACK_3F(this->vertices, rcos, -layerHeight, rsin)
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), (void*)this->vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if(this->needsColorBuffer){
		glGenBuffers(1, &this->colorBuffer);
		this->colors.clear();
		for(int i = 1; i <= Shape::MAX_POINTS_CIRCLE; ++i){
			const float color = 0.7f * ((float)i / (float)Shape::MAX_POINTS_CIRCLE);
			PUSH_BACK_4F(this->colors, color, color, color, 1.0f)
			PUSH_BACK_4F(this->colors, 0.1f, color, 0.1f, 1.0f)
		}
		for(int i = 1; i <= Shape::MAX_LAYERS_CIRCLE; ++i){
			for(int j = 1; j <= Shape::MAX_POINTS_CIRCLE; ++j){
				const float color = 0.7f * ((float)j / (float)Shape::MAX_POINTS_CIRCLE);
				PUSH_BACK_4F(this->colors, color, color, color, 1.0f)
				PUSH_BACK_4F(this->colors, 0.1f, color, 0.1f, 1.0f)
				PUSH_BACK_4F(this->colors, color, color, color, 1.0f)
				PUSH_BACK_4F(this->colors, 0.1f, color, 0.1f, 1.0f)
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->colors.size(), (void*)this->colors.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		this->needsColorBuffer = false;
	}
	if(this->needsElementBuffer){
		glGenBuffers(1, &this->elementBuffer);
		this->indices.clear();
		for(int i = 0; i < (Shape::MAX_POINTS_CIRCLE - 1); ++i){
			PUSH_BACK_3F(this->indices, i, i + 1, (2 * (i + 1)) + Shape::MAX_POINTS_CIRCLE)
			PUSH_BACK_3F(this->indices, i, (2 * i) + Shape::MAX_POINTS_CIRCLE, (2 * (i + 1)) + Shape::MAX_POINTS_CIRCLE)
			PUSH_BACK_3F(this->indices, i, i + 1, (2 * (i + 1)) + Shape::MAX_POINTS_CIRCLE + 1)
			PUSH_BACK_3F(this->indices, i, (2 * i) + Shape::MAX_POINTS_CIRCLE + 1, (2 * (i + 1)) + Shape::MAX_POINTS_CIRCLE + 1)
		}
		PUSH_BACK_3F(this->indices, Shape::MAX_POINTS_CIRCLE - 1, 0, Shape::MAX_POINTS_CIRCLE + 0)
		PUSH_BACK_3F(this->indices, Shape::MAX_POINTS_CIRCLE - 1, (3 * Shape::MAX_POINTS_CIRCLE) - 2, Shape::MAX_POINTS_CIRCLE + 0)
		PUSH_BACK_3F(this->indices, Shape::MAX_POINTS_CIRCLE - 1, 0, Shape::MAX_POINTS_CIRCLE + 1)
		PUSH_BACK_3F(this->indices, Shape::MAX_POINTS_CIRCLE - 1, (3 * Shape::MAX_POINTS_CIRCLE) - 1, Shape::MAX_POINTS_CIRCLE + 1)
		for(int i = 0; i < (Shape::MAX_LAYERS_CIRCLE - 1); ++i){
			for(int j = 0; j < (Shape::MAX_POINTS_CIRCLE - 1); ++j){
				PUSH_BACK_3F(this->indices, (((2 * i) + 1) * Shape::MAX_POINTS_CIRCLE) + (2 * j), (((2 * i) + 1) * Shape::MAX_POINTS_CIRCLE) + (2 * (j + 1)), (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) + (2 * (j + 1)))
				PUSH_BACK_3F(this->indices, (((2 * i) + 1) * Shape::MAX_POINTS_CIRCLE) + (2 * j), (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) + (2 * j), (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) + (2 * (j + 1)))
				PUSH_BACK_3F(this->indices, (((2 * i) + 1) * Shape::MAX_POINTS_CIRCLE) + (2 * j) + 1, (((2 * i) + 1) * Shape::MAX_POINTS_CIRCLE) + (2 * (j + 1)) + 1, (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) + (2 * (j + 1)) + 1)
				PUSH_BACK_3F(this->indices, (((2 * i) + 1) * Shape::MAX_POINTS_CIRCLE) + (2 * j) + 1, (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) + (2 * j) + 1, (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) + (2 * (j + 1)) + 1)
			}
			PUSH_BACK_3F(this->indices, (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) - 2, (((2 * i) + 1) * Shape::MAX_POINTS_CIRCLE), (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE))
			PUSH_BACK_3F(this->indices, (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) - 2, (((2 * i) + 5) * Shape::MAX_POINTS_CIRCLE) - 2, (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE))
			PUSH_BACK_3F(this->indices, (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) - 1, (((2 * i) + 1) * Shape::MAX_POINTS_CIRCLE) + 1, (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) + 1)
			PUSH_BACK_3F(this->indices, (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) - 1, (((2 * i) + 5) * Shape::MAX_POINTS_CIRCLE) - 1, (((2 * i) + 3) * Shape::MAX_POINTS_CIRCLE) + 1)
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * this->indices.size(), (void*)this->indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		this->needsElementBuffer = false;
	}
}

void Sphere::render(const Mat4f* const parentTransformations, const bool isSelected) const{
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
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

bool Sphere::apply(const int function, const float argument){
	switch(function){
	case Parser::FUNCTION_NAME_RADIUS:
		this->radius = argument;
		this->initializeVertexBuffers();
		return true;
	default:
		return Shape::apply(function, argument);
	}
}

bool Sphere::apply(const int function, const std::string& listElement){
	switch(function){
	case Parser::FUNCTION_NAME_TRANSFORMATIONS:
		return this->addTransformationFromWorkspace(listElement);
	default:
		return false;
	}
}

Sphere::~Sphere(){}
