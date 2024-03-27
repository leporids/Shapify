#ifndef POINT_H
#define POINT_H
#include "Printable.h"
#include "vectors.h"
#include "Printer.h"
class Workspace;
#include "Workspace.h"
#include "Animated.h"
#include "Animation.h"
#include <string>
class Point {
	public:
		PRINTABLE_CLASS

		Point();
		Point(const float x, const float y, const float z);

		void setWorkspace(Workspace* const workspace);
		const Vec3f* getPosition() const;
		const std::string& getIdentifier() const;

		void setPosition(const Vec3f* const position);
		void setPosition(const float x, const float y, const float z);
		virtual void setIdentifier(const std::string& identifier);
		virtual void setIdentifier(const int sequenceNumber);

		virtual size_t size() const;

		void addPosition(const Vec3f* const other);

		virtual bool apply(const int function, const float argument);
		virtual bool apply(const int function, const std::string& listElement);

		PRINTABLE_DECLARE_PRINT(Point)
		ANIMATED_DECLARATION

		virtual ~Point();
	protected:
		Vec3f position;
		Workspace* workspace;
		int workspaceIndex;
		int referenceCount;
		std::string identifier;
		friend class Workspace;
		friend class PlanePoints;
		friend class Animation;
};
#endif // POINT_H
