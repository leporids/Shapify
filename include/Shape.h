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
		virtual void render(const Mat4f* const parentTransformations, const bool isSelected) const = 0;
		virtual bool apply(const int function, const float argument);
		virtual bool apply(const int function, const std::string& listElement) = 0;

		static const int MAX_POINTS_CIRCLE = 50;
		static const int MAX_LAYERS_CIRCLE = 50;

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
