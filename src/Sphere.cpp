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
	this->normals.clear();
	for(int i = 1; i <= Shape::MAX_POINTS_CIRCLE; ++i){
		const float rcos = this->radius * cos((float)i * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
		const float rsin = this->radius * sin((float)i * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
		PUSH_BACK_3F(this->vertices, rcos, 0, rsin)
		PUSH_BACK_3F(this->normals, rcos, 0, rsin)
	}
	const float radiusSquared = this->radius * this->radius;
	for(int i = 1; i <= Shape::MAX_LAYERS_CIRCLE; ++i){
		const float layerHeight = ((float)i * this->radius) / (float)Shape::MAX_LAYERS_CIRCLE;
		const float layerRadius = sqrt(radiusSquared - (layerHeight * layerHeight));
		for(int j = 1; j <= Shape::MAX_POINTS_CIRCLE; ++j){
			const float rcos = layerRadius * cos((float)j * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
			const float rsin = layerRadius * sin((float)j * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
			PUSH_BACK_3F(this->vertices, rcos, layerHeight, rsin)
			PUSH_BACK_3F(this->normals, rcos, layerHeight, rsin)
			PUSH_BACK_3F(this->vertices, rcos, -layerHeight, rsin)
			PUSH_BACK_3F(this->normals, rcos, -layerHeight, rsin)
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), (void*)this->vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if(this->needsColorBuffer){
		glGenBuffers(1, &this->colorBuffer);
		this->colors.clear();
		for(int i = 1; i <= Shape::MAX_POINTS_CIRCLE; ++i){
			PUSH_BACK_4F(this->colors, Shape::DEFAULT_COLOR_R, Shape::DEFAULT_COLOR_G, Shape::DEFAULT_COLOR_B, Shape::DEFAULT_COLOR_A)
			PUSH_BACK_4F(this->colors, Shape::DEFAULT_SELECTED_COLOR_R, Shape::DEFAULT_SELECTED_COLOR_G, Shape::DEFAULT_SELECTED_COLOR_B, Shape::DEFAULT_SELECTED_COLOR_A)
		}
		for(int i = 1; i <= Shape::MAX_LAYERS_CIRCLE; ++i){
			for(int j = 1; j <= Shape::MAX_POINTS_CIRCLE; ++j){
				PUSH_BACK_4F(this->colors, Shape::DEFAULT_COLOR_R, Shape::DEFAULT_COLOR_G, Shape::DEFAULT_COLOR_B, Shape::DEFAULT_COLOR_A)
				PUSH_BACK_4F(this->colors, Shape::DEFAULT_SELECTED_COLOR_R, Shape::DEFAULT_SELECTED_COLOR_G, Shape::DEFAULT_SELECTED_COLOR_B, Shape::DEFAULT_SELECTED_COLOR_A)
				PUSH_BACK_4F(this->colors, Shape::DEFAULT_COLOR_R, Shape::DEFAULT_COLOR_G, Shape::DEFAULT_COLOR_B, Shape::DEFAULT_COLOR_A)
				PUSH_BACK_4F(this->colors, Shape::DEFAULT_SELECTED_COLOR_R, Shape::DEFAULT_SELECTED_COLOR_G, Shape::DEFAULT_SELECTED_COLOR_B, Shape::DEFAULT_SELECTED_COLOR_A)
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->colors.size(), (void*)this->colors.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		this->needsColorBuffer = false;
	}
	if(this->needsNormalBuffer){
		glGenBuffers(1, &this->normalBuffer);
		this->needsNormalBuffer = false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->normals.size(), (void*)this->normals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void Sphere::render(const Mat4f* const projection, const Mat4f* const view, const Mat4f* const parentTransformations, const float* const cameraPositionComponents, const bool isSelected) const{
	Mat4f model;
	this->transformToWorld(&this->position, &model);
	Mat4f allTransformations(parentTransformations, &model);
	glUseProgram(this->shader);
	SHAPE_UNIFORM_MATRICES(projection, view, allTransformations)
	SHAPE_ATTRIB_POINTER(position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0)
	SHAPE_ATTRIB_POINTER(color, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (isSelected ? (4 * sizeof(float)) : 0))
	SHAPE_ATTRIB_POINTER(normal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0)
	SHAPE_UNIFORM_DEFAULT_LIGHTING(cameraPositionComponents)
	SHAPE_DRAW_ELEMENTS(this->indices.size())
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
