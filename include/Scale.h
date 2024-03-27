#ifndef SCALE_H
#define SCALE_H
#include "Transformation.h"
class Scale : public Transformation {
	public:
		PRINTABLE_CLASS

		Scale(Vec3f* const components);
		Scale(const float x, const float y, const float z);

		virtual void setIdentifier(const int sequenceNumber);

		virtual void updateMatrix();

		PRINTABLE_DECLARE_PRINT(Transformation)
		ANIMATED_DECLARATION

		virtual ~Scale();
};
#endif // SCALE_H
