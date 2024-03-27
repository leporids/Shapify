#ifndef INDENTED_STREAM_H
#define INDENTED_STREAM_H
#include <iostream>
#include <string>
class IndentedStream {
	public:
		IndentedStream();
		IndentedStream(std::ostream& stream);
		IndentedStream(std::ostream& stream, unsigned int indentation);

		IndentedStream& operator<<(const std::string& value);
		IndentedStream& operator<<(const float value);

		IndentedStream& increaseIndent();
		IndentedStream& decreaseIndent();
		IndentedStream& indent();
		IndentedStream& indentOnce();

		IndentedStream& endl();

		~IndentedStream();
	private:
		std::ostream& stream;
		unsigned int indentation;
};
#endif // INDENTED_STREAM_H
