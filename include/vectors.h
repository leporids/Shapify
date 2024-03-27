#ifndef VECTORS_H
#define VECTORS_H
#include <stdio.h>
#include <math.h>
class Vec3f {
	public:
		Vec3f();
		Vec3f(const float x, const float y, const float z);
		Vec3f(const Vec3f* const other);

		const float* getComponents() const;
		void setComponents(const float x, const float y, const float z);
		void setComponents(const Vec3f* const other);

		void setX(const float x);
		void setY(const float y);
		void setZ(const float z);

		void add(const Vec3f* const other);
		void add(const Vec3f* const other, Vec3f* const result) const;
		void add(const float x, const float y, const float z);
		void add(const float x, const float y, const float z, Vec3f* const result) const;

		void multiply(const float scalar);
		void multiply(const float scalar, Vec3f* const result) const;

		void cross(const Vec3f* const other);
		void cross(const Vec3f* const other, Vec3f* const result) const;

		float length() const;
		void normalize();
		void normalize(Vec3f* const result) const;

		~Vec3f();
	private:
		float components[3];
		friend class Animation;
};
#endif // VECTORS_H
