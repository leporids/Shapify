#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include "vectors.h"
#include "matrices.h"
class Printer;
#include "Printable.h"
#include "Printer.h"
#include "Animated.h"
#include "Animation.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
class Transformation {
	public:
		static const int NONE = 0;
		static const int TRANSLATE = 1;
		static const int ROTATE = 2;
		static const int SCALE = 3;

		int getType() const;
		const Vec3f* getComponents() const;
		const Mat4f* getMatrix() const;
		const std::string& getIdentifier() const;

		virtual void setIdentifier(const std::string& identifier);
		virtual void setIdentifier(const int sequenceNumber);

		void addPosition(const Vec3f* const other);

		virtual void updateMatrix() = 0;
		virtual bool apply(const int function, const float argument);

		virtual void print(Printer* const printer) const = 0;

		ANIMATED_DECLARATION

		virtual ~Transformation();
	protected:
		Transformation(const int type, Vec3f* const components);
		Transformation(const int type, const float x, const float y, const float z);
		Transformation(const Transformation* const other);

		int type;
		int referenceCount;
		Vec3f components;
		Mat4f matrix;
		std::string identifier;
		PRINTABLE_CLASS_FRIENDSHIP
		friend class Shape;
		friend class Animation;
};
#endif // TRANSFORMATION_H
