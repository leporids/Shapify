#include <cmath>
#include "Cone.h"
#include "units.h"

PRINTABLE_CLASS_NAME(Cone)

Cone::Cone() :
	Shape(),
	height(1.0f),
	radius(1.0f)
{
	this->initializeVertexBuffers();
}

Cone::Cone(const Vec3f* const position, const float height, const float radius) :
	Shape(position),
	height(height),
	radius(radius)
{
	this->initializeVertexBuffers();
}

Cone::Cone(const float x, const float y, const float z, const float height, const float radius) :
	Shape(x, y, z),
	height(height),
	radius(radius)
{
	this->initializeVertexBuffers();
}

float Cone::getHeight() const{
	return this->height;
}

float Cone::getRadius() const{
	return this->radius;
}

void Cone::addSize(const Vec3f* const other){
	const float* const size = other->getComponents();
	this->height += size[1];
	this->radius += size[0];
	this->initializeVertexBuffers();
}

void Cone::setIdentifier(const int sequenceNumber){
	this->identifier = "cone" + std::to_string(sequenceNumber);
}

void Cone::initializeVertexBuffers(){
	if(this->needsPositionBuffer){
		glGenBuffers(1, &this->positionBuffer);
		this->needsPositionBuffer = false;
	}
	this->vertices.clear();
	this->normals.clear();
	const float bottom = -this->height / 2.0f;
	PUSH_BACK_3F(this->vertices, 0.0f, bottom, 0.0f)
	PUSH_BACK_3F(this->normals, 0.0f, -1.0f, 0.0f)
	for(int i = 1; i <= Shape::MAX_POINTS_CIRCLE; ++i){
		const float rcos = this->radius * cos((float)i * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
		const float rsin = this->radius * sin((float)i * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
		const float hcos = this->height * cos((float)i * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
		const float hsin = this->height * sin((float)i * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
		PUSH_BACK_3F(this->vertices, rcos, bottom, rsin)
		PUSH_BACK_3F(this->normals, hcos, this->radius, hsin)
	}
	for(int i = 1; i <= Shape::MAX_LAYERS_CIRCLE; ++i){
		const float layerRadius = this->radius * ((float)(Shape::MAX_LAYERS_CIRCLE - i) / (float)Shape::MAX_LAYERS_CIRCLE);
		const float layerHeight = bottom + (((float)i * this->height) / (float)Shape::MAX_LAYERS_CIRCLE);
		for(int j = 1; j <= Shape::MAX_POINTS_CIRCLE; ++j){
			const float rcos = layerRadius * cos((float)j * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
			const float rsin = layerRadius * sin((float)j * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
			const float hcos = this->height * cos((float)j * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
			const float hsin = this->height * sin((float)j * DEGREES_TO_RADIANS(360.0f / (float)Shape::MAX_POINTS_CIRCLE));
			PUSH_BACK_3F(this->vertices, rcos, layerHeight, rsin)
			PUSH_BACK_3F(this->normals, hcos, this->radius, hsin)
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), (void*)this->vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if(this->needsColorBuffer){
		glGenBuffers(1, &this->colorBuffer);
		this->colors.clear();
		PUSH_BACK_4F(this->colors, Shape::DEFAULT_COLOR_R, Shape::DEFAULT_COLOR_G, Shape::DEFAULT_COLOR_B, Shape::DEFAULT_COLOR_A)
		PUSH_BACK_4F(this->colors, Shape::DEFAULT_SELECTED_COLOR_R, Shape::DEFAULT_SELECTED_COLOR_G, Shape::DEFAULT_SELECTED_COLOR_B, Shape::DEFAULT_SELECTED_COLOR_A)
		for(int i = 1; i <= Shape::MAX_POINTS_CIRCLE; ++i){
			PUSH_BACK_4F(this->colors, Shape::DEFAULT_COLOR_R, Shape::DEFAULT_COLOR_G, Shape::DEFAULT_COLOR_B, Shape::DEFAULT_COLOR_A)
			PUSH_BACK_4F(this->colors, Shape::DEFAULT_SELECTED_COLOR_R, Shape::DEFAULT_SELECTED_COLOR_G, Shape::DEFAULT_SELECTED_COLOR_B, Shape::DEFAULT_SELECTED_COLOR_A)
		}
		for(int i = 1; i <= Shape::MAX_LAYERS_CIRCLE; ++i){
			for(int j = 1; j <= Shape::MAX_POINTS_CIRCLE; ++j){
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
			PUSH_BACK_3F(this->indices, 0, i + 1, i + 2)
		}
		PUSH_BACK_3F(this->indices, 0, (Shape::MAX_POINTS_CIRCLE + 0), 1)
		for(int i = 0; i < Shape::MAX_LAYERS_CIRCLE; ++i){
			for(int j = 0; j < (Shape::MAX_POINTS_CIRCLE - 1); ++j){
				PUSH_BACK_3F(this->indices, (i * Shape::MAX_POINTS_CIRCLE) + j + 1, (i * Shape::MAX_POINTS_CIRCLE) + j + 2, ((i + 1) * Shape::MAX_POINTS_CIRCLE) + j + 2)
				PUSH_BACK_3F(this->indices, (i * Shape::MAX_POINTS_CIRCLE) + j + 1, ((i + 1) * Shape::MAX_POINTS_CIRCLE) + j + 1, ((i + 1) * Shape::MAX_POINTS_CIRCLE) + j + 2)
			}
			PUSH_BACK_3F(this->indices, (i * Shape::MAX_POINTS_CIRCLE) + Shape::MAX_POINTS_CIRCLE, (i * Shape::MAX_POINTS_CIRCLE) + 1, ((i + 1) * Shape::MAX_POINTS_CIRCLE) + 1)
			PUSH_BACK_3F(this->indices, (i * Shape::MAX_POINTS_CIRCLE) + Shape::MAX_POINTS_CIRCLE, ((i + 1) * Shape::MAX_POINTS_CIRCLE) + Shape::MAX_POINTS_CIRCLE, ((i + 1) * Shape::MAX_POINTS_CIRCLE) + 1)
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * this->indices.size(), (void*)this->indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		this->needsElementBuffer = false;
	}
}

bool Cone::apply(const int function, const float argument){
	switch(function){
	case Parser::FUNCTION_NAME_HEIGHT:
		this->height = argument;
		this->initializeVertexBuffers();
		return true;
	case Parser::FUNCTION_NAME_RADIUS:
		this->radius = argument;
		this->initializeVertexBuffers();
		return true;
	default:
		return Shape::apply(function, argument);
	}
}

bool Cone::apply(const int function, const std::string& listElement){
	switch(function){
	case Parser::FUNCTION_NAME_TRANSFORMATIONS:
		return this->addTransformationFromWorkspace(listElement);
	default:
		return false;
	}
}

Cone::~Cone(){}
