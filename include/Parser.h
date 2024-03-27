#ifndef PARSER_H
#define PARSER_H
#include "Workspace.h"
#include "PlanePoints.h"
#include "Box.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Compound.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
class Parser {
	public:
		Parser(Workspace* const outputWorkspace);

		bool parseDirectory(const char* const directory);
		static const int FUNCTION_NAME_X = 0;
		static const int FUNCTION_NAME_Y = 1;
		static const int FUNCTION_NAME_Z = 2;
		static const int FUNCTION_NAME_LENGTH = 3;
		static const int FUNCTION_NAME_WIDTH = 4;
		static const int FUNCTION_NAME_HEIGHT = 5;
		static const int FUNCTION_NAME_RADIUS = 6;
		static const int FUNCTION_NAME_ANGLE = 7;
		static const int FUNCTION_NAME_TIME = 8;
		static const int FUNCTION_NAME_TYPE = 9;
		static const int FUNCTION_NAME_VISIBLE = 10;
		static const int FUNCTION_NAME_IS_NUMERIC = 11;
		static const int FUNCTION_NAME_POINTS = 12;
		static const int FUNCTION_NAME_TRANSFORMATIONS = 13;
		static const int FUNCTION_NAME_SHAPES = 14;
		static const int FUNCTION_NAME_IS_LIST = 15;
		static const int FUNCTION_NAME_SET_TO = 16;
		static const int FUNCTION_NAME_LINEAR_TO = 17;
		static const int FUNCTION_NAME_IS_INTERPOLATION = 18;

		~Parser();
	private:
		FILE* file;
		int iterator;
		int lineCounter;
		int columnCounter;
		int state;
		static const int DATA_LENGTH = 256;
		static const int CURRENT_WORD_LENGTH = 256;
		char currentWord[Parser::CURRENT_WORD_LENGTH];

		static const int READING_CLASS_NAME = 0;
		static const int READING_IDENTIFIER = 1;
		static const int READING_FUNCTION_NAME = 2;
		static const int READING_FUNCTION_ARGUMENT = 3;

		int currentClassName;
		static const int CLASS_NAME_BOX = 0;
		static const int CLASS_NAME_CONE = 1;
		static const int CLASS_NAME_CYLINDER = 2;
		static const int CLASS_NAME_SPHERE = 3;
		static const int CLASS_NAME_COMPOUND = 4;
		static const int CLASS_NAME_LOFT = 5;
		static const int CLASS_NAME_IS_SHAPE = 6;
		static const int CLASS_NAME_POINT = 7;
		static const int CLASS_NAME_PLANE_POINTS = 8;
		static const int CLASS_NAME_TRANSLATION = 9;
		static const int CLASS_NAME_ROTATION = 10;
		static const int CLASS_NAME_SCALE = 11;
		static const int CLASS_NAME_MATRIX = 12;
		static const int CLASS_NAME_IS_TRANSFORMATION = 13;
		static const int CLASS_NAME_FRAME = 14;

		int currentFunctionName;
		float currentFloat;

		bool parseFile(const std::string& filename);
		bool updateIterator(unsigned char* const data, size_t count);
		bool processClassName();
		bool processIdentifier();
		bool processFunctionName();
		bool processFunctionArgument();
		bool processFloat();
		bool processListElement();
		bool processInterpolationFunction();
		bool finalizeClass();
		bool error(const char* const message) const;

		Workspace* const outputWorkspace;
};
#endif // PARSER_H
