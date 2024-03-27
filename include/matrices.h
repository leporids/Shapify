#ifndef MATRICES_H
#define MATRICES_H
#include "vectors.h"
#include <cmath>
#define MAT4F_SET_ROW(i, x, y, z, w) \
	this->entries[i][0] = x; \
	this->entries[i][1] = y; \
	this->entries[i][2] = z; \
	this->entries[i][3] = w;
#define MAT4F_SET_COLUMN(j, x, y, z, w) \
	this->entries[0][j] = x; \
	this->entries[1][j] = y; \
	this->entries[2][j] = z; \
	this->entries[3][j] = w;
#define MAT4F_SET_ROW_FROM_MATRIX(i, name) \
	this->entries[i][0] = name[i][0]; \
	this->entries[i][1] = name[i][1]; \
	this->entries[i][2] = name[i][2]; \
	this->entries[i][3] = name[i][3];
#define MAT4F_GET_ROW(name, i) {name[i][0], name[i][1], name[i][2], name[i][3]}
#define MAT4F_GET_ALL_ROWS(name) MAT4F_GET_ROW(name, 0), MAT4F_GET_ROW(name, 1), MAT4F_GET_ROW(name, 2), MAT4F_GET_ROW(name, 3)
#define MAT4F_COPY_ROW(i, name) \
	name[i * Mat4f::COLUMNS] = this->entries[i][0]; \
	name[(i * Mat4f::COLUMNS) + 1] = this->entries[i][1]; \
	name[(i * Mat4f::COLUMNS) + 2] = this->entries[i][2]; \
	name[(i * Mat4f::COLUMNS) + 3] = this->entries[i][3];
#define MAT4F_SWAP_ENTRIES(entry, i, j, k, l) \
	entry = this->entries[i][j]; \
	this->entries[i][j] = this->entries[k][l]; \
	this->entries[k][l] = entry;
#define MAT4F_GET_MULTIPLIED_ROW(left, right, i) {(left[i][0] * right[0][0]) + (left[i][1] * right[1][0]) + (left[i][2] * right[2][0]) + (left[i][3] * right[3][0]), (left[i][0] * right[0][1]) + (left[i][1] * right[1][1]) + (left[i][2] * right[2][1]) + (left[i][3] * right[3][1]), (left[i][0] * right[0][2]) + (left[i][1] * right[1][2]) + (left[i][2] * right[2][2]) + (left[i][3] * right[3][2]), (left[i][0] * right[0][3]) + (left[i][1] * right[1][3]) + (left[i][2] * right[2][3]) + (left[i][3] * right[3][3])}
#define MAT4F_GET_ALL_MULTIPLIED_ROWS(left, right) MAT4F_GET_MULTIPLIED_ROW(left, right, 0), MAT4F_GET_MULTIPLIED_ROW(left, right, 1), MAT4F_GET_MULTIPLIED_ROW(left, right, 2), MAT4F_GET_MULTIPLIED_ROW(left, right, 3)
class Mat4f {
	public:
		Mat4f();
		Mat4f(const Mat4f* const other);
		Mat4f(const Mat4f* const leftMultiplicand, const Mat4f* const rightMultiplicand);

		float* getEntries() const;
		void getEntries(float* const entries) const;

		void setEntries(const Mat4f* const other);

		static const int ROWS = 4;
		static const int COLUMNS = 4;

		void zero();
		void identity();
		void transpose();
		void multiply(const Mat4f* const other, Mat4f* const result) const;

		void basisFromPositiveZ(const Vec3f* const z);
		void translate(const Vec3f* const other);
		void rotate(const float angle, const Vec3f* const other);
		void scale(const Vec3f* const other);

		void camera(const Vec3f* const positionVector, const Vec3f* const lookAtVector, const Vec3f* const upVector);
		void perspective(const float angle, const float ratio, const float zNear, const float zFar);

		~Mat4f();
	private:
		float entries[4][4];
};
#endif // MATRICES_H
