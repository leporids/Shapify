#ifndef AXIS_2D_H
#define AXIS_2D_H
#include <GL/glut.h>
class Axis2D {
	public:
		Axis2D();
		Axis2D(const int mode);
		static const int ANTERIOR_AXES = 0;
		static const int LATERAL_LEFT_AXES = 1;
		static const int POSTERIOR_AXES = 2;
		static const int LATERAL_RIGHT_AXES = 3;
		static const int DORSAL_AXES = 4;
		static const int VENTRAL_AXES = 5;

		int getMode() const;

		void setMode(const int mode);

		void render(const int winWidth, const int winHeight) const;

		void renderPositivePositive(const int winWidth, const int winHeight, const float* const horizontalColor, const float* const verticalColor) const;
		void renderNegativePositive(const int winWidth, const int winHeight, const float* const horizontalColor, const float* const verticalColor) const;
		void renderPositiveNegative(const int winWidth, const int winHeight, const float* const horizontalColor, const float* const verticalColor) const;
		static const float xColor[3];
		static const float yColor[3];
		static const float zColor[3];

		~Axis2D();
	private:
		int mode;
		int size;
		int margin;
		int triangleLength;
};
#endif // AXIS_2D_H
