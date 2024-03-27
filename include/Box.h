#ifndef BOX_H
#define BOX_H
#include "Shape.h"
#include "Parser.h"
#include "vectors.h"
class Box : public Shape {
	public:
		PRINTABLE_CLASS

		Box();
		Box(const Vec3f* const position, const Vec3f* const size);
		Box(const float x, const float y, const float z, const float l, const float w, const float h);

		const Vec3f* getSize() const;
		virtual void addSize(const Vec3f* const other);

		virtual void setIdentifier(const int sequenceNumber);

		virtual void initializeVertexBuffers();
		static const int NUMBER_OF_VERTICES = 8;
		static const int NUMBER_OF_TRIANGLES = 12;
		static const int NUMBER_OF_INDICES = 36;

		virtual void render(const Mat4f* const parentTransformations, const bool isSelected) const;
		virtual bool apply(const int function, const float argument);
		virtual bool apply(const int function, const std::string& listElement);

		PRINTABLE_DECLARE_PRINT(Shape)
		ANIMATED_DECLARATION

		virtual ~Box();
	private:
		Vec3f size;
		static const int indices[Box::NUMBER_OF_INDICES];
		friend class Animation;
};
#endif // BOX_H
