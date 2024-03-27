#ifndef FRAME_H
#define FRAME_H
#include "Printable.h"
#include "FrameFunction.h"
#include "InterpolatedFrameFunction.h"
#include "FrameEnd.h"
class Shape;
class Transformation;
#include <vector>
class Frame {
	public:
		Frame(const float time);

		float getTime() const;
		void setTime(const float time);

		void addFrameFunction(FrameFunction* const frameFunction);

		void buildInterpolationFunctions(const Frame* const previous);
		void newInterpolationFunction(const int type, void* const receiver, const int receiverType, float* const pointer, const float start, const float end);
		void buildFinalFunctions(const Frame* const next);
		void newFinalFunction(void* const receiver, const int receiverType, float* const pointer, const float value);

		float percentageFrom(const float time, const Frame* const previous) const;
		bool tick(const float time, const float percentage) const;
		void finalize() const;

		bool apply(const int function, const float argument);

		~Frame();
	private:
		template <typename T> void addIfAbsentIdentical(std::vector<T*>& collection, T* const item);
		float time;
		std::vector<FrameFunction*> functions;
		std::vector<InterpolatedFrameFunction*> interpolationFunctions;
		std::vector<Shape*> interpolatedShapes;
		std::vector<Transformation*> interpolatedTransformations;
		std::vector<FrameEnd*> finalFunctions;
		std::vector<Shape*> finalShapes;
		std::vector<Transformation*> finalTransformations;
		PRINTABLE_CLASS_FRIENDSHIP
};
#endif // FRAME_H
