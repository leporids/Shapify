#include "Parser.h"

Parser::Parser(Workspace* const outputWorkspace) :
	file(0),
	iterator(0),
	lineCounter(1),
	columnCounter(0),
	state(0),
	currentWord{},
	currentClassName(0),
	currentFunctionName(0),
	currentFloat(0),
	outputWorkspace(outputWorkspace)
{}

bool Parser::parseDirectory(const char* const directory){
	std::string points(directory);
	std::string shapes(directory);
	std::string transformations(directory);
	std::string animations(directory);
	points += "/points.txt";
	shapes += "/shapes.txt";
	transformations += "/transformations.txt";
	animations += "/animations.txt";
	if(this->parseFile(points) && this->parseFile(transformations) && this->parseFile(shapes) && this->parseFile(animations)){
		this->outputWorkspace->directory = std::string(directory);
		return true;
	}
	return false;
}

bool Parser::parseFile(const std::string& filename){
	bool result = true;
	unsigned char data[Parser::DATA_LENGTH];
	size_t count = 0;
	memset(data, 0, Parser::DATA_LENGTH);
#ifdef PARSER_FOPEN
	FILE* file = fopen(filename.c_str(), "r");
#else
	FILE* file = 0;
	fopen_s(&file, filename.c_str(), "r");
#endif
	if(file){
		this->file = file;
		this->iterator = 0;
		this->lineCounter = 1;
		this->columnCounter = 0;
		this->state = Parser::READING_CLASS_NAME;
		memset(currentWord, 0, Parser::CURRENT_WORD_LENGTH);
		while((count = fread(data, 1, Parser::DATA_LENGTH, this->file))){
			if(!this->updateIterator(data, count)){
				result = false;
				break;
			}
		}
		fclose(this->file);
		this->file = 0;
		if((this->state == Parser::READING_FUNCTION_NAME) && (this->iterator == 0)){
			if(!this->finalizeClass()){
				return this->error("Can not finalize redeclaration.");
			}
		}
	}else{
		this->lineCounter = 0;
		this->columnCounter = 0;
		return this->error("File could not be opened.");
	}
	return result;
}

bool Parser::updateIterator(unsigned char* const data, size_t count){
	for(size_t i = 0; i < count; ++i){
		if(data[i] == '\n'){
			this->columnCounter = 0;
			++this->lineCounter;
		}else{
			++this->columnCounter;
		}
		if(this->state == Parser::READING_CLASS_NAME){
			if(isalnum(data[i])){
				if(this->iterator < Parser::CURRENT_WORD_LENGTH){
					this->currentWord[this->iterator] = data[i];
					++this->iterator;
				}else{
					return this->error("Class name is too long.");
				}
			}else if(data[i] == ':'){
				if(!this->processClassName()){
					return this->error("Unrecognized class name.");
				}
				this->state = Parser::READING_IDENTIFIER;
				this->iterator = 0;
			}else if((data[i] == '\n') && (this->iterator == 0)){
				this->state = Parser::READING_CLASS_NAME;
				this->iterator = 0;
			}else{
				return this->error("Class name contains invalid character.");
			}
		}else if(this->state == Parser::READING_IDENTIFIER){
			if(isalnum(data[i])){
				if(this->iterator < Parser::CURRENT_WORD_LENGTH){
					this->currentWord[this->iterator] = data[i];
					++this->iterator;
				}else{
					return this->error("Class identifier is too long.");
				}
			}else if(data[i] == '\n'){
				this->processIdentifier();
				this->state = Parser::READING_FUNCTION_NAME;
				this->iterator = 0;
			}else if(isspace(data[i])){
				if(this->iterator > 0){
					return this->error("Identifier can not contain whitespace.");
				}
			}else{
				return this->error("Class identifier contains invalid character.");
			}
		}else if(this->state == Parser::READING_FUNCTION_NAME){
			if(isalnum(data[i])){
				if(this->iterator < Parser::CURRENT_WORD_LENGTH){
					this->currentWord[this->iterator] = data[i];
					++this->iterator;
				}else{
					return this->error("Function name is too long.");
				}
			}else if(data[i] == ':'){
				if(!this->processFunctionName()){
					return this->error("Unrecognized function name.");
				}
				this->state = Parser::READING_FUNCTION_ARGUMENT;
				this->iterator = 0;
			}else if((data[i] == '\n') && (this->iterator == 0)){
				if(!this->finalizeClass()){
					return this->error("Can not finalize redeclaration.");
				}
				this->state = Parser::READING_CLASS_NAME;
			}else if(isspace(data[i])){
				if(this->iterator > 0){
					return this->error("Function name can not contain whitespace.");
				}
			}else{
				return this->error("Function name contains invalid characters.");
			}
		}else if(this->state == Parser::READING_FUNCTION_ARGUMENT){
			if(isalnum(data[i]) || (data[i] == '+') || (data[i] == '-') || (data[i] == '.')){
				if(this->iterator < Parser::CURRENT_WORD_LENGTH){
					this->currentWord[this->iterator] = data[i];
					++this->iterator;
				}else{
					return this->error("Function argument is too long.");
				}
			}else if(data[i] == '\n'){
				if(!this->processFunctionArgument()){
					return this->error("Invalid argument to function.");
				}
				this->state = Parser::READING_FUNCTION_NAME;
				this->iterator = 0;
			}else if((data[i] == ',') && (this->currentFunctionName > Parser::FUNCTION_NAME_IS_NUMERIC) && (this->currentFunctionName < Parser::FUNCTION_NAME_IS_LIST)){
				if(!this->processFunctionArgument()){
					return this->error("Invalid argument to function.");
				}
				this->iterator = 0;
			}else if(((data[i] == ',') || (data[i] == '.')) && (this->currentFunctionName > Parser::FUNCTION_NAME_IS_LIST) && (this->currentFunctionName < Parser::FUNCTION_NAME_IS_INTERPOLATION)){
				if(this->iterator < Parser::CURRENT_WORD_LENGTH){
					this->currentWord[this->iterator] = data[i];
					++this->iterator;
				}else{
					return this->error("Function argument is too long.");
				}
			}else if(isspace(data[i])){
				if(this->iterator > 0){
					return this->error("Function argument can not contain whitespace.");
				}
			}else{
				return this->error("Function argument contains invalid characters.");
			}
		}
	}
	return true;
}

bool Parser::processClassName(){
	if((this->iterator > 2) && (strncmp(this->currentWord, "Box", 3) == 0)){
		this->currentClassName = Parser::CLASS_NAME_BOX;
		this->outputWorkspace->newShapeFromParser((Shape*)(new Box()));
		return true;
	}else if((this->iterator > 3) && (strncmp(this->currentWord, "Cone", 4) == 0)){
		this->currentClassName = Parser::CLASS_NAME_CONE;
		this->outputWorkspace->newShapeFromParser((Shape*)(new Cone()));
		return true;
	}else if((this->iterator > 7) && (strncmp(this->currentWord, "Cylinder", 8) == 0)){
		this->currentClassName = Parser::CLASS_NAME_CYLINDER;
		this->outputWorkspace->newShapeFromParser((Shape*)(new Cylinder()));
		return true;
	}else if((this->iterator > 5) && (strncmp(this->currentWord, "Sphere", 6) == 0)){
		this->currentClassName = Parser::CLASS_NAME_SPHERE;
		this->outputWorkspace->newShapeFromParser((Shape*)(new Sphere()));
		return true;
	}else if((this->iterator > 7) && (strncmp(this->currentWord, "Compound", 8) == 0)){
		this->currentClassName = Parser::CLASS_NAME_COMPOUND;
		this->outputWorkspace->newShapeFromParser((Shape*)(new Compound()));
		return true;
	}else if((this->iterator > 3) && (strncmp(this->currentWord, "Loft", 4) == 0)){
		this->currentClassName = Parser::CLASS_NAME_LOFT;
		return false;
	}else if((this->iterator > 4) && (strncmp(this->currentWord, "Point", 5) == 0)){
		this->currentClassName = Parser::CLASS_NAME_POINT;
		this->outputWorkspace->newPointFromParser((Point*)(new Point()));
		return true;
	}else if((this->iterator > 10) && (strncmp(this->currentWord, "PlanePoints", 11) == 0)){
		this->currentClassName = Parser::CLASS_NAME_PLANE_POINTS;
		this->outputWorkspace->newPointFromParser((Point*)(new PlanePoints()));
		return true;
	}else if((this->iterator > 8) && (strncmp(this->currentWord, "Translate", 9) == 0)){
		this->currentClassName = Parser::CLASS_NAME_TRANSLATION;
		this->outputWorkspace->newTransformationFromParser((Transformation*)(new Translate(0.0f, 0.0f, 0.0f)));
		return true;
	}else if((this->iterator > 5) && (strncmp(this->currentWord, "Rotate", 6) == 0)){
		this->currentClassName = Parser::CLASS_NAME_ROTATION;
		this->outputWorkspace->newTransformationFromParser((Transformation*)(new Rotate(0.0f, 0.0f, 0.0f, 0.0f)));
		return true;
	}else if((this->iterator > 4) && (strncmp(this->currentWord, "Scale", 5) == 0)){
		this->currentClassName = Parser::CLASS_NAME_SCALE;
		this->outputWorkspace->newTransformationFromParser((Transformation*)(new Scale(0.0f, 0.0f, 0.0f)));
		return true;
	}else if((this->iterator > 5) && (strncmp(this->currentWord, "Matrix", 6) == 0)){
		this->currentClassName = Parser::CLASS_NAME_MATRIX;
		return false;
	}else if((this->iterator > 4) && (strncmp(this->currentWord, "Frame", 5) == 0)){
		this->currentClassName = Parser::CLASS_NAME_FRAME;
		this->outputWorkspace->newFrameFromParser((Frame*)(new Frame(0.0f)));
		return true;
	}
	return false;
}

bool Parser::processIdentifier(){
	std::string identifier(this->currentWord, this->iterator);
	if(this->currentClassName < Parser::CLASS_NAME_IS_SHAPE){
		this->outputWorkspace->shapes.back()->setIdentifier(identifier);
		return true;
	}else if((this->currentClassName == Parser::CLASS_NAME_POINT) || (this->currentClassName == Parser::CLASS_NAME_PLANE_POINTS)){
		this->outputWorkspace->points.back()->setIdentifier(identifier);
		return true;
	}else if(this->currentClassName < Parser::CLASS_NAME_IS_TRANSFORMATION){
		this->outputWorkspace->transformations.back()->setIdentifier(identifier);
		return true;
	}else if(this->currentClassName == Parser::CLASS_NAME_FRAME){
		return true;
	}
	return false;
}

bool Parser::processFunctionName(){
	if((this->iterator > 0) && (strncmp(this->currentWord, "x", 1) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_X;
		return true;
	}else if((this->iterator > 0) && (strncmp(this->currentWord, "y", 1) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_Y;
		return true;
	}else if((this->iterator > 0) && (strncmp(this->currentWord, "z", 1) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_Z;
		return true;
	}else if((this->iterator > 5) && (strncmp(this->currentWord, "length", 6) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_LENGTH;
		return true;
	}else if((this->iterator > 4) && (strncmp(this->currentWord, "width", 5) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_WIDTH;
		return true;
	}else if((this->iterator > 5) && (strncmp(this->currentWord, "height", 6) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_HEIGHT;
		return true;
	}else if((this->iterator > 5) && (strncmp(this->currentWord, "radius", 6) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_RADIUS;
		return true;
	}else if((this->iterator > 4) && (strncmp(this->currentWord, "angle", 5) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_ANGLE;
		return true;
	}else if((this->iterator > 3) && (strncmp(this->currentWord, "time", 4) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_TIME;
		return true;
	}else if((this->iterator > 3) && (strncmp(this->currentWord, "type", 4) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_TYPE;
		return true;
	}else if((this->iterator > 6) && (strncmp(this->currentWord, "visible", 7) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_VISIBLE;
		return true;
	}else if((this->iterator > 5) && (strncmp(this->currentWord, "points", 6) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_POINTS;
		return true;
	}else if((this->iterator > 14) && (strncmp(this->currentWord, "transformations", 15) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_TRANSFORMATIONS;
		return true;
	}else if((this->iterator > 5) && (strncmp(this->currentWord, "shapes", 6) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_SHAPES;
		return true;
	}else if((this->iterator > 4) && (strncmp(this->currentWord, "setTo", 5) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_SET_TO;
		return true;
	}else if((this->iterator > 7) && (strncmp(this->currentWord, "linearTo", 8) == 0)){
		this->currentFunctionName = Parser::FUNCTION_NAME_LINEAR_TO;
		return true;
	}
	return false;
}

bool Parser::processFunctionArgument(){
	if(this->currentFunctionName < Parser::FUNCTION_NAME_IS_NUMERIC){
		if(this->processFloat()){
			if(this->currentClassName < Parser::CLASS_NAME_IS_SHAPE){
				return this->outputWorkspace->shapes.back()->apply(this->currentFunctionName, this->currentFloat);
			}else if((this->currentClassName == Parser::CLASS_NAME_POINT) || (this->currentClassName == Parser::CLASS_NAME_PLANE_POINTS)){
				return this->outputWorkspace->points.back()->apply(this->currentFunctionName, this->currentFloat);
			}else if(this->currentClassName < Parser::CLASS_NAME_IS_TRANSFORMATION){
				return this->outputWorkspace->transformations.back()->apply(this->currentFunctionName, this->currentFloat);
			}else if(this->currentClassName == Parser::CLASS_NAME_FRAME){
				return this->outputWorkspace->applyToLastFrame(this->currentFunctionName, this->currentFloat);
			}else{
				return false;
			}
		}else{
			return false;
		}
	}else if(this->currentFunctionName < Parser::FUNCTION_NAME_IS_LIST){
		return this->processListElement();
	}else if(this->currentFunctionName < Parser::FUNCTION_NAME_IS_INTERPOLATION){
		return this->processInterpolationFunction();
	}
	return true;
}

bool Parser::processFloat(){
	float result = 0.0f;
	float fraction = 0.0f;
	float decimals = 1.0f;
	float factor = 1.0f;
	bool sign = false;
	bool decimal = false;
	for(int i = 0; i < this->iterator; ++i){
		if((this->currentWord[i] == '+') || (this->currentWord[i] == '-')){
			if(this->currentWord[i] == '-'){
				factor *= -1;
			}
			if((!sign) && (!decimal)){
				sign = true;
			}else{
				return false;
			}
		}else if(this->currentWord[i] == '.'){
			if(!decimal){
				decimal = true;
			}else{
				return false;
			}
		}else if(isdigit(this->currentWord[i])){
			if(decimal){
				fraction = (fraction * 10) + (this->currentWord[i] - '0');
				decimals *= 10;
			}else{
				result = (result * 10) + (this->currentWord[i] - '0');
			}
			sign = true;
		}else{
			return false;
		}
	}
	this->currentFloat = (result + (fraction / decimals)) * factor;
	return true;
}

bool Parser::processListElement(){
	std::string listElement(this->currentWord, this->iterator);
	if(this->currentClassName < Parser::CLASS_NAME_IS_SHAPE){
		return this->outputWorkspace->shapes.back()->apply(this->currentFunctionName, listElement);
	}else if(this->currentClassName == Parser::CLASS_NAME_PLANE_POINTS){
		return this->outputWorkspace->points.back()->apply(this->currentFunctionName, listElement);
	}
	return false;
}

bool Parser::processInterpolationFunction(){
	int type = (this->currentFunctionName == Parser::FUNCTION_NAME_SET_TO) ? FrameFunction::SET_TO : FrameFunction::LINEAR_TO;
	std::string identifier;
	std::string selector;
	std::string value;
	int readString = 0;
	for(int i = 0; i < this->iterator; ++i){
		if((readString == 0) && (this->currentWord[i] == '.')){
			readString = 1;
		}else if((readString == 1) && (this->currentWord[i] == ',')){
			readString = 2;
		}else{
			switch(readString){
			case 0:
				identifier += this->currentWord[i];
				break;
			case 1:
				selector += this->currentWord[i];
				break;
			case 2:
				value += this->currentWord[i];
				break;
			default:
				break;
			}
		}
	}
	if((identifier.size() > 0) && (selector.size() > 0) && (value.size() > 0)){
		return this->outputWorkspace->processInterpolationFunction(type, identifier, Animation::selectorFromWorkspaceString(selector), strtof(value.c_str(), NULL));
	}else{
		return false;
	}
}

bool Parser::finalizeClass(){
	if(this->currentClassName < Parser::CLASS_NAME_IS_SHAPE){
		Shape* const shape = this->outputWorkspace->shapes.back();
		const std::string& identifier = shape->getIdentifier();
		if(this->outputWorkspace->shapesByIdentifier.count(identifier) > 0){
			return false;
		}else{
			this->outputWorkspace->shapesByIdentifier[identifier] = shape;
			return true;
		}
	}else if((this->currentClassName == Parser::CLASS_NAME_POINT) || (this->currentClassName == Parser::CLASS_NAME_PLANE_POINTS)){
		Point* const point = this->outputWorkspace->points.back();
		const std::string& identifier = point->getIdentifier();
		if(this->outputWorkspace->pointsByIdentifier.count(identifier) > 0){
			return false;
		}else{
			this->outputWorkspace->pointsByIdentifier[identifier] = point;
			return true;
		}
	}else if(this->currentClassName < Parser::CLASS_NAME_IS_TRANSFORMATION){
		Transformation* const transformation = this->outputWorkspace->transformations.back();
		const std::string& identifier = transformation->getIdentifier();
		if(this->outputWorkspace->transformationsByIdentifier.count(identifier) > 0){
			return false;
		}else{
			this->outputWorkspace->transformationsByIdentifier[identifier] = transformation;
			return true;
		}
	}
	return true;
}

bool Parser::error(const char* const message) const{
	printf("Line %d, column %d: ", this->lineCounter, this->columnCounter);
	puts(message);
	return false;
}

Parser::~Parser(){
	if(this->file){
		fclose(this->file);
		this->file = 0;
	}
}
