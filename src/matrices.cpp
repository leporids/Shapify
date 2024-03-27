#include "matrices.h"
#include "units.h"

Mat4f::Mat4f() :
	entries{}
{}

Mat4f::Mat4f(const Mat4f* const other) :
	entries{MAT4F_GET_ALL_ROWS(other->entries)}
{}

Mat4f::Mat4f(const Mat4f* const leftMultiplicand, const Mat4f* const rightMultiplicand) :
	entries{MAT4F_GET_ALL_MULTIPLIED_ROWS(leftMultiplicand->entries, rightMultiplicand->entries)}
{}

float* Mat4f::getEntries() const{
	return (float*)this->entries;
}

void Mat4f::getEntries(float* const entries) const{
	MAT4F_COPY_ROW(0, entries)
	MAT4F_COPY_ROW(1, entries)
	MAT4F_COPY_ROW(2, entries)
	MAT4F_COPY_ROW(3, entries)
}

void Mat4f::setEntries(const Mat4f* const other){
	MAT4F_SET_ROW_FROM_MATRIX(0, other->entries)
	MAT4F_SET_ROW_FROM_MATRIX(1, other->entries)
	MAT4F_SET_ROW_FROM_MATRIX(2, other->entries)
	MAT4F_SET_ROW_FROM_MATRIX(3, other->entries)
}

void Mat4f::zero(){
	MAT4F_SET_ROW(0, 0, 0, 0, 0)
	MAT4F_SET_ROW(1, 0, 0, 0, 0)
	MAT4F_SET_ROW(2, 0, 0, 0, 0)
	MAT4F_SET_ROW(3, 0, 0, 0, 0)
}

void Mat4f::identity(){
	MAT4F_SET_ROW(0, 1, 0, 0, 0)
	MAT4F_SET_ROW(1, 0, 1, 0, 0)
	MAT4F_SET_ROW(2, 0, 0, 1, 0)
	MAT4F_SET_ROW(3, 0, 0, 0, 1)
}

void Mat4f::transpose(){
	float entry = 0.0f;
	MAT4F_SWAP_ENTRIES(entry, 0, 1, 1, 0)
	MAT4F_SWAP_ENTRIES(entry, 0, 2, 2, 0)
	MAT4F_SWAP_ENTRIES(entry, 0, 3, 3, 0)
	MAT4F_SWAP_ENTRIES(entry, 1, 2, 2, 1)
	MAT4F_SWAP_ENTRIES(entry, 1, 3, 3, 1)
	MAT4F_SWAP_ENTRIES(entry, 2, 3, 3, 2)
}

void Mat4f::multiply(const Mat4f* const other, Mat4f* const result) const{
	for(int i = 0; i < Mat4f::ROWS; ++i){
		for(int j = 0; j < Mat4f::COLUMNS; ++j){
			result->entries[i][j] = (this->entries[i][0] * other->entries[0][j]) + (this->entries[i][1] * other->entries[1][j]) + (this->entries[i][2] * other->entries[2][j]) + (this->entries[i][3] * other->entries[3][j]);
		}
	}
}

void Mat4f::basisFromPositiveZ(const Vec3f* const z){
	Vec3f zNormalized(z);
	zNormalized.normalize();
	if(zNormalized.length() > 0){
		const float* const components = zNormalized.getComponents();
		const float pitch = asin(-components[1]);
		const float yaw = atan2(components[0], components[2]);
		MAT4F_SET_ROW(0, cos(yaw), 0, -sin(yaw), 0)
		MAT4F_SET_ROW(1, -sin(yaw) * components[1], cos(pitch), -cos(yaw) * components[1], 0)
		MAT4F_SET_ROW(2, components[0], components[1], components[2], 0)
		MAT4F_SET_ROW(3, 0, 0, 0, 1)
	}else{
		this->identity();
	}
}

void Mat4f::translate(const Vec3f* const other){
	const float* const components = other->getComponents();
	MAT4F_SET_ROW(0, 1, 0, 0, components[0])
	MAT4F_SET_ROW(1, 0, 1, 0, components[1])
	MAT4F_SET_ROW(2, 0, 0, 1, components[2])
	MAT4F_SET_ROW(3, 0, 0, 0, 1)
}

void Mat4f::rotate(const float angle, const Vec3f* const other){
	if(other->length() > 0){
		const float cosAngle = cos(DEGREES_TO_RADIANS(angle));
		const float sinAngle = sin(DEGREES_TO_RADIANS(angle));
		Mat4f basis;
		basis.basisFromPositiveZ(other);
		Mat4f transpose(&basis);
		transpose.transpose();
		MAT4F_SET_ROW(0, cosAngle, -sinAngle, 0, 0)
		MAT4F_SET_ROW(1, sinAngle, cosAngle, 0, 0)
		MAT4F_SET_ROW(2, 0, 0, 1, 0)
		MAT4F_SET_ROW(3, 0, 0, 0, 1)
		Mat4f multiplied(&transpose, this);
		multiplied.multiply(&basis, this);
	}else{
		this->identity();
	}
}

void Mat4f::scale(const Vec3f* const other){
	const float* const components = other->getComponents();
	MAT4F_SET_ROW(0, components[0], 0, 0, 0)
	MAT4F_SET_ROW(1, 0, components[1], 0, 0)
	MAT4F_SET_ROW(2, 0, 0, components[2], 0)
	MAT4F_SET_ROW(3, 0, 0, 0, 1)
}

void Mat4f::camera(const Vec3f* const positionVector, const Vec3f* const lookAtVector, const Vec3f* const upVector){
	const float* const position = positionVector->getComponents();
	const float* const lookAt = lookAtVector->getComponents();
	const float lookAtLength = lookAtVector->length();
	Vec3f cross(lookAtVector);
	cross.cross(upVector);
	cross.normalize();
	Vec3f crossUp(cross);
	crossUp.cross(lookAtVector);
	crossUp.normalize();
	const float* const right = cross.getComponents();
	const float* const up = crossUp.getComponents();
	MAT4F_SET_ROW(0, right[0], right[1], right[2], -(position[0] * right[0]) - (position[1] * right[1]) - (position[2] * right[2]))
	MAT4F_SET_ROW(1, up[0], up[1], up[2], -(position[0] * up[0]) - (position[1] * up[1]) - (position[2] * up[2]))
	MAT4F_SET_ROW(2, -lookAt[0] / lookAtLength, -lookAt[1] / lookAtLength, -lookAt[2] / lookAtLength, ((position[0] * lookAt[0]) + (position[1] * lookAt[1]) + (position[2] * lookAt[2])) / lookAtLength)
	MAT4F_SET_ROW(3, 0, 0, 0, 1)
}

void Mat4f::perspective(const float angle, const float ratio, const float zNear, const float zFar){
	const float planeDistance = 1.0f / tan(DEGREES_TO_RADIANS(angle / 2.0f));
	const float nearFarSum = zNear + zFar;
	const float nearFarDifference = zNear - zFar;
	const float nearFarProduct = zNear * zFar;
	MAT4F_SET_ROW(0, planeDistance / ratio, 0, 0, 0)
	MAT4F_SET_ROW(1, 0, planeDistance, 0, 0)
	MAT4F_SET_ROW(2, 0, 0, nearFarSum / nearFarDifference, 2.0f * nearFarProduct / nearFarDifference)
	MAT4F_SET_ROW(3, 0, 0, -1, 0)
}

Mat4f::~Mat4f(){}
