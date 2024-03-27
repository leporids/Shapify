#ifndef FRAME_FUNCTION_H
#define FRAME_FUNCTION_H
#include "Printable.h"
#include <string>
class FrameFunction {
	public:
		FrameFunction(const int type, float* const pointer, const float value, void* const receiver, const int receiverType, const int selector);

		static const int MINIMUM_INTERPOLATION_TYPE = 0;
		static const int SET_TO = 0;
		static const int LINEAR_TO = 1;
		static const int MAXIMUM_INTERPOLATION_TYPE = 1;

		static const int RECEIVER_NONE = 0;
		static const int RECEIVER_POINT_VALUE = 1;
		static const int RECEIVER_SHAPE_POSITION = 2;
		static const int RECEIVER_SHAPE_SIZE = 3;
		static const int RECEIVER_TRANSFORMATION_VALUE = 4;
		static const std::string& UNPRINTABLE;

		const std::string& getIdentifier() const;

		static void getWorkspaceTypeString(const int type, std::string& output);
		static void getTypeString(const int type, std::string& output);

		virtual ~FrameFunction();
	protected:
		FrameFunction(const int type, float* const pointer, const float value);

		const int type;
		float* const pointer;
		const float value;
		void* const receiver;
		const int receiverType;
		const int selector;
		PRINTABLE_CLASS_FRIENDSHIP
		friend class Frame;
};
#endif // FRAME_FUNCTION_H
