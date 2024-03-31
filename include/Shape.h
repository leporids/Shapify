#ifndef SHAPE_H
#define SHAPE_H
class Workspace;
class Printer;
#include "Printable.h"
#include "Transformation.h"
#include "Printer.h"
#include "Animated.h"
#include "Animation.h"
#include <vector>
#define PUSH_BACK_3F(name, a, b, c) \
	name.push_back(a); \
	name.push_back(b); \
	name.push_back(c);
#define PUSH_BACK_4F(name, a, b, c, d) \
	name.push_back(a); \
	name.push_back(b); \
	name.push_back(c); \
	name.push_back(d);
#define SHAPE_UNIFORM_MATRICES(projection, view, model) \
	GLuint location = glGetUniformLocation(this->shader, "projection"); \
	glUniformMatrix4fv(location, 1, GL_TRUE, projection->getEntries()); \
	location = glGetUniformLocation(this->shader, "view"); \
	glUniformMatrix4fv(location, 1, GL_TRUE, view->getEntries()); \
	location = glGetUniformLocation(this->shader, "model"); \
	glUniformMatrix4fv(location, 1, GL_TRUE, model.getEntries());
#define SHAPE_ATTRIB_POINTER(attribute, size, type, normalized, stride, pointer) \
	glBindBuffer(GL_ARRAY_BUFFER, this->attribute##Buffer); \
	location = glGetAttribLocation(this->shader, #attribute); \
	glEnableVertexAttribArray(location); \
	glVertexAttribPointer(location, size, type, normalized, stride, (const void*)pointer);
#define SHAPE_UNIFORM_VALUE(glUniform, name, value) \
	location = glGetUniformLocation(this->shader, #name); \
	glUniform(location, value);
#define SHAPE_UNIFORM_VECTOR(glUniform, name, value) \
	location = glGetUniformLocation(this->shader, #name); \
	glUniform(location, 1, value);
#define SHAPE_UNIFORM_DEFAULT_LIGHTING(cameraPositionComponents) \
	SHAPE_UNIFORM_VECTOR(glUniform4fv, ambientLightIntensity, Shape::DEFAULT_LIGHT_PARAMETERS + 0) \
	SHAPE_UNIFORM_VECTOR(glUniform4fv, ambientLightColor, Shape::DEFAULT_LIGHT_PARAMETERS + 4) \
	SHAPE_UNIFORM_VECTOR(glUniform3fv, lightWorldPosition, cameraPositionComponents) \
	SHAPE_UNIFORM_VECTOR(glUniform4fv, diffuseLightIntensity, Shape::DEFAULT_LIGHT_PARAMETERS + 8) \
	SHAPE_UNIFORM_VECTOR(glUniform4fv, diffuseLightColor, Shape::DEFAULT_LIGHT_PARAMETERS + 12) \
	SHAPE_UNIFORM_VECTOR(glUniform3fv, cameraWorldPosition, cameraPositionComponents) \
	SHAPE_UNIFORM_VECTOR(glUniform4fv, specularLightIntensity, Shape::DEFAULT_LIGHT_PARAMETERS + 16) \
	SHAPE_UNIFORM_VECTOR(glUniform4fv, specularLightColor, Shape::DEFAULT_LIGHT_PARAMETERS + 20) \
	SHAPE_UNIFORM_VALUE(glUniform1f, specularLightPower, Shape::DEFAULT_LIGHT_PARAMETERS[24])
#define SHAPE_DRAW_ELEMENTS(count) \
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer); \
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL); \
	glBindBuffer(GL_ARRAY_BUFFER, 0); \
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); \
	glUseProgram(0);
class Shape {
	public:
		const Vec3f* getPosition() const;
		const std::string& getIdentifier() const;

		void setWorkspace(Workspace* const workspace);
		virtual void setIdentifier(const std::string& identifier);
		virtual void setIdentifier(const int sequenceNumber);

		void addPosition(const Vec3f* const other);
		virtual void addSize(const Vec3f* const other) = 0;

		bool getIsVisible() const;
		void setIsVisible(const bool isVisible);
		void show();
		void hide();

		void transformToWorld(const Vec3f* const position, Mat4f* const model) const;
		void addTransformation(Transformation* const transformation);
		bool addTransformationFromWorkspace(const std::string& identifier);

		virtual void initializeShader();
		virtual void initializeVertexBuffers();
		virtual void render(const Mat4f* const projection, const Mat4f* const view, const Mat4f* const parentTransformations, const float* const cameraPositionComponents, const bool isSelected) const = 0;
		virtual bool apply(const int function, const float argument);
		virtual bool apply(const int function, const std::string& listElement) = 0;

		static const int MAX_POINTS_CIRCLE = 50;
		static const int MAX_LAYERS_CIRCLE = 50;

		static const float DEFAULT_LIGHT_PARAMETERS[25];

		static const float DEFAULT_COLOR_R;
		static const float DEFAULT_COLOR_G;
		static const float DEFAULT_COLOR_B;
		static const float DEFAULT_COLOR_A;

		static const float DEFAULT_SELECTED_COLOR_R;
		static const float DEFAULT_SELECTED_COLOR_G;
		static const float DEFAULT_SELECTED_COLOR_B;
		static const float DEFAULT_SELECTED_COLOR_A;

		virtual void print(Printer* const printer) const = 0;

		ANIMATED_DECLARATION

		virtual ~Shape();
	protected:
		Shape();
		Shape(const Vec3f* const position);
		Shape(const float x, const float y, const float z);

		bool isVisible;
		std::vector<Transformation*> transformations;
		bool needsPositionBuffer;
		GLuint positionBuffer;
		std::vector<float> vertices;
		bool needsColorBuffer;
		GLuint colorBuffer;
		std::vector<float> colors;
		bool needsNormalBuffer;
		GLuint normalBuffer;
		std::vector<float> normals;
		bool needsTextureBuffer;
		GLuint textureBuffer;
		std::vector<float> textures;
		bool needsElementBuffer;
		GLuint elementBuffer;
		std::vector<int> indices;
		bool needsShader;
		GLuint shader;
		Workspace* workspace;
		int workspaceIndex;
		int referenceCount;
		Vec3f position;
		std::string identifier;
		PRINTABLE_CLASS_FRIENDSHIP
		friend class Workspace;
		friend class Compound;
		friend class Animation;
};
#endif // SHAPE_H
