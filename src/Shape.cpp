#include "Shape.h"
#include "Workspace.h"
#include "Parser.h"

Shape::Shape() :
	Shape(0.0f, 0.0f, 0.0f)
{
	this->initializeShader();
}

Shape::Shape(const Vec3f* const position) :
	isVisible(true),
	transformations(),
	needsPositionBuffer(true),
	positionBuffer(0),
	vertices(),
	needsColorBuffer(true),
	colorBuffer(0),
	colors(),
	needsNormalBuffer(true),
	normalBuffer(0),
	normals(),
	needsTextureBuffer(true),
	textureBuffer(0),
	textures(),
	needsElementBuffer(true),
	elementBuffer(0),
	indices(),
	needsShader(true),
	shader(0),
	workspace(0),
	workspaceIndex(0),
	referenceCount(0),
	position(position),
	identifier("shape")
{
	this->initializeShader();
}

Shape::Shape(const float x, const float y, const float z) :
	isVisible(true),
	transformations(),
	needsPositionBuffer(true),
	positionBuffer(0),
	vertices(),
	needsColorBuffer(true),
	colorBuffer(0),
	colors(),
	needsNormalBuffer(true),
	normalBuffer(0),
	normals(),
	needsTextureBuffer(true),
	textureBuffer(0),
	textures(),
	needsElementBuffer(true),
	elementBuffer(0),
	indices(),
	needsShader(true),
	shader(0),
	workspace(0),
	workspaceIndex(0),
	referenceCount(0),
	position(x, y, z),
	identifier("shape")
{
	this->initializeShader();
}

const Vec3f* Shape::getPosition() const{
	return &this->position;
}

const std::string& Shape::getIdentifier() const{
	return this->identifier;
}

void Shape::setWorkspace(Workspace* const workspace){
	this->workspace = workspace;
}

void Shape::setIdentifier(const std::string& identifier){
	this->identifier = identifier;
}

void Shape::setIdentifier(const int sequenceNumber){
	this->identifier = "shape" + std::to_string(sequenceNumber);
}

void Shape::addPosition(const Vec3f* const other){
	this->position.add(other);
}

bool Shape::getIsVisible() const{
	return this->isVisible;
}

void Shape::setIsVisible(const bool isVisible){
	this->isVisible = isVisible;
}

void Shape::show(){
	this->isVisible = true;
}

void Shape::hide(){
	this->isVisible = false;
}

void Shape::transformToWorld(const Vec3f* const position, Mat4f* const model) const{
	const size_t transformationsSize = this->transformations.size();
	Mat4f result;
	if((transformationsSize % 2) > 0){
		result.translate(position);
		for(size_t i = 1; i < transformationsSize; i += 2){
			result.multiply(this->transformations[i - 1]->getMatrix(), model);
			model->multiply(this->transformations[i]->getMatrix(), &result);
		}
		result.multiply(this->transformations[transformationsSize - 1]->getMatrix(), model);
	}else{
		model->translate(position);
		for(size_t i = 0; i < transformationsSize; i += 2){
			model->multiply(this->transformations[i]->getMatrix(), &result);
			result.multiply(this->transformations[i + 1]->getMatrix(), model);
		}
	}
}

void Shape::addTransformation(Transformation* const transformation){
	++transformation->referenceCount;
	this->transformations.push_back(transformation);
}

bool Shape::addTransformationFromWorkspace(const std::string& identifier){
	Transformation* transformation;
	this->workspace->findTransformation(identifier, &transformation);
	if(transformation){
		this->addTransformation(transformation);
		return true;
	}else{
		return false;
	}
}

void Shape::initializeShader(){
	if(this->needsShader){
		int compileStatus = 0;
		char error[256];
		const char* vertexShaderSource = "#version 330 core\n"
			"in vec3 position;\n"
			"attribute vec4 color;\n"
			"uniform mat4 transformations;\n"
			"\n"
			"out vec4 vertexColor;\n"
			"\n"
			"void main(){\n"
				"gl_Position = transformations * vec4(position, 1);\n"
				"vertexColor = color;\n"
			"}\0";
		const char* fragmentShaderSource = "#version 330 core\n"
			"in vec4 vertexColor;\n"
			"\n"
			"out vec4 fragColor;\n"
			"\n"
			"void main(){\n"
				"fragColor = vertexColor;\n"
			"}\0";
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
		if(compileStatus != GL_TRUE){
			glGetShaderInfoLog(vertexShader, 256, NULL, error);
			puts("Vertex shader did not compile:");
			printf("%s\n\n", error);
		}
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
		if(compileStatus != GL_TRUE){
			glGetShaderInfoLog(fragmentShader, 256, NULL, error);
			puts("Fragment shader did not compile:");
			printf("%s\n\n", error);
		}
		this->shader = glCreateProgram();
		glAttachShader(this->shader, vertexShader);
		glAttachShader(this->shader, fragmentShader);
		glLinkProgram(this->shader);
		glGetProgramiv(this->shader, GL_LINK_STATUS, &compileStatus);
		if(compileStatus != GL_TRUE){
			glGetProgramInfoLog(this->shader, 256, NULL, error);
			puts("Shader did not link:");
			printf("%s\n\n", error);
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		this->needsShader = false;
	}
}

void Shape::initializeVertexBuffers(){
	this->vertices.clear();
	this->colors.clear();
	this->normals.clear();
	this->textures.clear();
	this->indices.clear();
}

bool Shape::apply(const int function, const float argument){
	switch(function){
	case Parser::FUNCTION_NAME_X:
		this->position.setX(argument);
		return true;
	case Parser::FUNCTION_NAME_Y:
		this->position.setY(argument);
		return true;
	case Parser::FUNCTION_NAME_Z:
		this->position.setZ(argument);
		return true;
	case Parser::FUNCTION_NAME_VISIBLE:
		if(argument <= 0){
			this->hide();
		}else{
			this->show();
		}
		return true;
	default:
		return false;
	}
}

Shape::~Shape(){}
