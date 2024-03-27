#ifndef ROTATE_H 
#define ROTATE_H
#include "Transformation.h"
class Rotate : public Transformation {
	public:
		PRINTABLE_CLASS

		Rotate(const float angle, Vec3f* const components);
		Rotate(const float angle, const float x, const float y, const float z);

		float getAngle() const;
		void setAngle(const float angle);
		void addAngle(const float angle);

		virtual void setIdentifier(const int sequenceNumber);

		virtual void updateMatrix();
		virtual bool apply(const int function, const float argument);

		PRINTABLE_DECLARE_PRINT(Transformation)
		ANIMATED_DECLARATION

		virtual ~Rotate();
	private:
		float angle;
		friend class Animation;
};
#endif // ROTATE_H
