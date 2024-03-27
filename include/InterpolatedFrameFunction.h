#ifndef INTERPOLATED_FRAME_FUNCTION_H
#define INTERPOLATED_FRAME_FUNCTION_H
#include "FrameFunction.h"
class InterpolatedFrameFunction : public FrameFunction {
	public:
		InterpolatedFrameFunction(const int type, float* const pointer, const float start, const float end);

		void tick(const float percentage) const;

		void setTo(const float percentage) const;
		void linearTo(const float percentage) const;

		virtual ~InterpolatedFrameFunction();
	private:
		const float end;
		void (InterpolatedFrameFunction::*const interpolate)(const float) const;
};
#endif // INTERPOLATED_FRAME_FUNCTION_H
