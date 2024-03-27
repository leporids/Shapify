#ifndef SPHERE_H
#define SPHERE_H
#include "Shape.h"
#include "Parser.h"
#include "vectors.h"
class Sphere : public Shape {
	public:
		PRINTABLE_CLASS

		Sphere();
		Sphere(const Vec3f* const position, const float radius);
		Sphere(const float x, const float y, const float z, const float radius);

		float getRadius() const;
		virtual void addSize(const Vec3f* const other);

		virtual void setIdentifier(const int sequenceNumber);
		virtual void initializeVertexBuffers();

		virtual void render(const Mat4f* const parentTransformations, const bool isSelected) const;
		virtual bool apply(const int function, const float argument);
		virtual bool apply(const int function, const std::string& listElement);

		PRINTABLE_DECLARE_PRINT(Shape)
		ANIMATED_DECLARATION

		virtual ~Sphere();
	private:
		float radius;
		friend class Animation;
};
#endif // SPHERE_H
