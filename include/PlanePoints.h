#ifndef PLANE_POINTS_H
#define PLANE_POINTS_H
#include "Point.h"
#include "Animated.h"
class PlanePoints : public Point {
	public:
		PRINTABLE_CLASS

		PlanePoints();
		PlanePoints(const int type, const float x, const float y, const float z);

		int getType() const;
		void setType(const int type);
		static const int MINIMUM_TYPE = 0;
		static const int XY = 0;
		static const int XZ = 1;
		static const int YZ = 2;
		static const int MAXIMUM_TYPE = 2;

		bool addChild(Point* const point);
		bool addChildFromWorkspace(const std::string& identifier);
		virtual void setIdentifier(const int sequenceNumber);

		virtual size_t size() const;

		virtual bool apply(const int function, const float argument);
		virtual bool apply(const int function, const std::string& listElement);

		PRINTABLE_DECLARE_PRINT(Point)
		ANIMATED_DECLARATION

		virtual ~PlanePoints();
	private:
		int type;
		std::vector<Point*> children;
		friend class Loft;
};
#endif // PLANE_POINTS_H
