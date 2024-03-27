#ifndef PRINTER_H
#define PRINTER_H
#include "IndentedStream.h"
#include "vectors.h"
#include <vector>
class Point;
class PlanePoints;
class Shape;
class Box;
class Cone;
class Cylinder;
class Sphere;
class Compound;
class Loft;
class Transformation;
class Translate;
class Rotate;
class Scale;
class Animation;
#define PRINTER_PRINT_CLASS(name) this->printClass(name->CLASS_NAME, name->identifier);
#define PRINTER_ALL_IDENTIFIERS_DECLARATION(t) virtual void printAllIdentifiersIn(const std::vector<t*>* const list, const std::string& label) const
class Printer {
	public:
		IndentedStream* getStream();
		void setStream(IndentedStream* const stream);

		virtual void printClass(const std::string& name, const std::string& identifier) const = 0;
		virtual void printLabelledVec3f(const Vec3f* const, const std::string& x, const std::string& y, const std::string& z) const = 0;
		virtual void printLabelledFloat(const float value, const std::string& label) const = 0;
		PRINTER_ALL_IDENTIFIERS_DECLARATION(Point) = 0;
		PRINTER_ALL_IDENTIFIERS_DECLARATION(PlanePoints) = 0;
		PRINTER_ALL_IDENTIFIERS_DECLARATION(Shape) = 0;
		PRINTER_ALL_IDENTIFIERS_DECLARATION(Transformation) = 0;

		virtual void printPoint(const Point* const point) const = 0;
		virtual void printPoint(const PlanePoints* const planePoints) const = 0;

		virtual void printShape(const Box* const box) const = 0;
		virtual void printShape(const Cone* const cone) const = 0;
		virtual void printShape(const Cylinder* const cylinder) const = 0;
		virtual void printShape(const Sphere* const sphere) const = 0;
		virtual void printShape(const Compound* const compound) const = 0;
		virtual void printShape(const Loft* const loft) const = 0;

		virtual void printTransformation(const Translate* const translate) const = 0;
		virtual void printTransformation(const Rotate* const rotate) const = 0;
		virtual void printTransformation(const Scale* const scale) const = 0;

		virtual void printAnimation(const Animation* const animation) const = 0;

		virtual ~Printer();
	protected:
		Printer(IndentedStream* const stream);

		IndentedStream* stream;
};
#endif // PRINTER_H
