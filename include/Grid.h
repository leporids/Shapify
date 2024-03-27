#ifndef GRID_H
#define GRID_H
#include "units.h"
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

		void render() const;
		void renderOnXYPlane() const;
		void renderOnYZPlane() const;
		void renderOnXZPlane() const;
		static const int XY = 0;
		static const int YZ = 1;
		static const int XZ = 2;

		~Grid();
	private:
		int gridSize;
		int gridBounds;
		bool gridEnabled;
		int mode;
};
#endif // GRID_H
