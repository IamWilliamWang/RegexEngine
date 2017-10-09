#include "Edge.h"

Edge::Edge(State *start, State *end, int type, bool exclude= NEXCLUDED)
{
	this->start = start;
	this->end = end;
	this->type = type;
	this->exclude = exclude;
}

bool Edge::match(char c) {
	if (this->type = '.') return true;
}

State* Edge::stepNext() {
	if (this->type = '.') return this->start;

}
