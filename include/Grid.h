#ifndef GRID_H
#define GRID_H
#include "units.h"
#include "Camera.h"
#include <vector>
#include <stdio.h>
#define PUSH_BACK_3F(name, a, b, c) \
	name.push_back(a); \
	name.push_back(b); \
	name.push_back(c);
#define GRID_UNIFORM_MATRICES(projection, view) \
	GLuint location = glGetUniformLocation(this->shader, "projection"); \
	glUniformMatrix4fv(location, 1, GL_TRUE, projection->getEntries()); \
	location = glGetUniformLocation(this->shader, "view"); \
	glUniformMatrix4fv(location, 1, GL_TRUE, view->getEntries());
#define GRID_DRAW_ELEMENTS(plane, count) \
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer##plane); \
	location = glGetAttribLocation(this->shader, "position"); \
	glEnableVertexAttribArray(location); \
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0); \
	glDrawArrays(GL_LINES, 0, count); \
	glBindBuffer(GL_ARRAY_BUFFER, 0); \
	glUseProgram(0);
class Grid {
	public:
		Grid();
		Grid(const int gridSize, const int gridBounds);

		int getGridSize() const;
		int getGridBounds() const;

		void setGridSize(const int gridSize);
		void setMode(const int mode);

		static const int MINIMUM_GRID_SIZE = ONE_TENTH_MILLIMETER;
		static const int MAXIMUM_GRID_SIZE = FROM_MM(10);
		void validateGridSize();

		bool isEnabled() const;
		void enable();
		void disable();

		void decrease();
		void increase();

		void initializeShaders();
		void initializeVertexBuffers();

		void render(const Camera* const camera) const;
		void renderOnXYPlane(const Mat4f* const projection, const Mat4f* const view) const;
		void renderOnYZPlane(const Mat4f* const projection, const Mat4f* const view) const;
		void renderOnXZPlane(const Mat4f* const projection, const Mat4f* const view) const;
		static const int XY = 0;
		static const int YZ = 1;
		static const int XZ = 2;

		~Grid();
	private:
		int gridSize;
		int gridBounds;
		bool gridEnabled;
		int mode;
		bool needsShader;
		GLuint shader;
		bool needsPositionBuffer;
		GLuint positionBufferXY;
		GLuint positionBufferYZ;
		GLuint positionBufferXZ;
		std::vector<float> vertices;
		size_t numberOfLines;
};
#endif // GRID_H
