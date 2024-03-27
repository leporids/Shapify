#include "Axis2D.h"

const float Axis2D::xColor[3] = {1.0f, 0.0f, 0.0f};
const float Axis2D::yColor[3] = {0.0f, 1.0f, 0.0f};
const float Axis2D::zColor[3] = {0.0f, 0.5f, 1.0f};

Axis2D::Axis2D() :
	Axis2D(Axis2D::ANTERIOR_AXES)
{}

Axis2D::Axis2D(const int mode) :
	mode(mode),
	size(100),
	margin(5),
	triangleLength(15)
{}

int Axis2D::getMode() const{
	return this->mode;
}

void Axis2D::setMode(const int mode){
	this->mode = mode;
}

void Axis2D::render(const int winWidth, const int winHeight) const{
	switch(this->mode){
	case ANTERIOR_AXES:
		this->renderPositivePositive(winWidth, winHeight, Axis2D::xColor, Axis2D::yColor);
		break;
	case LATERAL_LEFT_AXES:
		this->renderNegativePositive(winWidth, winHeight, Axis2D::zColor, Axis2D::yColor);
		break;
	case POSTERIOR_AXES:
		this->renderNegativePositive(winWidth, winHeight, Axis2D::xColor, Axis2D::yColor);
		break;
	case LATERAL_RIGHT_AXES:
		this->renderPositivePositive(winWidth, winHeight, Axis2D::zColor, Axis2D::yColor);
		break;
	case DORSAL_AXES:
		this->renderPositiveNegative(winWidth, winHeight, Axis2D::xColor, Axis2D::zColor);
		break;
	case VENTRAL_AXES:
		this->renderPositivePositive(winWidth, winHeight, Axis2D::xColor, Axis2D::zColor);
		break;
	default:
		break;
	}
}

void Axis2D::renderPositivePositive(const int winWidth, const int winHeight, const float* const horizontalColor, const float* const verticalColor) const{
	const float twoMargin = 2.0f * (float)this->margin;
	const float xOriginPositive = (float)(winWidth - this->size);
	const float xBottomRight = (float)(winWidth - this->margin);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (double)winWidth, 0.0, (double)winHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_LINES);
	glColor4f(horizontalColor[0], horizontalColor[1], horizontalColor[2], 1.0f);
	glVertex3f(xOriginPositive, (float)this->margin, -0.1f);
	glVertex3f(xBottomRight, (float)this->margin, -0.1f);
	glColor4f(verticalColor[0], verticalColor[1], verticalColor[2], 1.0f);
	glVertex3f(xOriginPositive, (float)this->margin, -0.1f);
	glVertex3f(xOriginPositive, (float)this->size, -0.1f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor4f(horizontalColor[0], horizontalColor[1], horizontalColor[2], 1.0f);
	glVertex3f(xBottomRight - (float)this->triangleLength, twoMargin, -0.1f);
	glVertex3f(xBottomRight, (float)this->margin, -0.1f);
	glVertex3f(xBottomRight - (float)this->triangleLength, 0.0f, -0.1f);
	glColor4f(verticalColor[0], verticalColor[1], verticalColor[2], 1.0f);
	glVertex3f(xOriginPositive - (float)this->margin, (float)(this->size - this->triangleLength), -0.1f);
	glVertex3f(xOriginPositive, (float)this->size, -0.1f);
	glVertex3f(xOriginPositive + (float)this->margin, (float)(this->size - this->triangleLength), -0.1f);
	glEnd();
}

void Axis2D::renderNegativePositive(const int winWidth, const int winHeight, const float* const horizontalColor, const float* const verticalColor) const{
	const float twoMargin = 2.0f * (float)this->margin;
	const float xOriginPositive = (float)(winWidth - this->size);
	const float xBottomRight = (float)(winWidth - this->margin);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (double)winWidth, 0.0, (double)winHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_LINES);
	glColor4f(horizontalColor[0], horizontalColor[1], horizontalColor[2], 1.0f);
	glVertex3f(xBottomRight, (float)this->margin, -0.1f);
	glVertex3f(xOriginPositive, (float)this->margin, -0.1f);
	glColor4f(verticalColor[0], verticalColor[1], verticalColor[2], 1.0f);
	glVertex3f(xBottomRight, (float)this->margin, -0.1f);
	glVertex3f(xBottomRight, (float)this->size, -0.1f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor4f(horizontalColor[0], horizontalColor[1], horizontalColor[2], 1.0f);
	glVertex3f(xOriginPositive + (float)this->triangleLength, twoMargin, -0.1f);
	glVertex3f(xOriginPositive, (float)this->margin, -0.1f);
	glVertex3f(xOriginPositive + (float)this->triangleLength, 0.0f, -0.1f);
	glColor4f(verticalColor[0], verticalColor[1], verticalColor[2], 1.0f);
	glVertex3f(xBottomRight - (float)this->margin, (float)(this->size - this->triangleLength), -0.1f);
	glVertex3f(xBottomRight, (float)this->size, -0.1f);
	glVertex3f(xBottomRight + (float)this->margin, (float)(this->size - this->triangleLength), -0.1f);
	glEnd();
}

void Axis2D::renderPositiveNegative(const int winWidth, const int winHeight, const float* const horizontalColor, const float* const verticalColor) const{
	const float xOriginPositive = (float)(winWidth - this->size);
	const float xBottomRight = (float)(winWidth - this->margin);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (double)winWidth, 0.0, (double)winHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_LINES);
	glColor4f(horizontalColor[0], horizontalColor[1], horizontalColor[2], 1.0f);
	glVertex3f(xOriginPositive, (float)this->size, -0.1f);
	glVertex3f(xBottomRight, (float)this->size, -0.1f);
	glColor4f(verticalColor[0], verticalColor[1], verticalColor[2], 1.0f);
	glVertex3f(xOriginPositive, (float)this->size, -0.1f);
	glVertex3f(xOriginPositive, (float)this->margin, -0.1f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor4f(horizontalColor[0], horizontalColor[1], horizontalColor[2], 1.0f);
	glVertex3f(xBottomRight - (float)this->triangleLength, (float)(this->size + this->margin), -0.1f);
	glVertex3f(xBottomRight, (float)this->size, -0.1f);
	glVertex3f(xBottomRight - (float)this->triangleLength, (float)(this->size - this->margin), -0.1f);
	glColor4f(verticalColor[0], verticalColor[1], verticalColor[2], 1.0f);
	glVertex3f(xOriginPositive - (float)this->margin, (float)this->triangleLength, -0.1f);
	glVertex3f(xOriginPositive, 0.0f, -0.1f);
	glVertex3f(xOriginPositive + (float)this->margin, (float)this->triangleLength, -0.1f);
	glEnd();
}

Axis2D::~Axis2D(){}
