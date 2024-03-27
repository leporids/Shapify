#ifndef CYLINDER_H
#define CYLINDER_H
#include "Shape.h"
#include "Parser.h"
#include "vectors.h"
class Cylinder : public Shape {
	public:
		PRINTABLE_CLASS

		Cylinder();
		Cylinder(const Vec3f* const position, const float height, const float radius);
		Cylinder(const float x, const float y, const float z, const float height, const float radius);

		float getHeight() const;
		float getRadius() const;
		virtual void addSize(const Vec3f* const other);

		virtual void setIdentifier(const int sequenceNumber);
		virtual void initializeVertexBuffers();

		virtual void render(const Mat4f* const parentTransformations, const bool isSelected) const;
		virtual bool apply(const int function, const float argument);
		virtual bool apply(const int function, const std::string& listElement);

		PRINTABLE_DECLARE_PRINT(Shape)
		ANIMATED_DECLARATION

		virtual ~Cylinder();
	private:
		float height;
		float radius;
		friend class Animation;
};
#endif // CYLINDER_H
