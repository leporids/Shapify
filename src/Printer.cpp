#include "Printer.h"

Printer::Printer(IndentedStream* const stream)
	: stream(stream)
{}

IndentedStream* Printer::getStream(){
	return this->stream;
}

void Printer::setStream(IndentedStream* const stream){
	this->stream = stream;
}

Printer::~Printer(){}
