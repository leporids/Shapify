#ifndef COMPILE_PRINTER_H
#define COMPILE_PRINTER_H
#include "Printer.h"
#include "Point.h"
#include "PlanePoints.h"
#include "Shape.h"
#include "Box.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Compound.h"
#include "Loft.h"
#include "Transformation.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "Animation.h"
class CompilePrinter : public Printer {
	public:
		CompilePrinter(IndentedStream* const stream);

		virtual void printClass(const std::string& name, const std::string& identifier) const;
		virtual void endConstructor() const;
		virtual void printLabelledVec3f(const Vec3f* const, const std::string& x, const std::string& y, const std::string& z) const;
		virtual void printLabelledFloat(const float value, const std::string& label) const;
		PRINTER_ALL_IDENTIFIERS_DECLARATION(Point);
		PRINTER_ALL_IDENTIFIERS_DECLARATION(PlanePoints);
		PRINTER_ALL_IDENTIFIERS_DECLARATION(Shape);
		PRINTER_ALL_IDENTIFIERS_DECLARATION(Transformation);
		void printCallIf(const bool predicate, const std::string& identifier, const std::string& method) const;
		template <typename T> void printCalls(const T* const list, const std::string& identifier, const std::string& method) const;

		virtual void printPoint(const Point* const point) const;
		virtual void printPoint(const PlanePoints* const planePoints) const;

		virtual void printShape(const Box* const box) const;
		virtual void printShape(const Cone* const cone) const;
		virtual void printShape(const Cylinder* const cylinder) const;
		virtual void printShape(const Sphere* const sphere) const;
		virtual void printShape(const Compound* const compound) const;
		virtual void printShape(const Loft* const loft) const;

		virtual void printTransformation(const Translate* const translate) const;
		virtual void printTransformation(const Rotate* const rotate) const;
		virtual void printTransformation(const Scale* const scale) const;

		virtual void printAnimation(const Animation* const animation) const;
		virtual void printFrames(const std::vector<Frame*>* const frames) const;
		virtual void printFrameFunctions(const std::vector<FrameFunction*>* const functions) const;

		virtual ~CompilePrinter();
};
#endif // COMPILE_PRINTER_H
