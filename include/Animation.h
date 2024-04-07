#ifndef ANIMATION_H
#define ANIMATION_H
#include "Printable.h"
#include "Printer.h"
#include "Frame.h"
#include <string>
class Point;
class PlanePoints;
class Shape;
class Box;
class Cone;
class Cylinder;
class Sphere;
class Compound;
class Loft;
class Transformation;
class Translate;
class Rotate;
class Scale;
class Animation {
	public:
		PRINTABLE_CLASS

		Animation();

		void addFrame(const float time);
		void addFrame(Frame* const frame);
		bool applyToLastFrame(const int function, const float argument);
		bool addFrameFunction(const int type, Point* const point, const int selector, const float value);
		bool addFrameFunction(const int type, PlanePoints* const planePoints, const int selector, const float value);
		bool addFrameFunction(const int type, Shape* const shape, const int selector, const float value);
		bool addFrameFunction(const int type, Box* const box, const int selector, const float value);
		bool addFrameFunction(const int type, Cone* const cone, const int selector, const float value);
		bool addFrameFunction(const int type, Cylinder* const cylinder, const int selector, const float value);
		bool addFrameFunction(const int type, Sphere* const sphere, const int selector, const float value);
		bool addFrameFunction(const int type, Compound* const compound, const int selector, const float value);
		bool addFrameFunction(const int type, Loft* const loft, const int selector, const float value);
		bool addFrameFunction(const int type, Transformation* const transformation, const int selector, const float value);
		bool addFrameFunction(const int type, Translate* const translate, const int selector, const float value);
		bool addFrameFunction(const int type, Rotate* const rotate, const int selector, const float value);
		bool addFrameFunction(const int type, Scale* const scale, const int selector, const float value);

		enum {
			NONE,
			X,
			Y,
			Z,
			LENGTH,
			WIDTH,
			HEIGHT,
			RADIUS,
			ANGLE
		};

		void reset();
		bool build();
		bool getIsInitialized() const;
		void deleteAllFrames();

		bool tick(const float time);

		static int selectorFromWorkspaceString(const std::string& string);
		static void getWorkspaceSelectorString(const int selector, std::string& output);
		static void getSelectorString(const int selector, std::string& output);

		PRINTABLE_DECLARE_PRINT(Animation)

		~Animation();
	private:
		bool isInitialized;
		std::vector<Frame*> frames;
		int frameIndex;
		Frame* current;
		Frame* next;

		bool addFrameFunction(const int type, float* const pointer, const float value, void* const receiver, const int receiverType, const int selector);
};
#endif // ANIMATION_H
