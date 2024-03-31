#ifndef CONE_H
#define CONE_H
#include "Shape.h"
#include "Parser.h"
#include "vectors.h"
class Cone : public Shape {
	public:
		PRINTABLE_CLASS

		Cone();
		Cone(const Vec3f* const position, const float height, const float radius);
		Cone(const float x, const float y, const float z, const float height, const float radius);

		float getHeight() const;
		float getRadius() const;
		virtual void addSize(const Vec3f* const other);

		virtual void setIdentifier(const int sequenceNumber);
		virtual void initializeVertexBuffers();

		virtual void render(const Mat4f* const projection, const Mat4f* const view, const Mat4f* const parentTransformations, const float* const cameraPositionComponents, const bool isSelected) const;
		virtual bool apply(const int function, const float argument);
		virtual bool apply(const int function, const std::string& listElement);

		PRINTABLE_DECLARE_PRINT(Shape)
		ANIMATED_DECLARATION

		virtual ~Cone();
	private:
		float height;
		float radius;
		friend class Animation;
};
#endif // CONE_H
