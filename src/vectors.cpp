#include "vectors.h"

Vec3f::Vec3f() :
	Vec3f(0.0, 0.0, 0.0)
{}

Vec3f::Vec3f(const float x, const float y, const float z){
	this->setComponents(x, y, z);
}

Vec3f::Vec3f(const Vec3f* const other){
	this->setComponents(other);
}

const float* Vec3f::getComponents() const{
	return this->components;
}

void Vec3f::setComponents(const float x, const float y, const float z){
	this->components[0] = x;
	this->components[1] = y;
	this->components[2] = z;
}

void Vec3f::setComponents(const Vec3f* const other){
	this->components[0] = other->components[0];
	this->components[1] = other->components[1];
	this->components[2] = other->components[2];
}

void Vec3f::setX(const float x){
	this->components[0] = x;
}

void Vec3f::setY(const float y){
	this->components[1] = y;
}

void Vec3f::setZ(const float z){
	this->components[2] = z;
}

void Vec3f::add(const Vec3f* const other){
	this->components[0] += other->components[0];
	this->components[1] += other->components[1];
	this->components[2] += other->components[2];
}

void Vec3f::add(const Vec3f* const other, Vec3f* const result) const{
	result->setComponents(this);
	result->components[0] += other->components[0];
	result->components[1] += other->components[1];
	result->components[2] += other->components[2];
}

void Vec3f::add(const float x, const float y, const float z){
	this->components[0] += x;
	this->components[1] += y;
	this->components[2] += z;
}

void Vec3f::add(const float x, const float y, const float z, Vec3f* const result) const{
	result->setComponents(this);
	result->components[0] += x;
	result->components[1] += y;
	result->components[2] += z;
}

void Vec3f::multiply(const float scalar){
	this->components[0] *= scalar;
	this->components[1] *= scalar;
	this->components[2] *= scalar;
}

void Vec3f::multiply(const float scalar, Vec3f* const result) const{
	result->setComponents(this);
	result->components[0] *= scalar;
	result->components[1] *= scalar;
	result->components[2] *= scalar;
}

void Vec3f::cross(const Vec3f* const other){
	this->setComponents((this->components[1] * other->components[2]) - (this->components[2] * other->components[1]), (this->components[2] * other->components[0]) - (this->components[0] * other->components[2]), (this->components[0] * other->components[1]) - (this->components[1] * other->components[0]));
}

void Vec3f::cross(const Vec3f* const other, Vec3f* const result) const{
	result->setComponents(this);
	result->cross(other);
}

float Vec3f::length() const{
	return sqrt((this->components[0] * this->components[0]) + (this->components[1] * this->components[1]) + (this->components[2] * this->components[2]));
}

void Vec3f::normalize(){
	const float length = this->length();
	if(length > 0){
		this->multiply(1.0f / length);
	}
}

void Vec3f::normalize(Vec3f* const result) const{
	const float length = this->length();
	result->setComponents(this);
	if(length > 0){
		result->multiply(1.0f / length);
	}
}

Vec3f::~Vec3f(){}
