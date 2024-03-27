#include "IndentedStream.h"

IndentedStream::IndentedStream() :
	IndentedStream(std::cout, 0)
{}

IndentedStream::IndentedStream(std::ostream& stream) :
	IndentedStream(stream, 0)
{}

IndentedStream::IndentedStream(std::ostream& stream, unsigned int indentation) :
	stream(stream),
	indentation(indentation)
{}

IndentedStream& IndentedStream::operator<<(const std::string& value){
	this->stream << value;
	return *this;
}

IndentedStream& IndentedStream::operator<<(const float value){
	this->stream << value;
	return *this;
}

IndentedStream& IndentedStream::increaseIndent(){
	++this->indentation;
	return *this;
}

IndentedStream& IndentedStream::decreaseIndent(){
	if(this->indentation){
		--this->indentation;
	}
	return *this;
}

IndentedStream& IndentedStream::indent(){
	for(unsigned int i = 0; i < this->indentation; ++i){
		this->stream << '\t';
	}
	return *this;
}

IndentedStream& IndentedStream::indentOnce(){
	this->stream << '\t';
	return *this;
}

IndentedStream& IndentedStream::endl(){
	this->stream << std::endl;
	return this->indent();
}

IndentedStream::~IndentedStream(){}
