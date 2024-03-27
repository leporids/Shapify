#ifndef TRANSLATE_H
#define TRANSLATE_H
#include "Transformation.h"
class Translate : public Transformation {
	public:
		PRINTABLE_CLASS

		Translate(Vec3f* const components);
		Translate(const float x, const float y, const float z);

		virtual void setIdentifier(const int sequenceNumber);

		virtual void updateMatrix();

		PRINTABLE_DECLARE_PRINT(Transformation)
		ANIMATED_DECLARATION

		virtual ~Translate();
};
#endif // TRANSLATE_H
