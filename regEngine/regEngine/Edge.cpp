#include "Edge.h"

Edge::Edge(State *start, State *end, char character)
{
	this->start = start;
	this->end = end;
	this->character = character;
}

bool Edge::match(char c) {
	if (this->character = '.') return true;
}

State* Edge::stepNext() {
	if (this->character = '.') return this->start;

}
