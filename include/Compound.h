#ifndef COMPOUND_H
#define COMPOUND_H
#include "Shape.h"
#include "Parser.h"
#include "vectors.h"
class Compound : public Shape {
	public:
		PRINTABLE_CLASS

		Compound();
		Compound(const Vec3f* const position);
		Compound(const float x, const float y, const float z);

		virtual void setIdentifier(const int sequenceNumber);

		virtual void addSize(const Vec3f* const other);

		bool addChild(Shape* const shape);
		bool addChildFromWorkspace(const std::string& identifier);
		size_t size() const;

		virtual void render(const Mat4f* const parentTransformations, const bool isSelected) const;
		virtual bool apply(const int function, const std::string& listElement);

		PRINTABLE_DECLARE_PRINT(Shape)
		ANIMATED_DECLARATION

		virtual ~Compound();
	private:
		std::vector<Shape*> children;
};
#endif // COMPOUND_H
