#include "WorkspacePrinter.h"

WorkspacePrinter::WorkspacePrinter(IndentedStream* const stream)
	: Printer(stream)
{}

void WorkspacePrinter::printClass(const std::string& name, const std::string& identifier) const{
	(*this->stream << name << ": " << identifier).endl();
}

void WorkspacePrinter::printLabelledVec3f(const Vec3f* const vec3f, const std::string& x, const std::string& y, const std::string& z) const{
	const float* const xyz = vec3f->getComponents();
	this->stream->increaseIndent().indentOnce();
	(*this->stream << x << xyz[0]).endl();
	(*this->stream << y << xyz[1]).endl();
	(*this->stream << z << xyz[2]).decreaseIndent().endl();
}

void WorkspacePrinter::printLabelledFloat(const float value, const std::string& label) const{
	(this->stream->indentOnce() << label << value).endl();
}

#define WORKSPACE_PRINTER_ALL_IDENTIFIERS(t) \
	void WorkspacePrinter::printAllIdentifiersIn(const std::vector<t*>* const list, const std::string& label) const{ \
		if(list->size() > 0){ \
			this->stream->indentOnce() << label << list->at(0)->identifier; \
			for(size_t i = 1; i < list->size(); ++i){ \
				*this->stream << "," << list->at(i)->identifier; \
			} \
			this->stream->endl(); \
		} \
	}

WORKSPACE_PRINTER_ALL_IDENTIFIERS(Point)
WORKSPACE_PRINTER_ALL_IDENTIFIERS(PlanePoints)
WORKSPACE_PRINTER_ALL_IDENTIFIERS(Shape)
WORKSPACE_PRINTER_ALL_IDENTIFIERS(Transformation)

void WorkspacePrinter::printPoint(const Point* const point) const{
	PRINTER_PRINT_CLASS(point)
	this->printLabelledVec3f(&point->position, "x: ", "y: ", "z: ");
	this->stream->endl();
}

void WorkspacePrinter::printPoint(const PlanePoints* const planePoints) const{
	PRINTER_PRINT_CLASS(planePoints)
	this->printLabelledVec3f(&planePoints->position, "x: ", "y: ", "z: ");
	this->printLabelledFloat(planePoints->type, "type: ");
	this->printAllIdentifiersIn(&planePoints->children, "points: ");
	this->stream->endl();
}

void WorkspacePrinter::printShape(const Box* const box) const{
	PRINTER_PRINT_CLASS(box)
	this->printLabelledVec3f(&box->position, "x: ", "y: ", "z: ");
	this->printLabelledVec3f(&box->size, "length: ", "width: ", "height: ");
	this->printLabelledFloat(box->isVisible ? 1.0f : 0.0f, "visible: ");
	this->printAllIdentifiersIn(&box->transformations, "transformations: ");
	this->stream->endl();
}

void WorkspacePrinter::printShape(const Cone* const cone) const{
	PRINTER_PRINT_CLASS(cone)
	this->printLabelledVec3f(&cone->position, "x: ", "y: ", "z: ");
	this->printLabelledFloat(cone->height, "height: ");
	this->printLabelledFloat(cone->radius, "radius: ");
	this->printLabelledFloat(cone->isVisible ? 1.0f : 0.0f, "visible: ");
	this->printAllIdentifiersIn(&cone->transformations, "transformations: ");
	this->stream->endl();
}

void WorkspacePrinter::printShape(const Cylinder* const cylinder) const{
	PRINTER_PRINT_CLASS(cylinder)
	this->printLabelledVec3f(&cylinder->position, "x: ", "y: ", "z: ");
	this->printLabelledFloat(cylinder->height, "height: ");
	this->printLabelledFloat(cylinder->radius, "radius: ");
	this->printLabelledFloat(cylinder->isVisible ? 1.0f : 0.0f, "visible: ");
	this->printAllIdentifiersIn(&cylinder->transformations, "transformations: ");
	this->stream->endl();
}

void WorkspacePrinter::printShape(const Sphere* const sphere) const{
	PRINTER_PRINT_CLASS(sphere)
	this->printLabelledVec3f(&sphere->position, "x: ", "y: ", "z: ");
	this->printLabelledFloat(sphere->radius, "radius: ");
	this->printLabelledFloat(sphere->isVisible ? 1.0f : 0.0f, "visible: ");
	this->printAllIdentifiersIn(&sphere->transformations, "transformations: ");
	this->stream->endl();
}

void WorkspacePrinter::printShape(const Compound* const compound) const{
	PRINTER_PRINT_CLASS(compound)
	this->printLabelledVec3f(&compound->position, "x: ", "y: ", "z: ");
	this->printLabelledFloat(compound->isVisible ? 1.0f : 0.0f, "visible: ");
	this->printAllIdentifiersIn(&compound->transformations, "transformations: ");
	this->printAllIdentifiersIn(&compound->children, "shapes: ");
	this->stream->endl();
}

void WorkspacePrinter::printShape(const Loft* const loft) const{
	PRINTER_PRINT_CLASS(loft)
	this->printLabelledVec3f(&loft->position, "x: ", "y: ", "z: ");
	this->printLabelledFloat(loft->isVisible ? 1.0f : 0.0f, "visible: ");
	this->printAllIdentifiersIn(&loft->transformations, "transformations: ");
	this->printAllIdentifiersIn(&loft->children, "points: ");
	this->stream->endl();
}

void WorkspacePrinter::printTransformation(const Translate* const translate) const{
	PRINTER_PRINT_CLASS(translate)
	this->printLabelledVec3f(&translate->components, "x: ", "y: ", "z: ");
	this->stream->endl();
}

void WorkspacePrinter::printTransformation(const Rotate* const rotate) const{
	PRINTER_PRINT_CLASS(rotate)
	this->printLabelledFloat(rotate->angle, "angle: ");
	this->printLabelledVec3f(&rotate->components, "x: ", "y: ", "z: ");
	this->stream->endl();
}

void WorkspacePrinter::printTransformation(const Scale* const scale) const{
	PRINTER_PRINT_CLASS(scale)
	this->printLabelledVec3f(&scale->components, "x: ", "y: ", "z: ");
	this->stream->endl();
}

void WorkspacePrinter::printAnimation(const Animation* const animation) const{
	std::string label;
	std::string type;
	std::string selector;
	FrameFunction* function;
	for(size_t i = 0; i < animation->frames.size(); ++i){
		(*this->stream << "Frame: " << i).endl();
		this->printLabelledFloat(animation->frames[i]->time, "time: ");
		for(size_t j = 0; j < animation->frames[i]->functions.size(); ++j){
			function = animation->frames[i]->functions[j];
			FrameFunction::getWorkspaceTypeString(function->type, type);
			Animation::getWorkspaceSelectorString(function->selector, selector);
			label = type + ": " + function->getIdentifier() + "." + selector + ",";
			this->printLabelledFloat(function->value, label);
		}
		this->stream->endl();
	}
	this->stream->endl();
}

WorkspacePrinter::~WorkspacePrinter(){}
