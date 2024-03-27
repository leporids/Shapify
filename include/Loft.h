#ifndef LOFT_H
#define LOFT_H
#include "Shape.h"
#include "Parser.h"
#include "vectors.h"
class Loft : public Shape {
	public:
		PRINTABLE_CLASS

		Loft();
		Loft(const Vec3f* const position);
		Loft(const float x, const float y, const float z);

		virtual void setIdentifier(const int sequenceNumber);

		virtual void addSize(const Vec3f* const other);

		bool addChild(PlanePoints* const planePoints);
		bool addChildFromWorkspace(const std::string& identifier);
		size_t size() const;

		virtual void render(const Mat4f* const parentTransformations, const bool isSelected) const;
		virtual bool apply(const int function, const std::string& listElement);

		PRINTABLE_DECLARE_PRINT(Shape)
		ANIMATED_DECLARATION

		virtual ~Loft();
	private:
		std::vector<PlanePoints*> children;
};
#endif // LOFT_H
