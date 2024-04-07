#include <GL/glew.h>
#include <GL/glut.h>
#include "Grid.h"
#include "units.h"

Grid::Grid() :
	Grid(FROM_MM(1), FROM_MM(100))
{}

Grid::Grid(const int gridSize, const int gridBounds) :
	gridSize(gridSize),
	gridBounds(gridBounds),
	gridEnabled(true),
	mode(Grid::XY),
	needsShader(true),
	shader(0),
	needsPositionBuffer(true),
	positionBufferXY(0),
	positionBufferYZ(0),
	positionBufferXZ(0),
	vertices(),
	numberOfLines(0)
{
	this->validateGridSize();
}

int Grid::getGridSize() const{
	return this->gridSize;
}

int Grid::getGridBounds() const{
	return this->gridBounds;
}

void Grid::setGridSize(const int gridSize){
	this->gridSize = gridSize;
	this->validateGridSize();
	this->initializeVertexBuffers();
}

void Grid::setMode(const int mode){
	this->mode = mode;
}

void Grid::validateGridSize(){
	if(this->gridSize < Grid::MINIMUM_GRID_SIZE){
		this->gridSize = Grid::MINIMUM_GRID_SIZE;
	}
	if(this->gridSize > Grid::MAXIMUM_GRID_SIZE){
		this->gridSize = Grid::MAXIMUM_GRID_SIZE;
	}
}

bool Grid::isEnabled() const{
	return this->gridEnabled;
}

void Grid::enable(){
	this->gridEnabled = true;
}

void Grid::disable(){
	this->gridEnabled = false;
}

void Grid::decrease(){
	this->gridSize /= 10;
	if(this->gridSize < Grid::MINIMUM_GRID_SIZE){
		this->gridSize = Grid::MINIMUM_GRID_SIZE;
	}
	this->initializeVertexBuffers();
}

void Grid::increase(){
	this->gridSize *= 10;
	if(this->gridSize > Grid::MAXIMUM_GRID_SIZE){
		this->gridSize = Grid::MAXIMUM_GRID_SIZE;
	}
	this->initializeVertexBuffers();
}

void Grid::initializeShaders(){
	if(this->needsShader){
		int compileStatus = 0;
		char error[256];
		const char* vertexShaderSource = "#version 330 core\n"
			"uniform mat4 projection;\n"
			"uniform mat4 view;\n"
			"\n"
			"in vec3 position;\n"
			"\n"
			"void main(){\n"
				"gl_Position = projection * view * vec4(position, 1);\n"
			"}\0";
		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 color;\n"
			"\n"
			"void main(){\n"
				"color = vec4(0, 0, 0, 1);\n"
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

void Grid::initializeVertexBuffers(){
	if(this->needsPositionBuffer){
		glGenBuffers(1, &this->positionBufferXY);
		glGenBuffers(1, &this->positionBufferYZ);
		glGenBuffers(1, &this->positionBufferXZ);
		this->needsPositionBuffer = false;
	}
	this->vertices.clear();
	this->numberOfLines = 0;
	for(int i = 0; i < this->gridBounds; i += gridSize){
		PUSH_BACK_3F(this->vertices, (float)i, -(float)this->gridBounds, 0.0f)
		PUSH_BACK_3F(this->vertices, (float)i, (float)this->gridBounds, 0.0f)
		PUSH_BACK_3F(this->vertices, -(float)i, -(float)this->gridBounds, 0.0f)
		PUSH_BACK_3F(this->vertices, -(float)i, (float)this->gridBounds, 0.0f)
		PUSH_BACK_3F(this->vertices, -(float)this->gridBounds, (float)i, 0.0f)
		PUSH_BACK_3F(this->vertices, (float)this->gridBounds, (float)i, 0.0f)
		PUSH_BACK_3F(this->vertices, -(float)this->gridBounds, -(float)i, 0.0f)
		PUSH_BACK_3F(this->vertices, (float)this->gridBounds, -(float)i, 0.0f)
		this->numberOfLines += 4;
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBufferXY);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), (void*)this->vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->vertices.clear();
	for(int i = 0; i < this->gridBounds; i += gridSize){
		PUSH_BACK_3F(this->vertices, 0.0f, (float)i, -(float)this->gridBounds)
		PUSH_BACK_3F(this->vertices, 0.0f, (float)i, (float)this->gridBounds)
		PUSH_BACK_3F(this->vertices, 0.0f, -(float)i, -(float)this->gridBounds)
		PUSH_BACK_3F(this->vertices, 0.0f, -(float)i, (float)this->gridBounds)
		PUSH_BACK_3F(this->vertices, 0.0f, -(float)this->gridBounds, (float)i)
		PUSH_BACK_3F(this->vertices, 0.0f, (float)this->gridBounds, (float)i)
		PUSH_BACK_3F(this->vertices, 0.0f, -(float)this->gridBounds, -(float)i)
		PUSH_BACK_3F(this->vertices, 0.0f, (float)this->gridBounds, -(float)i)
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBufferYZ);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), (void*)this->vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->vertices.clear();
	for(int i = 0; i < this->gridBounds; i += gridSize){
		PUSH_BACK_3F(this->vertices, (float)i, 0.0f, -(float)this->gridBounds)
		PUSH_BACK_3F(this->vertices, (float)i, 0.0f, (float)this->gridBounds)
		PUSH_BACK_3F(this->vertices, -(float)i, 0.0f, -(float)this->gridBounds)
		PUSH_BACK_3F(this->vertices, -(float)i, 0.0f, (float)this->gridBounds)
		PUSH_BACK_3F(this->vertices, -(float)this->gridBounds, 0.0f, (float)i)
		PUSH_BACK_3F(this->vertices, (float)this->gridBounds, 0.0f, (float)i)
		PUSH_BACK_3F(this->vertices, -(float)this->gridBounds, 0.0f, -(float)i)
		PUSH_BACK_3F(this->vertices, (float)this->gridBounds, 0.0f, -(float)i)
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBufferXZ);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), (void*)this->vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->vertices.clear();
}

void Grid::render(const Camera* const camera) const{
	if(!this->gridEnabled){
		return;
	}
	switch(this->mode){
	case Grid::XY:
		this->renderOnXYPlane(camera->getProjectionMatrix(), camera->getViewMatrix());
		break;
	case Grid::YZ:
		this->renderOnYZPlane(camera->getProjectionMatrix(), camera->getViewMatrix());
		break;
	case Grid::XZ:
		this->renderOnXZPlane(camera->getProjectionMatrix(), camera->getViewMatrix());
		break;
	default:
		break;
	}
}

void Grid::renderOnXYPlane(const Mat4f* const projection, const Mat4f* const view) const{
	glUseProgram(this->shader);
	GRID_UNIFORM_MATRICES(projection, view)
	GRID_DRAW_ELEMENTS(XY, this->numberOfLines)
}

void Grid::renderOnYZPlane(const Mat4f* const projection, const Mat4f* const view) const{
	glUseProgram(this->shader);
	GRID_UNIFORM_MATRICES(projection, view)
	GRID_DRAW_ELEMENTS(YZ, this->numberOfLines)
}

void Grid::renderOnXZPlane(const Mat4f* const projection, const Mat4f* const view) const{
	glUseProgram(this->shader);
	GRID_UNIFORM_MATRICES(projection, view)
	GRID_DRAW_ELEMENTS(XZ, this->numberOfLines)
}

Grid::~Grid(){}
