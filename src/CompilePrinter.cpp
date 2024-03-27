#include "CompilePrinter.h"

CompilePrinter::CompilePrinter(IndentedStream* const stream)
	: Printer(stream)
{}

void CompilePrinter::printClass(const std::string& name, const std::string& identifier) const{
	*this->stream << name << " " << identifier << "(";
}

void CompilePrinter::endConstructor() const{
	(*this->stream << ");").endl();
}

void CompilePrinter::printLabelledVec3f(const Vec3f* const vec3f, const std::string& x, const std::string& y, const std::string& z) const{
	const float* const xyz = vec3f->getComponents();
	*this->stream << "/* " << x << " */ " << xyz[0] << ", /* " << y << " */ " << xyz[1] << ", /* " << z << " */ " << xyz[2];
}

void CompilePrinter::printLabelledFloat(const float value, const std::string& label) const{
	*this->stream << "/* " << label << " */ " << value;
}

#define COMPILE_PRINTER_ALL_IDENTIFIERS(t) \
	void CompilePrinter::printAllIdentifiersIn(const std::vector<t*>* const list, const std::string& label) const{ \
		if(list->size() > 0){ \
			*this->stream << label << "(&" << list->at(0)->identifier << ");"; \
			for(size_t i = 1; i < list->size(); ++i){ \
				this->stream->endl() << label << "(&" << list->at(i)->identifier << ");"; \
			} \
		} \
	}

COMPILE_PRINTER_ALL_IDENTIFIERS(Point)
COMPILE_PRINTER_ALL_IDENTIFIERS(PlanePoints)
COMPILE_PRINTER_ALL_IDENTIFIERS(Shape)
COMPILE_PRINTER_ALL_IDENTIFIERS(Transformation)

void CompilePrinter::printCallIf(const bool predicate, const std::string& identifier, const std::string& method) const{
	if(predicate){
		(this->stream->indentOnce() << identifier << method << "();").endl();
	}
}

template <typename T>
void CompilePrinter::printCalls(const T* const list, const std::string& identifier, const std::string& method) const{
	if(list->size() > 0){
		std::string label(identifier);
		label += method;
		this->stream->increaseIndent().indentOnce();
		this->printAllIdentifiersIn(list, label);
		this->stream->decreaseIndent().endl();
	}
}

void CompilePrinter::printPoint(const Point* const point) const{
	PRINTER_PRINT_CLASS(point)
	this->printLabelledVec3f(&point->position, "x", "y", "z");
	this->endConstructor();
}

void CompilePrinter::printPoint(const PlanePoints* const planePoints) const{
	PRINTER_PRINT_CLASS(planePoints)
	this->printLabelledFloat((float)planePoints->type, "type");
	*this->stream << ", ";
	this->printLabelledVec3f(&planePoints->position, "x", "y", "z");
	this->endConstructor();
	this->printCalls(&planePoints->children, planePoints->identifier, ".addChild");
}

void CompilePrinter::printShape(const Box* const box) const{
	PRINTER_PRINT_CLASS(box)
	this->printLabelledVec3f(&box->position, "x", "y", "z");
	*this->stream << ", ";
	this->printLabelledVec3f(&box->size, "length", "width", "height");
	this->endConstructor();
	this->printCallIf(!box->isVisible, box->identifier, ".hide");
	this->printCalls(&box->transformations, box->identifier, ".addTransformation");
}

void CompilePrinter::printShape(const Cone* const cone) const{
	PRINTER_PRINT_CLASS(cone)
	this->printLabelledVec3f(&cone->position, "x", "y", "z");
	*this->stream << ", ";
	this->printLabelledFloat(cone->height, "height");
	*this->stream << ", ";
	this->printLabelledFloat(cone->radius, "radius");
	this->endConstructor();
	this->printCallIf(!cone->isVisible, cone->identifier, ".hide");
	this->printCalls(&cone->transformations, cone->identifier, ".addTransformation");
}

void CompilePrinter::printShape(const Cylinder* const cylinder) const{
	PRINTER_PRINT_CLASS(cylinder)
	this->printLabelledVec3f(&cylinder->position, "x", "y", "z");
	*this->stream << ", ";
	this->printLabelledFloat(cylinder->height, "height");
	*this->stream << ", ";
	this->printLabelledFloat(cylinder->radius, "radius");
	this->endConstructor();
	this->printCallIf(!cylinder->isVisible, cylinder->identifier, ".hide");
	this->printCalls(&cylinder->transformations, cylinder->identifier, ".addTransformation");
}

void CompilePrinter::printShape(const Sphere* const sphere) const{
	PRINTER_PRINT_CLASS(sphere)
	this->printLabelledVec3f(&sphere->position, "x", "y", "z");
	*this->stream << ", ";
	this->printLabelledFloat(sphere->radius, "radius");
	this->endConstructor();
	this->printCallIf(!sphere->isVisible, sphere->identifier, ".hide");
	this->printCalls(&sphere->transformations, sphere->identifier, ".addTransformation");
}

void CompilePrinter::printShape(const Compound* const compound) const{
	PRINTER_PRINT_CLASS(compound)
	this->printLabelledVec3f(&compound->position, "x", "y", "z");
	this->endConstructor();
	this->printCallIf(!compound->isVisible, compound->identifier, ".hide");
	this->printCalls(&compound->transformations, compound->identifier, ".addTransformation");
	this->printCalls(&compound->children, compound->identifier, ".addChild");
}

void CompilePrinter::printShape(const Loft* const loft) const{
	PRINTER_PRINT_CLASS(loft)
	this->printLabelledVec3f(&loft->position, "x", "y", "z");
	this->endConstructor();
	this->printCallIf(!loft->isVisible, loft->identifier, ".hide");
	this->printCalls(&loft->transformations, loft->identifier, ".addTransformation");
	this->printCalls(&loft->children, loft->identifier, ".addChild");
}

void CompilePrinter::printTransformation(const Translate* const translate) const{
	PRINTER_PRINT_CLASS(translate)
	this->printLabelledVec3f(&translate->components, "x", "y", "z");
	this->endConstructor();
}

void CompilePrinter::printTransformation(const Rotate* const rotate) const{
	PRINTER_PRINT_CLASS(rotate)
	this->printLabelledFloat(rotate->angle, "angle");
	*this->stream << ", ";
	this->printLabelledVec3f(&rotate->components, "x", "y", "z");
	this->endConstructor();
}

void CompilePrinter::printTransformation(const Scale* const scale) const{
	PRINTER_PRINT_CLASS(scale)
	this->printLabelledVec3f(&scale->components, "x", "y", "z");
	this->endConstructor();
}

void CompilePrinter::printAnimation(const Animation* const animation) const{
	(*this->stream << "Animation animation;").endl();
	this->printFrames(&animation->frames);
}

void CompilePrinter::printFrames(const std::vector<Frame*>* const frames) const{
	if(frames->size() > 0){
		this->stream->increaseIndent().indentOnce() << "animation.addFrame(" << frames->at(0)->time << ");";
		this->printFrameFunctions(&frames->at(0)->functions);
		for(size_t i = 1; i < frames->size(); ++i){
			*this->stream << "animation.addFrame(" << frames->at(i)->time << ");";
			this->printFrameFunctions(&frames->at(i)->functions);
		}
		this->stream->decreaseIndent().endl();
	}
}

void CompilePrinter::printFrameFunctions(const std::vector<FrameFunction*>* const functions) const{
	if(functions->size() > 0){
		std::string type;
		std::string selector;
		FrameFunction* function = functions->at(0);
		FrameFunction::getTypeString(function->type, type);
		Animation::getSelectorString(function->selector, selector);
		this->stream->increaseIndent().endl() << "animation.addFrameFunction(" << type << ", " << "&" << function->getIdentifier() << ", " << selector << ", " << function->value << ");";
		for(size_t i = 1; i < functions->size(); ++i){
			function = functions->at(i);
			FrameFunction::getTypeString(function->type, type);
			Animation::getSelectorString(function->selector, selector);
			this->stream->endl() << "animation.addFrameFunction(" << type << ", " << "&" << function->getIdentifier() << ", " << selector << ", " << function->value << ");";
		}
		this->stream->decreaseIndent().endl();
	}else{
		this->stream->endl();
	}
}

CompilePrinter::~CompilePrinter(){}
