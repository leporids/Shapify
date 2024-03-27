#ifndef FRAME_END_H
#define FRAME_END_H
class FrameEnd {
	public:
		FrameEnd(float* const pointer, const float value);

		void finishFrame() const;

		~FrameEnd();
	private:
		float* const pointer;
		const float value;
};
#endif // FRAME_END_H
