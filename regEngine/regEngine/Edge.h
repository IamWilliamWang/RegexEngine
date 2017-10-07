#ifndef EDGE_H

#include <vector>
#include "State.h"

class Edge
{
public:
	Edge(State *start, State *end, char character);
	State *start;
	State *end;
	char character;

	bool match(char c);
	State *stepNext();
};

#endif // !EDGE_H

