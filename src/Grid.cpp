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
	mode(Grid::XY)
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
}

void Grid::increase(){
	this->gridSize *= 10;
	if(this->gridSize > Grid::MAXIMUM_GRID_SIZE){
		this->gridSize = Grid::MAXIMUM_GRID_SIZE;
	}
}

void Grid::render() const{
	if(!this->gridEnabled){
		return;
	}
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glLineWidth(1.5);
	switch(this->mode){
	case Grid::XY:
		this->renderOnXYPlane();
		break;
	case Grid::YZ:
		this->renderOnYZPlane();
		break;
	case Grid::XZ:
		this->renderOnXZPlane();
		break;
	default:
		break;
	}
}

void Grid::renderOnXYPlane() const{
	glBegin(GL_LINES);
	for(int i = 0; i < this->gridBounds; i += gridSize){
		glVertex3f((float)i, -(float)this->gridBounds, 0);
		glVertex3f((float)i, (float)this->gridBounds, 0);
		glVertex3f(-(float)i, -(float)this->gridBounds, 0);
		glVertex3f(-(float)i, (float)this->gridBounds, 0);
		glVertex3f(-(float)this->gridBounds, (float)i, 0);
		glVertex3f((float)this->gridBounds, (float)i, 0);
		glVertex3f(-(float)this->gridBounds, -(float)i, 0);
		glVertex3f((float)this->gridBounds, -(float)i, 0);
	}
	glEnd();
}

void Grid::renderOnYZPlane() const{
	glBegin(GL_LINES);
	for(int i = 0; i < this->gridBounds; i += gridSize){
		glVertex3f(0, (float)i, -(float)this->gridBounds);
		glVertex3f(0, (float)i, (float)this->gridBounds);
		glVertex3f(0, -(float)i, -(float)this->gridBounds);
		glVertex3f(0, -(float)i, (float)this->gridBounds);
		glVertex3f(0, -(float)this->gridBounds, (float)i);
		glVertex3f(0, (float)this->gridBounds, (float)i);
		glVertex3f(0, -(float)this->gridBounds, -(float)i);
		glVertex3f(0, (float)this->gridBounds, -(float)i);
	}
	glEnd();
}

void Grid::renderOnXZPlane() const{
	glBegin(GL_LINES);
	for(int i = 0; i < this->gridBounds; i += gridSize){
		glVertex3f((float)i, 0, -(float)this->gridBounds);
		glVertex3f((float)i, 0, (float)this->gridBounds);
		glVertex3f(-(float)i, 0, -(float)this->gridBounds);
		glVertex3f(-(float)i, 0, (float)this->gridBounds);
		glVertex3f(-(float)this->gridBounds, 0, (float)i);
		glVertex3f((float)this->gridBounds, 0, (float)i);
		glVertex3f(-(float)this->gridBounds, 0, -(float)i);
		glVertex3f((float)this->gridBounds, 0, -(float)i);
	}
	glEnd();
}

Grid::~Grid(){}
