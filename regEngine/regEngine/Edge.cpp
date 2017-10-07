#include "Edge.h"

Edge::Edge(Status *start, Status *end, char character)
{
	this->start = start;
	this->end = end;
	this->character = character;
}

bool Edge::match(char c) {
	if (this->character = '.') return true;
	if ()
}

Status* Edge::stepNext() {
	if (this->character = '.') return this->start;

}
